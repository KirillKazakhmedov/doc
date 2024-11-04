#include "TaskQueue.hpp"

namespace core {

TaskQueue::TaskQueue(std::uint32_t max_queue_size)
  : _queue_size(0), _is_released(false), _max_queue_size(max_queue_size)
{
}

bool TaskQueue::push(const Task& task)
{
  if (_max_queue_size == 0 || _queue_size.load(std::memory_order_acquire) < _max_queue_size) {
    const std::lock_guard lock(_mutex);
    _task_queue.push(task);
    _queue_size.fetch_add(1, std::memory_order_release);
    _cv.notify_one();
    return true;
  } else {
    return false;
  }
}

Task TaskQueue::pop()
{
  std::unique_lock lock(_mutex);
  _cv.wait(lock, [this] {
    return _queue_size.load(std::memory_order_acquire) || _is_released.load(std::memory_order_acquire);
  });
  Task task;
  if (_queue_size.load(std::memory_order_acquire)) {
    task = std::move(_task_queue.top());
    _task_queue.pop();
    _queue_size.fetch_sub(1, std::memory_order_release);
  }
  return task;
}

void TaskQueue::release()
{
  _is_released.store(true, std::memory_order_release);
  _cv.notify_all();
}
void TaskQueue::acquire() { _is_released.store(false, std::memory_order_release); }

std::uint32_t TaskQueue::size() const { return _queue_size.load(std::memory_order_acquire); }
bool TaskQueue::empty() const { return _queue_size.load(std::memory_order_acquire) == 0; }

void TaskQueue::clear()
{
  const std::lock_guard lock(_mutex);
  decltype(_task_queue) empty;
  std::swap(_task_queue, empty);
  _queue_size.store(0, std::memory_order_release);
}
}  // namespace core
