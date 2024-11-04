#pragma once

#include "EventHandlerImpl.hpp"
#include <memory>

namespace core {

/**
 * @brief This class contains static methods for creating EventHandlerImpl pointer
 * associated with corresponding event handler. Event handler may be presented as function
 * and class methods.
 */
class EventHandler {
public:
  /**
   * @brief Default ctor was deleted. Use static methods only.
   */
  EventHandler() = delete;

  /**
   * @brief This method creates event handler pointer corresponding
   * to function acquired const void* type to sender and template argument.
   * @tparam T Template argument type .
   * @param pFunction[in] Function(event handler) pointer.
   * @return EventHandlerImplPtr<T> Return unique pointer to event handler base.
   */
  template <typename T>
  static EventHandlerImplPtr<T> bind(void (*pFunction)(const void*, T))
  {
    return std::make_unique<EventHandlerImplForNonMemberFunction<T>>(pFunction);
  }

  /**
   * @brief This method creates event handler pointer corresponding
   * to class method acquired const void* type to sender and template argument.
   * @tparam T Template argument type.
   * @param pMemberFunction[in] Member function(event handler) pointer.
   * @return EventHandlerImplPtr<T> Return unique pointer to event handler base.
   */
  template <typename U, typename T>
  static EventHandlerImplPtr<T> bind(U* thisPtr, void (U::*pMemberFunction)(const void*, T))
  {
    return std::make_unique<EventHandlerImplForMemberFunction<U, T>>(thisPtr, pMemberFunction);
  }

  /**
   * @brief This method creates event handler pointer corresponding
   * to function acquired const void* type to sender.
   * @param pFunction[in] Function(event handler) pointer.
   * @return EventHandlerImplPtr<T> Return unique pointer to event handler base.
   */
  static EventHandlerImplPtr<void> bind(void (*pFunction)(const void*))
  {
    return std::make_unique<EventHandlerImplForNonMemberFunction<void>>(pFunction);
  }

  /**
   * @brief This method creates event handler pointer corresponding
   * to class method acquired const void* type to sender.
   * @param pMemberFunction[in] Member function(event handler) pointer.
   * @return EventHandlerImplPtr<T> Return unique pointer to event handler base.
   */
  template <typename U>
  static EventHandlerImplPtr<void> bind(U* thisPtr, void (U::*pMemberFunction)(const void*))
  {
    return std::make_unique<EventHandlerImplForMemberFunction<U, void>>(thisPtr, pMemberFunction);
  }
};
}  // namespace core
