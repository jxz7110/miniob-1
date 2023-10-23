//
// Created by jxz110 on 2023/10/19.
//

#include "sql/operator/aggregation_func_logical_operator.h"

AggregationLogicalOperator :: AggregationLogicalOperator(const std::vector<std::string> &agg_type, const std::vector<std::string> &agg_name) : agg_type_(agg_type), agg_name_(agg_name)
{}