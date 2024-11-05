#pragma once

#include "ThreadPool.hpp"

namespace core {

/**
  An abstract class for using a thread pool
 */
class ThreadPoolExecutable {
public:
  /**
   * @brief Set the thread count for execution via thread pool
   * @param thread_count 
   */
  void set_thread_count(std::uint32_t thread_count)
  {
    thread_pool_.reset(thread_count);
  }

protected:
  ThreadPool thread_pool_;
};
}  // namespace core
