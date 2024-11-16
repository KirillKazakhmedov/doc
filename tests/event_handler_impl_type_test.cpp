#include "common.hpp"
#include "EventHandler.hpp"

#include <gtest/gtest.h>

using testing::_;

TEST(EvenHandlerImplTypeTest, test_on_same_type_non_member_function)
{
    core::EventHandlerImplForNonMemberFunction<void> void_handler(&void_callback);
    core::EventHandlerImplForNonMemberFunction<int> int_handler(&callback<int>);
    core::EventHandlerImplForNonMemberFunction<std::string> string_handler(&callback<std::string>);
    core::EventHandlerImplForNonMemberFunction<CustomArgumentStruct>
            custom_handler(&callback<CustomArgumentStruct>);
    core::EventHandlerImplForNonMemberFunction<CustomArgumentStruct>
            another_custom_handler(&custom_callback);

    EXPECT_TRUE(void_handler.IsSametype(&void_handler));
    EXPECT_TRUE(void_handler.IsBindedToSameFunctionAs(&void_handler));
    EXPECT_TRUE(int_handler.IsSametype(&int_handler));
    EXPECT_TRUE(int_handler.IsBindedToSameFunctionAs(&int_handler));
    EXPECT_TRUE(string_handler.IsSametype(&string_handler));
    EXPECT_TRUE(string_handler.IsBindedToSameFunctionAs(&string_handler));
    EXPECT_TRUE(custom_handler.IsSametype(&custom_handler));
    EXPECT_TRUE(custom_handler.IsBindedToSameFunctionAs(&custom_handler));

    EXPECT_TRUE(custom_handler.IsSametype(&another_custom_handler));
    EXPECT_FALSE(custom_handler.IsBindedToSameFunctionAs(&another_custom_handler));

    EXPECT_FALSE(custom_handler.IsSametype(nullptr));
    EXPECT_FALSE(custom_handler.IsBindedToSameFunctionAs(nullptr));
}

TEST(EvenHandlerImplTypeTest, test_on_same_type_member_function)
{
    ExecutableEntity<CustomArgumentStruct> entity_obj;
    core::EventHandlerImplForMemberFunction<decltype(entity_obj), CustomArgumentStruct>
            custom_handler(&entity_obj, &decltype(entity_obj)::primary_execute);
    core::EventHandlerImplForMemberFunction<decltype(entity_obj), CustomArgumentStruct>
            another_custom_handler(&entity_obj, &decltype(entity_obj)::secondary_execute);
    core::EventHandlerImplForMemberFunction<decltype(entity_obj), void>
            void_custom_handler(&entity_obj, &decltype(entity_obj)::void_execute);

    EXPECT_TRUE(custom_handler.IsSametype(&custom_handler));
    EXPECT_TRUE(custom_handler.IsBindedToSameFunctionAs(&custom_handler));
    EXPECT_TRUE(another_custom_handler.IsSametype(&another_custom_handler));
    EXPECT_TRUE(another_custom_handler.IsBindedToSameFunctionAs(&another_custom_handler));
    EXPECT_TRUE(void_custom_handler.IsSametype(&void_custom_handler));
    EXPECT_TRUE(void_custom_handler.IsBindedToSameFunctionAs(&void_custom_handler));

    EXPECT_TRUE(custom_handler.IsSametype(&another_custom_handler));
    EXPECT_FALSE(custom_handler.IsBindedToSameFunctionAs(&another_custom_handler));

    EXPECT_FALSE(custom_handler.IsSametype(nullptr));
    EXPECT_FALSE(custom_handler.IsBindedToSameFunctionAs(nullptr));
}

TEST(EvenHandlerImplTypeTest, test_on_event_handler_result)
{
    ExecutableEntity<CustomArgumentStruct> entity_obj;
    const auto void_function_handler = core::EventHandler::bind(&void_callback);
    const auto custom_function_handler = core::EventHandler::bind(&custom_callback);
    const auto void_member_fnuction_handler = core::EventHandler::bind(&entity_obj, &ExecutableEntity<CustomArgumentStruct>::void_execute);
    const auto custom_member_fnuction_handler = core::EventHandler::bind(&entity_obj, &ExecutableEntity<CustomArgumentStruct>::primary_execute);

    const auto pvoid_function_handler =
            dynamic_cast<const core::EventHandlerImplForNonMemberFunction<void>*>(void_function_handler.get());
    EXPECT_TRUE(pvoid_function_handler != nullptr);
    const auto pcustom_function_handler =
            dynamic_cast<const core::EventHandlerImplForNonMemberFunction<CustomArgumentStruct>*>(custom_function_handler.get());
    EXPECT_TRUE(pcustom_function_handler != nullptr);
    const auto pvoid_member_fnuction_handler =
            dynamic_cast<const core::EventHandlerImplForMemberFunction<decltype(entity_obj), void>*>(void_member_fnuction_handler.get());
    EXPECT_TRUE(pvoid_member_fnuction_handler != nullptr);
    const auto pcustom_member_fnuction_handler =
            dynamic_cast<const core::EventHandlerImplForMemberFunction<decltype(entity_obj), CustomArgumentStruct>*>(custom_member_fnuction_handler.get());
    EXPECT_TRUE(pcustom_member_fnuction_handler != nullptr);
}
