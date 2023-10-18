#include "common/log/log.h"
#include "sql/operator/update_physical_operator.h"
#include "storage/record/record.h"
#include "storage/trx/trx.h"
#include "sql/stmt/update_stmt.h"

UpdatePhysicalOperator::UpdatePhysicalOperator(Table *table, std::string attribute_name, Value value)
    : table_(table), attribute_name_(attribute_name), value_(value)
{}

RC UpdatePhysicalOperator::open(Trx *trx)
{
  if (children_.empty()) {
    return RC::SUCCESS;
  }

  std::unique_ptr<PhysicalOperator> &child = children_[0];
  RC rc = child->open(trx);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to open child operator: %s", strrc(rc));
    return rc;
  }

  trx_ = trx;

  return RC::SUCCESS;
}

RC UpdatePhysicalOperator::next()
{
  RC rc = RC::SUCCESS;
  if (children_.empty()) {
    return RC::RECORD_EOF;
  }

  PhysicalOperator *child = children_[0].get();
  while (RC::SUCCESS == (rc = child->next())) {
    Tuple *tuple = child->current_tuple();
    if (nullptr == tuple) {
      LOG_WARN("failed to get current record: %s", strrc(rc));
      return rc;
    }

    RowTuple *row_tuple = static_cast<RowTuple *>(tuple);
    Record &delete_record = row_tuple->record();
    Record insert_record(delete_record);
    rc = trx_->delete_record(table_, delete_record);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to delete record: %s", strrc(rc));
      return rc;
    }
    
    const TableMeta &table_meta = table_->table_meta();
    const FieldMeta *field_meta = table_meta.field(attribute_name_.c_str());
    memcpy(insert_record.data() + field_meta->offset(), value_.data(), field_meta->len());
    rc = trx_->insert_record(table_, insert_record);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to insert record: %s", strrc(rc));
      return rc;
    }
  }

  return RC::RECORD_EOF;
}

RC UpdatePhysicalOperator::close()
{
  if (!children_.empty()) {
    children_[0]->close();
  }
  return RC::SUCCESS;
}