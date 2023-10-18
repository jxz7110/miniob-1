#include "sql/operator/update_logical_operator.h"

UpdateLogicalOperator::UpdateLogicalOperator(Table *table, std::string attribute_name, Value value)
    : table_(table), attribute_name_(attribute_name), value_(value)
{}