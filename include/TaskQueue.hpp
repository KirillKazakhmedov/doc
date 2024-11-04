#pragma once

#include "Task.hpp"

#include <mutex>
#include <queue>
#include <condition_variable>

namespace core {

/**
 * @brief This class represent Task safe-queue implementation.
 *
 */
class TaskQueue {
public:
  /**
   * @brief Construct a new Task Queue object. If you would like skip max queue size
   * factor for task pushing, set 0.
   * @param[in] max_queue_size Max queue size. If current queue size equal max size,
   * then n + 1 task cannot be enqueued.
   */
  TaskQueue(std::uint32_t max_queue_size = 0);

  /**
   * @brief Enqueue task to task queue.
   * @param[in] task
   * @return true If enqueuing was successful.
   * @return false Otherwise.
   */
  bool push(const Task& task);

  /**
   * @brief Extract next task for executing.
   * @return Task Extracting task.
   */
  Task pop();

  /**
   * @brief Executable thread does not wait while a next task won't be inserted.
   */
  void release();

  /**
   * @brief Executable thread wait while a next task won't be inserted.
   */
  void acquire();

  /**
   * @brief Return current queue size.
   * @return std::uint32_t Queue size.
   */
  std::uint32_t size() const;

  /**
   * @brief Check queue equals 0.
   * @return true If size equal 0.
   * @return false Otherwise.
   */
  bool empty() const;

  /**
   * @brief Clear current task queue.
   */
  void clear();

private:
  /**
   * @brief Presents thread barrier until the queue is empty or the is_released flag is set.
   */
  std::condition_variable _cv;
  /**
   * @brief Mutex for condition variable.
   */
  mutable std::mutex _mutex;
  /**
   * @brief Represents the priority queue for incoming tasks.
   */
  std::priority_queue<Task, std::vector<Task>, TaskComparator> _task_queue;
  /**
   * @brief Represents current queue size.
   */
  std::atomic_uint _queue_size;
  /**
   * @brief Flag to reset waiting for incoming tasks.
   * Used to wake up sleeping threads, usually to finish their work.
   */
  std::atomic_bool _is_released;
  /**
   * @brief Represent max queue size for incoming task.
   * If current queue size equal max queue size, task pushing is ignored.
   */
  const std::uint32_t _max_queue_size;
};
}  // namespace core
