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
// Created by 16386 on 19/10/2023.
//

#pragma once

#include <string>
#include <vector>

#include "sql/stmt/stmt.h"

class Db;

#ifndef MINIDB_DROP_TABLE_STMT_H
#define MINIDB_DROP_TABLE_STMT_H

#endif  // MINIDB_DROP_TABLE_STMT_H

/**
 * @brief 表示创建表的语句
 * @ingroup Statement
 * @details 参考了create table写的
 */
class DropTableStmt : public Stmt
{
public:
  DropTableStmt(const std::string &table_name)
    : table_name_(table_name)
  {}
  virtual ~DropTableStmt() = default;

  StmtType type() const override { return  StmtType::DROP_TABLE; }

  const std::string &table_name() const {return table_name_; }

  static RC create(Db *db, const DropTableSqlNode &drop_table, Stmt *&stmt);

private:
  std::string table_name_;
};


