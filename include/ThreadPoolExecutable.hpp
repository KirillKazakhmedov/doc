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
   * @param max_task_queue_size
   */
  void init_thread_pool(std::uint32_t thread_count, std::uint32_t max_task_queue_size)
  {
    thread_pool_.reset(new ThreadPool(thread_count, max_task_queue_size));
  }

protected:
  /**
   * @brief Default ctor ThreadPoolExecutable class.
   * Create new object of ThreadPoolExecutable type.
   */
  ThreadPoolExecutable() = default;

  /**
   * @brief Default dtor ThreadPoolExecutable class.
   * Destroy ThreadPoolExecutable instance.
   */
  virtual ~ThreadPoolExecutable() = default;

protected:
  ThreadPool::UniquePtr thread_pool_;
};
}  // namespace core
