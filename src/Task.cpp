#include "Task.hpp"

namespace core {

Task::Task(TaskPriority priority) : _priority(priority), _curr_thread_id(std::this_thread::get_id()) {}

bool Task::empty() const { return _func == nullptr; }
}  // namespace evo::foundation
