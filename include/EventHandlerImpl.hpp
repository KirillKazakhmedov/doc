#pragma once

#include "EventHandlerImplBase.hpp"

#include <utility>
#include <queue>
#include <iostream>
#include <memory>

namespace core {

template <typename T>
class EventHandlerImpl;

template <typename T>
using EventHandlerImplPtr = std::unique_ptr<EventHandlerImpl<T>>;

/**
 * @brief Interface class for implementing subscriber notification methods.
 * Notification can be done synchronously(on_event) and asynchronously(on_event_async).
 * @tparam T Passed argument type.
 */
template <typename T>
class EventHandlerImpl : public EventHandlerImplBase<T> {
public:
  /**
   * @brief Implement this method to notify subscribers synchronously.
   * The method takes a pointer to the sender and a passed argument.
   * @param[in] psender Pointer to the sender.
   * @param[in] arg Passed argument.
   */
  virtual void on_event(const void* psender, const T& arg) = 0;
  /**
   * @brief Implement this method to notify subscribers asynchronously.
   * The method takes a pointer to the sender and a passed argument.
   * It is recommended to use thread pool for asynchronous notification.
   * @param[in] psender Pointer to the sender.
   * @param[in] arg Passed argument.
   * @return AsyncResult Return std::future<bool> as execution result to wait or get operation status.
   */
  //virtual EventHandlerAsyncResult on_event_async(const void* psender, const T& arg) = 0;

// protected:
//   EventHandlerImpl() : thread_pool_(ThreadPool(2, 0)) {}
//   ThreadPool thread_pool_;
};

/**
 * @brief Interface class for implementing subscriber notification methods.
 * Notification can be done synchronously(on_event) and asynchronously(on_event_async).
 */
template <>
class EventHandlerImpl<void> : public EventHandlerImplBase<void> {
public:
  /**
   * @brief Implement this method to notify subscribers synchronously.
   * The method takes a pointer to the sender.
   * @param[in] psender Pointer to the sender.
   */
  virtual void on_event(const void* psender) = 0;
  /**
   * @brief Implement this method to notify subscribers asynchronously.
   * The method takes a pointer to the sender.
   * It is recommended to use thread pool for asynchronous notification.
   * @param[in] psender Pointer to the sender.
   * @return EventHandlerAsyncResult Return std::future<bool> as execution result to wait or get operation status.
   */
  //virtual EventHandlerAsyncResult on_event_async(const void* psender) = 0;

// protected:
//   EventHandlerImpl() : thread_pool_(ThreadPool(2, 0)) {}
//   ThreadPool thread_pool_;
};

/**
 * @brief Event handler for a function that takes a pointer
 * to the sender object and an argument of type T.
 * @tparam T Argument type.
 */
template <typename T>
class EventHandlerImplForNonMemberFunction : public EventHandlerImpl<T> {
public:
  /**
   * @brief Construct a new EventHandlerImplForNonMemberFunction object.
   * @param[in] pFunction Function pointer that takes a pointer
   * to the sender object and an argument of type T
   */
  EventHandlerImplForNonMemberFunction(void (*pFunction)(const void*, T)) : pFunction_(pFunction) {}

  /**
   * @brief Call handler via pointer to function.
   * The method takes a pointer to the sender and a passed argument.
   * It is recommended to use thread pool for asynchronous notification.
   * @param[in] psender Pointer to the sender.
   * @param[in] arg Passed argument.
   * @return EventHandlerAsyncResult Return std::future<bool> as execution result to wait or get operation status.
   */
  // virtual EventHandlerAsyncResult on_event_async(const void* psender, const T& arg) override final
  // {
  //   Task task;
  //   auto result = task.assign(pFunction_, psender, arg);
  //   this->thread_pool_.push_task(task);
  //   return result;
  // }

  /**
   * @brief Call handler via pointer to function.
   * The method takes a pointer to the sender and a passed argument.
   * @param[in] psender Pointer to the sender.
   * @param[in] arg Passed argument.
   */
  virtual void on_event(const void* psender, const T& arg) override final { pFunction_(psender, arg); }

