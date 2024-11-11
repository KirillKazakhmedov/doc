#include "EventHandlerImpl.hpp"
#include "EventHandler.hpp"

//#include <gtest/gtest.h>
#include <gmock/gmock.h>

using testing::_;

namespace {
struct CustomArgumentStruct {
    int a;
    double b;
    std::string str;
};

template<typename T>
void callback(const void* psender, T arg) {}
void void_callback(const void* psender) {}

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
   virtual void OnEvent(const void* psender, const T& arg)
   {

   }
   virtual bool IsBindedToSameFunctionAs(const core::EventHandlerImplBase<T>* pHandler) const
   {
    return true;
   }
};

template<typename T>
class MockEventHandlerImpl : public core::EventHandlerImpl<T> {
 public:
  MockEventHandlerImpl(const core::EventHandlerImplSPtr<T>& event_handler)
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
  core::EventHandlerImplSPtr<T> event_handler_;  // Keeps an instance of the fake in the mock.
};
}

TEST(EvenHandlerImplFunctionTest, test_on_call_function)
{
    // ExecutableEntity<CustomArgumentStruct> entity_obj;
    // const auto void_function_handler = core::EventHandler::bind(&void_callback);
    // const auto custom_function_handler = core::EventHandler::bind(&callback<CustomArgumentStruct>);
    core::EventHandlerImplSPtr<CustomArgumentStruct> 
        fake_event_handler(new FakeEventHandlerImpl<CustomArgumentStruct>());
    MockEventHandlerImpl<CustomArgumentStruct> mock_event_handler(fake_event_handler);
    mock_event_handler.DelegateToFake();

    EXPECT_CALL(mock_event_handler, OnEvent(_, _));
    EXPECT_CALL(mock_event_handler, IsBindedToSameFunctionAs(_));

    mock_event_handler.OnEvent(nullptr, CustomArgumentStruct());  // FakeFoo::DoThis() is invoked.
    mock_event_handler.IsBindedToSameFunctionAs(nullptr);
}