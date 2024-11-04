#pragma once

#include "EventBase.hpp"

namespace core {

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
        pHandler->on_event(psender, arg);
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
    std::vector<EventHandlerAsyncResult> results;
    results.reserve(handlers_.size());
    std::shared_lock lock(mutex_);
    for (const auto& pHandler : handlers_) {
      if (pHandler)
        results.emplace_back(pHandler->on_event_async(psender, arg));
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
  using EventBase<void>::mutex_;
  using EventBase<void>::handlers_;

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
        pHandler->on_event(psender);
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
    std::vector<EventHandlerAsyncResult> results;
    results.reserve(handlers_.size());
    std::shared_lock lock(mutex_);
    for (const auto& pHandler : handlers_) {
      if (pHandler)
        results.emplace_back(pHandler->on_event_async(psender));
    }
    return results;
  }
};
}  // namespace core
