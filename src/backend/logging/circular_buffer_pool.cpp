/*-------------------------------------------------------------------------
 *
 * circular_buffer_pool.cpp
 * file description
 *
 * Copyright(c) 2015, CMU
 *
 * /peloton/src/backend/logging/circular_buffer_pool.cpp
 *
 *-------------------------------------------------------------------------
 */

#include <atomic>
#include <cassert>
#include <xmmintrin.h>

#include "backend/logging/circular_buffer_pool.h"

namespace peloton {
namespace logging {

//===--------------------------------------------------------------------===//
// Circular Buffer Pool
//===--------------------------------------------------------------------===//
CircularBufferPool::CircularBufferPool()
    : head_(ATOMIC_VAR_INIT(0)), tail_(ATOMIC_VAR_INIT(0)) {
  memset(buffers_, 0, BUFFER_POOL_SIZE * sizeof(std::shared_ptr<LogBuffer>));
}

CircularBufferPool::~CircularBufferPool() {}

bool CircularBufferPool::Put(std::shared_ptr<LogBuffer> buffer) {
  // assume enough place for put..
  unsigned int current_idx = head_.fetch_add(1) & (BUFFER_POOL_SIZE - 1);
  // LOG_INFO("CircularBufferPool::Put - current_idx: %u", current_idx);
  buffers_[current_idx] = buffer;
  return true;
}

std::shared_ptr<LogBuffer> CircularBufferPool::Get() {
  unsigned int current_idx = tail_.fetch_add(1) & (BUFFER_POOL_SIZE - 1);
  while (true) {
    if (buffers_[current_idx]) {
      break;
    } else {
      // pause for a minimum amount of time
      _mm_pause();
    }
  }
  // LOG_INFO("CircularBufferPool::Get - current_idx: %u", current_idx);
  std::shared_ptr<LogBuffer> buff = buffers_[current_idx];
  memset(buffers_ + current_idx, 0, sizeof(std::shared_ptr<LogBuffer>));
  return buff;
}

}  // namespace logging
}  // namespace peloton
