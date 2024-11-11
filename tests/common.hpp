#include "EventHandlerImpl.hpp"

#include <gmock/gmock.h>

namespace {

static int void_counter = 0;
static int parameter_counter = 0;

struct CustomArgumentStruct {
    int a;
    double b;
    std::string str;
};

template<typename T>
void callback(const void* psender, T arg) { parameter_counter++; }
void custom_callback(const void* psender, CustomArgumentStruct arg) { }
void void_callback(const void* psender) { void_counter++; }

template<typename T>
class ExecutableEntity {
    public:
    void primary_execute(const void* psender, T arg) {}
    void secondary_execute(const void* psender, T arg) {}
    void void_execute(const void* psender) {}
};

template<typename T>
class FakeEventHandlerImpl : public core::EventHandlerImpl<T> {
 public:
   virtual void OnEvent(const void* psender, const T& arg) override final
   {

   }
   virtual bool IsBindedToSameFunctionAs(const core::EventHandlerImplBase<T>* pHandler) const override final
   {
    return true;
   }
};

template<>
class FakeEventHandlerImpl<void> : public core::EventHandlerImpl<void> {
 public:
   virtual void OnEvent(const void* psender) override final
   {

   }
   virtual bool IsBindedToSameFunctionAs(const core::EventHandlerImplBase<void>* pHandler) const override final
   {
    return true;
   }
};

template<typename T>
class MockEventHandlerImpl : public core::EventHandlerImpl<T> {
 public:
  MockEventHandlerImpl(core::EventHandlerImpl<T>* event_handler)
  : event_handler_(event_handler) {}
  // Normal mock method definitions using gMock.
  MOCK_METHOD(bool, IsBindedToSameFunctionAs, (const core::EventHandlerImplBase<T>* pHandler), (const, override));
  MOCK_METHOD(void, OnEvent, (const void* psender, const T& arg), (override));

  // Delegates the default actions of the methods to a FakeFoo object.
  // This must be called *before* the custom ON_CALL() statements.
  void DelegateToFake() {
    ON_CALL(*this, OnEvent).WillByDefault([this](const void* psender, const T& arg) {
      event_handler_->OnEvent(psender, arg);
    });
    ON_CALL(*this, IsBindedToSameFunctionAs).WillByDefault([this](const core::EventHandlerImplBase<T>* pHandler) {
      return event_handler_->IsBindedToSameFunctionAs(pHandler);
    });
  }

 private:
  core::EventHandlerImpl<T>* event_handler_;
};

template<>
class MockEventHandlerImpl<void> : public core::EventHandlerImpl<void> {
 public:
  MockEventHandlerImpl(core::EventHandlerImpl<void>* event_handler)
  : event_handler_(event_handler) {}
  // Normal mock method definitions using gMock.
  MOCK_METHOD(bool, IsBindedToSameFunctionAs, (const core::EventHandlerImplBase<void>* pHandler), (const, override));
  MOCK_METHOD(void, OnEvent, (const void* psender), (override));

  // Delegates the default actions of the methods to a FakeFoo object.
  // This must be called *before* the custom ON_CALL() statements.
  void DelegateToFake() {
    ON_CALL(*this, OnEvent).WillByDefault([this](const void* psender) {
      event_handler_->OnEvent(psender);
    });
    ON_CALL(*this, IsBindedToSameFunctionAs).WillByDefault([this](const core::EventHandlerImplBase<void>* pHandler) {
      return event_handler_->IsBindedToSameFunctionAs(pHandler);
    });
  }

 private:
  core::EventHandlerImpl<void>* event_handler_;
};
}