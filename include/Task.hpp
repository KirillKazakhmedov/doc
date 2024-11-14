#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <future>
#include <thread>
#include <type_traits>

namespace core {

/**
 * @brief Declaration struct for member function returning type.
 * @tparam T Member function type.
 */
template <typename T>
struct member_function_return_type;

/**
 * @brief Definition struct for member function returning type.
 * @tparam ObjectT The class contains a function for which it is necessary to determine the return type.
 * @tparam ReturnT Returning type.
 * @tparam Args Arguments passed to member function.
 */
template <typename ObjectT, typename ReturnT, typename... Args>
struct member_function_return_type<ReturnT (ObjectT::*)(Args...)> {
  using type = ReturnT;
};

template <typename FuncT>
using member_function_return_type_t = typename member_function_return_type<FuncT>::type;

/**
 * @brief Enum class for task ordering by priority.
 */
enum class TaskPriority : std::uint8_t { Lowest, Low, Medium, High, Highest };

/**
 * @brief This class is used as a wrapper over the passed functional objects.
 * Functional objects can be ordinary functions or class methods.
 */
class Task {
public:
  /**
   * @brief For access task priority field.
   */
  friend struct TaskComparator;

  /**
   * @brief Construct a new Task object. May pass task priority.
   * @param priority Priority task.
   */
  Task(TaskPriority priority = TaskPriority::Medium);

  /**
   * @brief Wraps a function with a variable number of arguments in std::function<void> object.
   * Return std::future<bool> if returning type has void. If function execution was failed result are
   * setted as std::current_exception().
   * @tparam F Function object.
   * @tparam Args Passed arguments.
   * @return std::future<bool> Will return via future.get() true, if execution was finished successful,
   * will throw an exception or return false otherwise.
   */
  template <typename F, typename... Args,
            typename = std::enable_if_t<std::is_void_v<std::invoke_result_t<std::decay_t<F>, std::decay_t<Args>...>>>>
  [[nodiscard]] std::future<bool> assign(F&& func, Args&&... args)
  {
    std::shared_ptr<std::promise<bool>> tsk_promise(new std::promise<bool>);
    auto future = tsk_promise->get_future();
    _func = [func = std::forward<F>(func), args = std::make_tuple(std::forward<Args>(args)...), tsk_promise] {
      try {
        std::apply(func, std::move(args));
        tsk_promise->set_value(true);
      } catch (...) {
        try {
          tsk_promise->set_exception(std::current_exception());
        } catch (...) {
          tsk_promise->set_value(false);
        }
      }
    };
    return future;
  }

  /**
   * @brief Wraps a function with a variable number of arguments in std::function<void> object.
   * Return std::future<R> if returning type has not void. If function execution was failed result are
   * setted as std::current_exception().
   * @tparam F Function object.
   * @tparam Args Passed arguments.
   * @tparam R Returning type.
   * @return std::future<R> Will return via future.get() R type, if execution was finished successful,
   * will throw an exception otherwise.
   */
  template <typename F, typename... Args, typename R = std::invoke_result_t<std::decay_t<F>, std::decay_t<Args>...>,
            typename = std::enable_if_t<!std::is_void_v<R>>>
  [[nodiscard]] std::future<R> assign(F&& func, Args&&... args)
  {
    std::shared_ptr<std::promise<R>> tsk_promise(new std::promise<R>);
    std::future<R> future = tsk_promise->get_future();
    _func = [func = std::forward<F>(func), args = std::make_tuple(std::forward<Args>(args)...), tsk_promise] {
      try {
        tsk_promise->set_value(std::apply(func, std::move(args)));
      } catch (...) {
        try {
          tsk_promise->set_exception(std::current_exception());
        } catch (...) {
        }
      }
    };
    return future;
  }

  /**
   * @brief Wraps a member function with a variable number of arguments in std::function<void> object.
   * Return std::future<bool> if returning type has void. If member function execution was failed result are
   * setted as std::current_exception().
   * @tparam T Class contains member function type as FuncT.
   * @tparam FuncT Member function type.
   * @tparam Args Passed arguments.
   * @return std::future<bool> Will return via future.get() true, if execution was finished successful,
   * will throw an exception or return false otherwise.
   */
  template <typename T, typename FuncT, typename... Args,
            typename = std::enable_if_t<std::is_void_v<std::decay_t<member_function_return_type_t<FuncT>>>>>
  [[nodiscard]] std::future<bool> assign(T pobj, FuncT pfunc, Args&&... args)
  {
    std::shared_ptr<std::promise<bool>> tsk_promise(new std::promise<bool>);
    std::future<bool> future = tsk_promise->get_future();
    _func = [pobj, pfunc, args = std::make_tuple(std::forward<Args>(args)...), tsk_promise] {
      try {
        std::apply(pfunc, std::tuple_cat(std::make_tuple(pobj), std::move(args)));
        tsk_promise->set_value(true);
      } catch (...) {
        try {
          tsk_promise->set_exception(std::current_exception());
        } catch (...) {
          tsk_promise->set_value(false);
        }
      }
    };
    return future;
  }

  /**
   * @brief Wraps a member function with a variable number of arguments in std::function<void> object.
   * Return std::future<R> if returning type has void. If member function execution was failed result are
   * setted as std::current_exception().
   * @tparam T Class contains member function type as FuncT.
   * @tparam FuncT Member function type.
   * @tparam Args Passed arguments.
   * @tparam R Returning type.
   * @return std::future<R> Will return via future.get() R, if execution was finished successful,
   * will throw an exception otherwise.
   */
  template <typename T, typename FuncT, typename... Args, typename R = member_function_return_type_t<FuncT>,
            typename = std::enable_if_t<!std::is_void_v<std::decay_t<R>>>>
  [[nodiscard]] std::future<R> assign(T pobj, FuncT pfunc, Args&&... args)
  {
    std::shared_ptr<std::promise<R>> tsk_promise(new std::promise<R>);
    std::future<R> future = tsk_promise->get_future();
    _func = [pobj, pfunc, args = std::make_tuple(std::forward<Args>(args)...), tsk_promise] {
      try {
        tsk_promise->set_value(std::apply(pfunc, std::tuple_cat(std::make_tuple(pobj), std::move(args))));
      } catch (...) {
        try {
          tsk_promise->set_exception(std::current_exception());
        } catch (...) {
        }
      }
    };
    return future;
  }

  /**
   * @brief Checks for an empty function object.
   * @return true If std::function<void> is empty.
   * @return false Otherwise.
   */
  bool empty() const;

  /**
   * @brief Execution operator for current functional object.
   * If the task is launched in the thread in which it was created, then a deadlock will occur.
   * In this case, it is moved to an asynchronous call, otherwise it is executed as is.
   */
  void operator()() const
  {
    if (_curr_thread_id == std::this_thread::get_id()) {
      auto f = std::async(_func);
      f.wait();
    } else {
      _func();
    }
  }

private:
  /**
   * @brief Task priority.
   */
  TaskPriority _priority;
  /**
   * @brief Thread id in which the task was created.
   */
  std::thread::id _curr_thread_id;
  /**
   * @brief Functional object for execution.
   */
  std::function<void()> _func;
};

struct TaskComparator {
  bool operator()(const Task& t1, const Task& t2) const { return t1._priority > t2._priority; }
};
}  // namespace core