  /**
   * @brief Сhecks the current and passed event handler.
   * Checks the type of the passed event handler with the current one.
   * Performs function check pointer handlers.
   * @param[in] pHandler2 Pointer to the event handler.
   * @return true If handlers are same type and have same function pointer.
   * @return false Otherwise
   */
  virtual bool IsBindedToSameFunctionAs(const EventHandlerImplBase<T>* pHandler2) const override final
  {
    if (!EventHandlerImplBase<T>::IsSametype(pHandler2)) {
      return false;
    }
    const auto pHandlerCasted = dynamic_cast<const EventHandlerImplForNonMemberFunction<T>*>(pHandler2);
    if (!pHandlerCasted) {
      return false;
    }
    return this->pFunction_ == pHandlerCasted->pFunction_;
  }

private:
  /**
   * @brief Pointer to a function for event handling.
   */
  void (*pFunction_)(const void*, T);
};

/**
 * @brief Event handler for a class method that takes a pointer
 * to the sender object and an argument of type T.
 * @tparam U Class name.
 * @tparam T Argument type.
 */
template <typename U, typename T>
class EventHandlerImplForMemberFunction : public EventHandlerImpl<T> {
public:
  /**
   * @brief Construct a new EventHandlerImplForMemberFunction object.
   * @param[in] thisPtr Object pointer, that initiate method call via pointer.
   * @param[in] pMemberFunction Function pointer that takes a pointer
   * to the sender object and an argument of type T.
   */
  EventHandlerImplForMemberFunction(U* thisPtr, void (U::*pMemberFunction)(const void*, T))
    : pCaller_(thisPtr), pMemberFunction_(pMemberFunction)
  {
  }

  /**
   * @brief Call handler via pointer to object and pointer to class method.
   * The method takes a pointer to the sender and a passed argument.
   * It is recommended to use thread pool for asynchronous notification.
   * @param[in] psender Pointer to the sender.
   * @param[in] arg Passed argument.
   * @return EventHandlerAsyncResult Return std::future<bool> as execution result to wait or get operation status.
   */
  // virtual EventHandlerAsyncResult on_event_async(const void* psender, const T& arg) override final
  // {
  //   Task task;
  //   auto result = task.assign(pCaller_, pMemberFunction_, psender, arg);
  //   this->thread_pool_.push_task(task);
  //   return result;
  // }

  /**
   * @brief Call handler via pointer to object and pointer to class method.
   * The method takes a pointer to the sender and a passed argument.
   * @param[in] psender Pointer to the sender.
   * @param[in] arg Passed argument.
   */
  virtual void on_event(const void* psender, const T& arg) override final
  {
    (pCaller_->*(pMemberFunction_))(psender, arg);
  }

  /**
   * @brief Сhecks the current and passed event handler.
   * Checks the type of the passed event handler with the current one.
   * Checks pointers to objects and member functions.
   * @param[in] pHandler2 Pointer to the event handler.
   * @return true If handlers are same type and have same object and method pointers.
   * @return false Otherwise
   */
  virtual bool IsBindedToSameFunctionAs(const EventHandlerImplBase<T>* pHandler2) const override final
  {
    if (!EventHandlerImplBase<T>::IsSametype(pHandler2)) {
      return false;
    }
    const auto pHandlerCasted = dynamic_cast<const EventHandlerImplForMemberFunction<U, T>*>(pHandler2);
    if (!pHandlerCasted) {
      return false;
    }
    return this->pCaller_ == pHandlerCasted->pCaller_ && this->pMemberFunction_ == pHandlerCasted->pMemberFunction_;
  }

private:
  /**
   * @brief Pointer to object.
   */
  U* pCaller_;
  /**
   * @brief Pointer to class method for event handling.
   */
  void (U::*pMemberFunction_)(const void*, T);
};

/**
 * @brief This is specialization EventHandlerImplForNonMemberFunction for void type.
 * Event handler for a function that takes a pointer to the sender object.
 */
template <>
class EventHandlerImplForNonMemberFunction<void> : public EventHandlerImpl<void> {
public:
  /**
   * @brief Construct a new EventHandlerImplForNonMemberFunction object.
   * @param[in] pFunction Function pointer that takes a pointer to the sender object.
   */
  EventHandlerImplForNonMemberFunction(void (*pFunction)(const void*)) : pFunction_(pFunction) {}

