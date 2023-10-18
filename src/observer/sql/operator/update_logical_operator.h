#pragma once

#include "sql/operator/logical_operator.h"

/**
 * @brief 逻辑算子，用于执行update语句
 * @ingroup LogicalOperator
*/
class UpdateLogicalOperator : public LogicalOperator
{
public:
  UpdateLogicalOperator(Table *table, std::string attribute_name, Value value);
  virtual ~UpdateLogicalOperator() = default;

  LogicalOperatorType type() const override
  {
    return LogicalOperatorType::UPDATE;
  }
  Table *table() const { return table_; }
  std::string &attribute_name() { return attribute_name_; }
  const Value &value() const { return value_; }
  Value &value() { return value_; }

private:
  Table *table_ = nullptr;
  std::string attribute_name_;
  Value value_;
};