#pragma once

#include <typeinfo>

namespace core {
/**
 * @brief This class contains some check functions for primary verification
 * using event model.
 * This interface to implement type check and check event
 * handler depend on some function or method.
 * @tparam T Argument type acquiring function/method handler.
 */
template <typename T>
class EventHandlerImplBase {
public:
  /**
   * @brief Default dtor. Destruct EventHandlerImplBase instance.
   */
  virtual ~EventHandlerImplBase() = default;

  /**
   * @brief Interface function that checks the current and passed event handler.
   * @return true If both handlers are of the same type and point to the same function/method.
   * @return false Otherwise
   */
  virtual bool IsBindedToSameFunctionAs(const EventHandlerImplBase<T>* pHandler) const = 0;

  /**
   * @brief Checks the type of the current handler with the one passed.
   * This check must be included in the IsBindedToSameFunctionAs method.
   * @param EventHandlerImplBase<T>* Pointer to passed handler
   * @return true If types are matched
   * @return false Otherwise
   */
  bool IsSametype(const EventHandlerImplBase<T>* pHandler) const
  {
    if (!pHandler || typeid(*this) != typeid(*pHandler)) {
      return false;
    }
    return true;
  }
};
}  // namespace core