  /**
   * @brief Call handler via pointer to function.
   * The method takes a pointer to the sender.
   * It is recommended to use thread pool for asynchronous notification.
   * @param[in] psender Pointer to the sender.
   * @return EventHandlerAsyncResult Return std::future<bool> as execution result to wait or get operation status.
   */
  // virtual EventHandlerAsyncResult on_event_async(const void* psender) override final
  // {
  //   Task task;
  //   auto result = task.assign(pFunction_, psender);
  //   thread_pool_.push_task(task);
  //   return result;
  // }

  /**
   * @brief Call handler via pointer to function.
   * The method takes a pointer to the sender.
   * @param[in] psender Pointer to the sender.
   */
  virtual void on_event(const void* psender) override final { pFunction_(psender); }

  /**
   * @brief Сhecks the current and passed event handler.
   * Checks the type of the passed event handler with the current one.
   * Performs function check pointer handlers.
   * @param[in] pHandler2 Pointer to the event handler.
   * @return true If handlers are same type and have same function pointer.
   * @return false Otherwise.
   */
  virtual bool IsBindedToSameFunctionAs(const EventHandlerImplBase<void>* pHandler2) const override final
  {
    if (!IsSametype(pHandler2)) {
      return false;
    }
    const auto pHandlerCasted = dynamic_cast<const EventHandlerImplForNonMemberFunction<void>*>(pHandler2);
    if (!pHandlerCasted) {
      return false;
    }

    return this->pFunction_ == pHandlerCasted->pFunction_;
  }

private:
  /**
   * @brief Pointer to a function for event handling.
   */
  void (*pFunction_)(const void*);
};

/**
 * @brief This is specialization EventHandlerImplForMemberFunction for void type.
 * Event handler for a class method that takes a pointer to the sender object.
 * @tparam U Class name.
 */
template <typename U>
class EventHandlerImplForMemberFunction<void, U> : public EventHandlerImpl<void> {
public:
  /**
   * @brief Construct a new EventHandlerImplForMemberFunction object.
   * @param[in] thisPtr Object pointer, that initiate method call via pointer.
   * @param[in] pMemberFunction Function pointer that takes a pointer to the sender object.
   */
  EventHandlerImplForMemberFunction(U* thisPtr, void (U::*pMemberFunction)(const void*))
    : pCaller_(thisPtr), pMemberFunction_(pMemberFunction)
  {
  }

  /**
   * @brief Call handler via pointer to object and pointer to class method.
   * The method takes a pointer to the sender.
   * It is recommended to use thread pool for asynchronous notification.
   * @param[in] psender Pointer to the sender.
   * @return EventHandlerAsyncResult Return std::future<bool> as execution result to wait or get operation status.
   */
  // virtual EventHandlerAsyncResult on_event_async(const void* psender) override final
  // {
  //   Task task;
  //   auto result = task.assign(pCaller_, pMemberFunction_, psender);
  //   thread_pool_.push_task(task);
  //   return result;
  // }

  /**
   * @brief Call handler via pointer to object and pointer to class method.
   * The method takes a pointer to the sender.
   * @param[in] psender Pointer to the sender.
   */
  virtual void on_event(const void* psender) override final { (pCaller_->*(pMemberFunction_))(psender); }

  /**
   * @brief Сhecks the current and passed event handler.
   * Checks the type of the passed event handler with the current one.
   * Checks pointers to objects and member functions.
   * @param[in] pHandler2 Pointer to the event handler.
   * @return true If handlers are same type and have same object and method pointers.
   * @return false Otherwise
   */
  virtual bool IsBindedToSameFunctionAs(const EventHandlerImplBase<void>* pHandler2) const override final
  {
    if (!IsSametype(pHandler2)) {
      return false;
    }

    const auto pHandlerCasted = dynamic_cast<const EventHandlerImplForMemberFunction<void, U>*>(pHandler2);
    if (!pHandlerCasted) {
      return false;
    }
    return this->pCaller_ == pHandlerCasted->pCaller_ && this->pMemberFunction_ == pHandlerCasted->pMemberFunction_;
  }

private:
  /**
   * @brief Pointer to object.
   */
  U* pCaller_;
  /**
   * @brief Pointer to class method for event handling.
   */
  void (U::*pMemberFunction_)(const void*);
};

}  // namespace core
