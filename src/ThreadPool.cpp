#include "ThreadPool.hpp"

namespace core {

ThreadPool::ThreadPool(std::uint32_t thread_count, std::uint32_t max_task_queue_size)
  : _tasks(TaskQueue(max_task_queue_size))
  , _thread_count(thread_count ? thread_count : 1)
  , _threads(new std::thread[_thread_count ? _thread_count : 1])
  , _tasks_total(0)
  , _paused(false)
  , _joined(false)
{
  create_threads();
  _running.store(true, std::memory_order_relaxed);
}

ThreadPool::~ThreadPool() { join_all(); }

std::uint32_t ThreadPool::get_queued_task_count() const { return _tasks.size(); }

std::uint32_t ThreadPool::get_running_task_count() const
{
  return _tasks_total.load(std::memory_order_acquire) - get_queued_task_count();
}

std::uint32_t ThreadPool::get_total_task_count() const { return _tasks_total; }

std::uint32_t ThreadPool::get_thread_count() const { return _thread_count; }

bool ThreadPool::push_task(const Task& task)
{
  if (!_joined.load(std::memory_order_acquire)) {
    _tasks_total.fetch_add(1, std::memory_order_release);
    return _tasks.push(task);
  } else {
    return false;
  }
}

void ThreadPool::reset(std::uint32_t thread_count)
{
  _thread_count = thread_count ? thread_count : 1;
  reset();
}

void ThreadPool::reset()
{
  interrupt();
  _threads.reset(new std::thread[_thread_count]);
}

void ThreadPool::pause() { _paused.store(true, std::memory_order_release); }

void ThreadPool::resume()
{
  _paused.store(false, std::memory_order_release);
  _pause_cv.notify_all();
}

void ThreadPool::interrupt()
{
  if (_running.load(std::memory_order_acquire)) {
    unblock();
    for (std::uint32_t i = 0; i < _thread_count; ++i) {
      _threads[i].join();
    }
  }
}

void ThreadPool::join_all()
{
  if (_running.load(std::memory_order_acquire)) {
    _joined.store(true, std::memory_order_release);
    {
      std::unique_lock lock(_finish_mutex);
      _finish_cv.wait(lock, [this] { return get_queued_task_count() == 0; });
    }
    unblock();
    for (std::uint32_t i = 0; i < _thread_count; ++i) {
      _threads[i].join();
    }
  }
}

void ThreadPool::unblock()
{
  _running.store(false, std::memory_order_release);
  if (_paused.load(std::memory_order_acquire)) {
    resume();
  }
  _tasks.release();
}

void ThreadPool::create_threads()
{
  for (std::uint32_t i = 0; i < _thread_count; i++) {
    _threads[i] = std::thread(&ThreadPool::run, this);
  }
}

void ThreadPool::run()
{
  while (_running) {
    if (_paused.load(std::memory_order_acquire)) {
      std::unique_lock lock(_pause_mutex);
      _pause_cv.wait(lock, [this] { return !_paused.load(std::memory_order_acquire); });
    }
    if (_joined.load(std::memory_order_acquire) && get_queued_task_count() == 0) {
      _finish_cv.notify_one();
    }
    auto task = _tasks.pop();
    if (!task.empty()) {
      task();
      _tasks_total.fetch_sub(1, std::memory_order_release);
    }
  }
}
}  // namespace core
