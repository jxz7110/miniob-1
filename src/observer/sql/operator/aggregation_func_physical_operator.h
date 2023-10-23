//
// Created by jxz110 on 2023/10/19.
//

/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */


#pragma once

#include <memory>
#include "sql/operator/physical_operator.h"
#include "sql/expr/tuple.h"

class FilterStmt;

enum class AggregateType{
  MAX,
  MIN,
  COUNT,
  AVG,
  SUM,
  UNKNOW,
};

/**
 * @brief 聚合函数物理算子
 * @ingroup PhysicalOperator
 */
class AggregationPhysicalOperator : public PhysicalOperator
{
public:
  AggregationPhysicalOperator(std::vector<AggregateType> agg_types , std::vector<std::string> agg_names);

  virtual ~AggregationPhysicalOperator() = default;

  PhysicalOperatorType type() const override
  {
    return PhysicalOperatorType::AGGREGATE;
  }

  RC open(Trx *trx) override;
  RC next() override;
  RC close() override;
  Tuple *current_tuple() override;
  RC set_schema(TupleSchema & schema);

private:
  AggregateTuple tuple_;
  std::vector<AggregateType> agg_types_;
  std::vector<std::string> agg_names_;
  bool is_visit=false;
};
