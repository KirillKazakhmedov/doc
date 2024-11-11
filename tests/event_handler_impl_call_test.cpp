#include "common.hpp"

#include "EventHandler.hpp"

#include <gtest/gtest.h>

using testing::_;

TEST(EvenHandlerImplCallTest, test_on_call_fake_function)
{
    core::EventHandlerImplPtr<CustomArgumentStruct> 
        fake_event_handler(new FakeEventHandlerImpl<CustomArgumentStruct>());
    MockEventHandlerImpl<CustomArgumentStruct> mock_event_handler(fake_event_handler.get());
    mock_event_handler.DelegateToFake();

    EXPECT_CALL(mock_event_handler, OnEvent(_, _));
    EXPECT_CALL(mock_event_handler, IsBindedToSameFunctionAs(_));

    mock_event_handler.OnEvent(nullptr, CustomArgumentStruct());
    mock_event_handler.IsBindedToSameFunctionAs(nullptr);
}

TEST(EvenHandlerImplCallTest, test_on_call_void_non_member_function)
{
    const auto function_event_handler = core::EventHandler::bind(&void_callback);
    MockEventHandlerImpl<void> mock_event_handler(function_event_handler.get());
    mock_event_handler.DelegateToFake();

    EXPECT_CALL(mock_event_handler, OnEvent(_));
    EXPECT_CALL(mock_event_handler, IsBindedToSameFunctionAs(_));

    mock_event_handler.OnEvent(nullptr);
    mock_event_handler.IsBindedToSameFunctionAs(nullptr);
    EXPECT_TRUE(void_counter == 1);
}

TEST(EvenHandlerImplCallTest, test_on_call_parameter_non_member_function)
{
    const auto function_event_handler = core::EventHandler::bind<CustomArgumentStruct>(&callback);
    MockEventHandlerImpl<CustomArgumentStruct> mock_event_handler(function_event_handler.get());
    mock_event_handler.DelegateToFake();

    EXPECT_CALL(mock_event_handler, OnEvent(_, _));
    EXPECT_CALL(mock_event_handler, IsBindedToSameFunctionAs(_));

    mock_event_handler.OnEvent(nullptr, CustomArgumentStruct());
    mock_event_handler.IsBindedToSameFunctionAs(nullptr);
    EXPECT_TRUE(parameter_counter == 1);
}