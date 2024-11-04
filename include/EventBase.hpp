#pragma once

#include "EventHandlerImpl.hpp"

#include <vector>
#include <shared_mutex>
#include <type_traits>

namespace core {
/**
 * @brief Event base class, propagating event model for
 * object communication between your components in the program.
 * Add/remove forwarded event handler from observer array.
 * Adding/removing operations are thread safe.
 * @tparam T
 */
template <typename T>
class EventBase {
public:
  /**
   * @brief Copy ctor.
   * This constructor was deleted.
   */
  EventBase(const EventBase&) = delete;

  /**
   * @brief Copy assignment operator.
   * This opetator was deleted.
   * @return EventBase&
   */
  EventBase& operator=(const EventBase&) = delete;

  /**
   * @brief This operator add event handler instance to observer vector.
   * @param[in] pHandler Event handler for current event.
   */
  EventBase<T>& operator+=(EventHandlerImplPtr<T> pHandlerToAdd)
  {
    if (pHandlerToAdd) {
      std::unique_lock lock(mutex_);
      auto it = std::find_if(std::begin(handlers_), std::end(handlers_), [&pHandlerToAdd](auto& pHandler) {
        return pHandlerToAdd->IsBindedToSameFunctionAs(pHandler.get());
      });
      if (it == std::end(handlers_)) {
        handlers_.push_back(std::move(pHandlerToAdd));
      }
    }
    return *this;
  }

  /**
   * @brief This operator remove event handler instance from observer vector.
   * @param pHandlerToRemove[in] Removable event handler
   */
  EventBase<T>& operator-=(EventHandlerImplPtr<T> pHandlerToRemove)
  {
    if (!pHandlerToRemove) {
      return *this;
    }
    {
      std::unique_lock lock(mutex_);
      auto it = std::remove_if(std::begin(handlers_), std::end(handlers_), [&pHandlerToRemove](auto& pHandler) {
        return pHandlerToRemove->IsBindedToSameFunctionAs(pHandler.get());
      });
      if (it != std::end(handlers_)) {
        handlers_.erase(it);
      }
    }
    return *this;
  }

protected:
  /**
   * @brief Default ctor EventBase class.
   * Create new object of EventBase type.
   */
  EventBase() = default;

  /**
   * @brief Default dtor EventBase class.
   * Destroy EventBase instance.
   */
  virtual ~EventBase() = default;

protected:
  std::vector<EventHandlerImplPtr<T> > handlers_;
  std::shared_mutex mutex_;
};
}  // namespace core
