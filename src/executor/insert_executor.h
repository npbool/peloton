/**
 * @brief Header file for insert executor.
 *
 * Copyright(c) 2015, CMU
 */

#pragma once

#include "executor/abstract_executor.h"

namespace nstore {
namespace executor {

class InsertExecutor : public AbstractExecutor {
  InsertExecutor(const InsertExecutor &) = delete;
  InsertExecutor& operator=(const InsertExecutor &) = delete;

 public:
  explicit InsertExecutor(planner::AbstractPlanNode *node,
                          storage::Tuple *tuple = nullptr);

 protected:
  bool DInit();

  bool DExecute();

 private:

  // tuple to be inserted
  storage::Tuple *tuple = nullptr;
};

} // namespace executor
} // namespace nstore