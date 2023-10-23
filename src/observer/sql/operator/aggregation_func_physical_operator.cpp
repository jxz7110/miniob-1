//
// Created by jxz110 on 2023/10/19.
//

#include "aggregation_func_physical_operator.h"
/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by WangYunlai on 2022/6/27.
//

#include "common/log/log.h"
#include "sql/operator/predicate_physical_operator.h"
#include "storage/record/record.h"
#include "sql/stmt/filter_stmt.h"
#include "storage/field/field.h"

/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by WangYunlai on 2022/07/01.
//

#include "common/log/log.h"
#include "sql/operator/aggregation_func_physical_operator.h"
#include "storage/table/table.h"

AggregationPhysicalOperator::AggregationPhysicalOperator(std::vector<AggregateType> agg_types , std::vector<std::string> agg_names)
    : agg_types_(agg_types), agg_names_(agg_names)
{
}

RC AggregationPhysicalOperator::open(Trx *trx)
{
  if (children_.empty()) {
    return RC::SUCCESS;
  }

  PhysicalOperator *child = children_[0].get();
  RC rc = child->open(trx);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to open child operator: %s", strrc(rc));
    return rc;
  }
  //这个位置里面的project计算完了，这里需要计算想聚合函数部分的代码
  int count=0;
  int cell_num;
  std::vector<Value> res;
  if(RC::SUCCESS == (rc = child->next()))
  {
    Tuple *tuple=child->current_tuple();
    cell_num=tuple->cell_num();
    for(int i = 0;i<cell_num;i++){
      Value value;
      rc = tuple->cell_at(i,value);
      if(rc != RC::SUCCESS){
        return rc;
      }
      switch(agg_types_[i]){
        case AggregateType::MAX:{
          res.push_back(value);
        }break;
        case AggregateType::MIN:{
          res.push_back(value);
        }break;
        case AggregateType::COUNT:{
          Value temp;
          temp.set_int(1);
          res.push_back(temp);
        }break;
        case AggregateType::SUM:{
          res.push_back(value);
        }break;
        case AggregateType::AVG:{
          res.push_back(value);;
          count++;
        }break;
      }
    }
  }
  while(RC::SUCCESS == (rc = child->next()))
  {
    Tuple *tuple=child->current_tuple();
    cell_num=tuple->cell_num();
    for(int i = 0;i<cell_num;i++){
      Value value;
      rc = tuple->cell_at(i,value);
      if(rc != RC::SUCCESS){
        return rc;
      }
      switch(agg_types_[i]){
        case AggregateType::MAX:{
          if(res[i].compare((value)) < 0){
            res[i]=value;
          }
        }break;
        case AggregateType::MIN:{
          if(res[i].compare((value)) > 0){
            res[i]=value;
          }
        }break;
        case AggregateType::COUNT:{
          res[i].my_add(1);
        }break;
        case AggregateType::SUM:{
          res[i].my_add(value);
        }break;
        case AggregateType::AVG:{
          res[i].my_add(value);
          count++;
        }break;
      }
    }
  }
  for(int i=0;i<cell_num;i++){
    if(agg_types_[i]==AggregateType::AVG){
      res[i].my_div(count);
    }
  }
  tuple_.set_cells(res);
  return RC::SUCCESS;
}

RC AggregationPhysicalOperator::next()
{
  if(!is_visit){
    is_visit=true;
    return RC::SUCCESS;
  }else{
    return RC::RECORD_EOF;
  }
}

RC AggregationPhysicalOperator::close()
{
  if (!children_.empty()) {
    children_[0]->close();
  }
  return RC::SUCCESS;
}
Tuple *AggregationPhysicalOperator::current_tuple()
{
  return &tuple_;
}

RC AggregationPhysicalOperator::set_schema(TupleSchema &schema)
{
  for(int i=0; i<agg_names_.size(); i++){
    switch(agg_types_[i]){
      case AggregateType::MAX:{
        std::string t="MAX(" + agg_names_[i] +")";
        schema.append_cell(t.c_str());
      }break;
      case AggregateType::MIN:{
        std::string t="MIN(" + agg_names_[i] +")";
        schema.append_cell(t.c_str());
      }break;
      case AggregateType::COUNT:{
        std::string t="COUNT(" + agg_names_[i] +")";
        schema.append_cell(t.c_str());
      }break;
      case AggregateType::SUM:{
        std::string t="SUM(" + agg_names_[i] +")";
        schema.append_cell(t.c_str());
      }break;
      case AggregateType::AVG:{
        std::string t="AVG(" + agg_names_[i] +")";
        schema.append_cell(t.c_str());
      }break;
    }
  }
  return RC::SUCCESS;
}
