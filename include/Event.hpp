#pragma once

#include "EventBase.hpp"

namespace core {

using EventHandlerAsyncResult = std::future<bool>;

/**
 * @brief This class implement event object. It provides
 * notification process for all observers/sunscribers to the event.
 * Notification may be occured in sync and async modes.
 * @tparam T Template parameter contains argument for observers/sunscribers.
 */
template <typename T>
class Event : public EventBase<T> {
  using EventBase<T>::mutex_;
  using EventBase<T>::handlers_;
  using EventBase<T>::thread_pool_;

public:
  /**
   * @brief This function provides sync notification. Notification
   * are thread safe process.
   * @param psender[in] Event sender.
   * @param arg[in] Argument sender for observers/subscribers.
   */
  void notify(const void* psender, const T& arg)
  {
    std::shared_lock lock(mutex_);
    for (const auto& pHandler : handlers_) {
      if (pHandler)
        pHandler->OnEvent(psender, arg);
    }
  }
  /**
   * @brief This function provides async notification. Notification are provided
   * via thread pool. This process are thread safe.
   * @param psender[in] Event sender.
   * @param arg[in] Argument sender for observers/subscribers.
   * @return std::vector<EventHandlerAsyncResult> Return execution result for every handler for the event.
   * operation status.
   */
  std::vector<EventHandlerAsyncResult> notify_async(const void* psender, const T& arg)
  {
    if(!thread_pool_) {
      throw std::domain_error("Thread pool was not setted for async notification!");
    }

    std::vector<EventHandlerAsyncResult> results;
    results.reserve(handlers_.size());
    std::shared_lock lock(mutex_);
    for (const auto& pHandler : handlers_) {
      if (pHandler) {
            Task task;
            auto result = task.assign(pHandler.get(), &EventHandlerImpl<T>::OnEvent, psender, arg);
            thread_pool_->push_task(task);
            results.push_back(std::move(result));
      }
    }
    return results;
  }
};

/**
 * @brief This class implement event object. It provides
 * notification process for all observers/sunscribers to the event.
 * Notification may be occured in sync and async modes.
 */
template <>
class Event<void> : public EventBase<void> {
public:
  /**
   * @brief This function provides sync notification. Notification
   * are thread safe process.
   * @param psender[in] Event sender.
   */
  void notify(const void* psender)
  {
    std::shared_lock lock(mutex_);
    for (const auto& pHandler : handlers_) {
      if (pHandler)
        pHandler->OnEvent(psender);
    }
  }

  /**
   * @brief This function provides sync notification. Notification
   * are thread safe process.
   * @param psender[in] Event sender.
   * @return std::vector<EventHandlerAsyncResult> Return execution result for every handler for the event.
   */
  std::vector<EventHandlerAsyncResult> notify_async(const void* psender)
  {
    if(!thread_pool_) {
      throw std::domain_error("Thread pool was not setted for async notification!");
    }

    std::vector<EventHandlerAsyncResult> results;
    results.reserve(handlers_.size());
    std::shared_lock lock(mutex_);
    for (const auto& pHandler : handlers_) {
      if (pHandler) {
        Task task;
        auto result = task.assign(pHandler.get(), &EventHandlerImpl<void>::OnEvent, psender);
        thread_pool_->push_task(task);
        results.emplace_back(std::move(result));
      }
    }
    return results;
  }
};
}  // namespace core
