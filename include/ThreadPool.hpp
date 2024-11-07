#pragma once

#include "TaskQueue.hpp"

#include <atomic>       // std::atomic
#include <chrono>       // std::chrono
#include <cstdint>      // std::int_fast64_t, std::uint_fast32_t
#include <functional>   // std::function
#include <future>       // std::future, std::promise
#include <iostream>     // std::cout, std::ostream
#include <memory>       // std::shared_ptr, std::unique_ptr
#include <mutex>        // std::mutex, std::scoped_lock
#include <queue>        // std::queue
#include <thread>       // std::this_thread, std::thread
#include <type_traits>  // std::common_type_t, std::decay_t, std::enable_if_t, std::is_void_v, std::invoke_result_t
#include <utility>      // std::move

namespace core {

class ThreadPool {
public:
  using SharedPtr = std::shared_ptr<ThreadPool>;
  using UniquePtr = std::unique_ptr<ThreadPool>;
  /**
   * @brief Construct a new thread pool.
   *
   * @param thread_count The number of threads to use. The default value is the total number of hardware threads
   * available, as reported by the implementation. With a hyperthreaded CPU, this will be twice the number of CPU cores.
   * If the argument is zero, the default value will be used instead.
   * @param max_task_queue_size The maximum number of tasks in the queue.
   *  The task is added if the queue is not full otherwise false is returned
   */
  ThreadPool(std::uint32_t thread_count, std::uint32_t max_task_queue_size);

  /**
   * @brief Destruct the thread pool. Waits for all tasks to complete, then destroys all threads. Note that if the
   * variable paused is set to true, then any tasks still in the queue will never be executed.
   */
  ~ThreadPool();

  /**
   * @brief Get the number of tasks currently waiting in the queue to be executed by the threads.
   *
   * @return The number of queued tasks.
   */
  std::uint32_t get_queued_task_count() const;

  /**
   * @brief Get the number of tasks currently being executed by the threads.
   *
   * @return The number of running tasks.
   */
  std::uint32_t get_running_task_count() const;

  /**
   * @brief Get the total number of unfinished tasks - either still in the queue, or running in a thread.
   *
   * @return The total number of tasks.
   */
  std::uint32_t get_total_task_count() const;

  /**
   * @brief Get the number of threads in the pool.
   *
   * @return The number of threads.
   */
  std::uint32_t get_thread_count() const;

  /**
   * @brief Push a function with no arguments or return value into the task queue.
   *
   * @tparam Task packed callable task.
   * @param task The function to push.
   * @return bool Return true if push finished successfully,
   * false otherwise(current queue size more or equal task queue capacity)
   */
  bool push_task(const Task& task);

  /**
   * @brief Interrupts execution of all threads
   * and recreates the pool with the given number of threads as an input argument.
   * @param thread_count Thread count.
   */
  void reset(std::uint32_t thread_count);

  /**
   * @brief Interrupts execution of all threads and recreates the pool.
   */
  void reset();

  /**
   * @brief Pauses the execution of all threads using a condition variable.
   */
  void pause();

  /**
   * @brief Resumes execution of all threads stopped by the pause() command.
   * Thread wakeup notification is implemented using a condition variableю
   */
  void resume();

  /**
   * @brief Wait for tasks to be completed.
   */
  void join_all();

  /**
   * @brief Interrupt task execution for all threads.
   * To wait for a specific task, call the wait()|get() member function of the generated future.
   */
  void interrupt();

private:
  /**
   * @brief Make task queue unblocking for all threads
   *
   */
  void unblock();

  /**
   * @brief Create the threads in the pool and assign a worker to each thread.
   */
  void create_threads();

  /**
   * @brief A worker function to be assigned to each thread in the pool.
   * Continuously pops tasks out of the queue and executes them,
   * as long as the atomic variable running is set to true.
   */
  void run();

  /**
   * @brief A queue of tasks to be executed by the threads.
   */
  TaskQueue _tasks;

  /**
   * @brief The number of threads in the pool.
   */
  std::uint32_t _thread_count;

  /**
   * @brief A smart pointer to manage the memory allocated for the threads.
   */
  std::unique_ptr<std::thread[]> _threads;

  /**
   * @brief Condition variable for pausing thread pool
   *
   */
  std::condition_variable _pause_cv;

  /**
   * @brief Condition variable for finish thread pool execution
   *
   */
  std::condition_variable _finish_cv;

  /**
   * @brief Mutex to make pausing thread pool executing
   *
   */
  std::mutex _pause_mutex;

  /**
   * @brief Mutex to make pausing thread pool executing
   *
   */
  std::mutex _finish_mutex;

  /**
   * @brief An atomic variable to keep track of the total number of unfinished tasks -
   * either still in the queue, or running in a thread.
   */
  std::atomic_uint _tasks_total;

  /**
   * @brief An atomic variable indicating to the workers to pause.
   * When set to true, the workers temporarily stop execution tasks,
   * although any tasks already executed will keep running until they are done.
   * Call resume() again to resume task execution.
   */
  std::atomic_bool _paused;

  /**
   * @brief An atomic variable indicating to the workers to finish working.
   * When set to true, the workers permanently stop execution tasks,
   * although any tasks already executed will keep running until they are done.
   */
  std::atomic_bool _joined;

  /**
   * @brief An atomic variable indicating to the workers to keep running.
   * When set to false, the workers permanently stop working.
   */
  std::atomic_bool _running;
};
}  // namespace core
