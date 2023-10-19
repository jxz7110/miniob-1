//
// Created by 16386 on 19/10/2023.
//


#pragma once

#include "common/rc.h"

class SQLStageEvent;


/**
 * @brief 删除表的执行器
 * @ingroup Executor
 */
class DropTableExecutor
{
public:
  DropTableExecutor() = default;
  virtual ~DropTableExecutor() = default;

  RC execute(SQLStageEvent *sql_event);
};