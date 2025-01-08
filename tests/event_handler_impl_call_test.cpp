#include "common.hpp"

#include "EventHandler.hpp"

#include <gtest/gtest.h>

using testing::_;

namespace {
template<typename T>
class MockExecutableEntity : public ExecutableEntity<T> {
    public:
    MOCK_METHOD(void, primary_execute, (const void* psender, T arg), ());
    MOCK_METHOD(void, void_execute, (const void* psender), ());
};
}

TEST(EvenHandlerImplCallTest, test_on_call_fake_function)
{
    using ArgT = CustomArgumentStruct;
    core::EventHandlerImplPtr<ArgT>
        fake_event_handler(new FakeEventHandlerImpl<ArgT>());
    MockEventHandlerImpl<ArgT> mock_event_handler(fake_event_handler.get());
    mock_event_handler.DelegateToFake();

    EXPECT_CALL(mock_event_handler, OnEvent(_, _));
    EXPECT_CALL(mock_event_handler, IsBindedToSameFunctionAs(_));

    mock_event_handler.OnEvent(nullptr, ArgT());
    mock_event_handler.IsBindedToSameFunctionAs(nullptr);
}

TEST(EvenHandlerImplCallTest, test_on_call_void_non_member_function)
{
    const auto function_event_handler = core::EventHandler::bind(&void_callback);
    MockEventHandlerImpl<void> mock_event_handler(function_event_handler.get());
    mock_event_handler.DelegateToFake();

    EXPECT_CALL(mock_event_handler, OnEvent(_)).Times(1);
    EXPECT_CALL(mock_event_handler, IsBindedToSameFunctionAs(_)).Times(1);

    mock_event_handler.OnEvent(nullptr);
    mock_event_handler.IsBindedToSameFunctionAs(nullptr);
    EXPECT_TRUE(void_counter == 1);
}

TEST(EvenHandlerImplCallTest, test_on_call_parameter_non_member_function)
{
    using ArgT = CustomArgumentStruct;
    const auto function_event_handler = core::EventHandler::bind<ArgT>(&callback);
    MockEventHandlerImpl<ArgT> mock_event_handler(function_event_handler.get());
    mock_event_handler.DelegateToFake();

    EXPECT_CALL(mock_event_handler, OnEvent(_, _)).Times(1);
    EXPECT_CALL(mock_event_handler, IsBindedToSameFunctionAs(_)).Times(1);

    mock_event_handler.OnEvent(nullptr, ArgT());
    mock_event_handler.IsBindedToSameFunctionAs(nullptr);
    EXPECT_TRUE(parameter_counter == 1);
}

TEST(EvenHandlerImplCallTest, test_on_call_parameter_non_member_nullptr)
{
    using ArgT = CustomArgumentStruct;
    const auto function_event_handler = core::EventHandler::bind<ArgT>(nullptr);
    MockEventHandlerImpl<ArgT> mock_event_handler(function_event_handler.get());
    mock_event_handler.DelegateToFake();

    EXPECT_CALL(mock_event_handler, OnEvent(_, _)).Times(1);
    EXPECT_CALL(mock_event_handler, IsBindedToSameFunctionAs(_)).Times(1);

    mock_event_handler.OnEvent(nullptr, ArgT());
    mock_event_handler.IsBindedToSameFunctionAs(nullptr);
}

TEST(EvenHandlerImplCallTest, test_on_call_parameter_member_function)
{
    using ArgT = CustomArgumentStruct;
    MockExecutableEntity<ArgT> mock_exec_entity;
    const auto function_event_handler =
        core::EventHandler::bind<decltype(mock_exec_entity), ArgT>(&mock_exec_entity,
                                                                   &decltype(mock_exec_entity)::primary_execute);
    MockEventHandlerImpl<ArgT> mock_event_handler(function_event_handler.get());
    mock_event_handler.DelegateToFake();

    EXPECT_CALL(mock_exec_entity, primary_execute(_, _)).Times(1);

    mock_event_handler.OnEvent(nullptr, ArgT());
}

TEST(EvenHandlerImplCallTest, test_on_call_parameter_member_nullptr)
{
    using ArgT = CustomArgumentStruct;
    MockExecutableEntity<ArgT> mock_exec_entity;
    const auto function_event_handler =
        core::EventHandler::bind<decltype(mock_exec_entity), ArgT>(nullptr, nullptr);
    MockEventHandlerImpl<ArgT> mock_event_handler(function_event_handler.get());
    mock_event_handler.DelegateToFake();

    EXPECT_CALL(mock_event_handler, OnEvent(_, _)).Times(1);
    EXPECT_CALL(mock_event_handler, IsBindedToSameFunctionAs(_)).Times(1);

    mock_event_handler.OnEvent(nullptr, ArgT());
    mock_event_handler.IsBindedToSameFunctionAs(nullptr);
}

TEST(EvenHandlerImplCallTest, test_on_call_void_member_function)
{
    using ArgT = CustomArgumentStruct;
    MockExecutableEntity<ArgT> mock_exec_entity;
    const auto function_event_handler =
        core::EventHandler::bind<decltype(mock_exec_entity)>(&mock_exec_entity,
                                                             &decltype(mock_exec_entity)::void_execute);
    MockEventHandlerImpl<void> mock_event_handler(function_event_handler.get());
    mock_event_handler.DelegateToFake();

    EXPECT_CALL(mock_exec_entity, void_execute(_)).Times(1);

    mock_event_handler.OnEvent(nullptr);
}
