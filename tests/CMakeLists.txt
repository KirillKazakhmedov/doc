find_package(GTest REQUIRED)

set(TEST_PROJECT ${PROJECT_NAME}_test)

file(GLOB SOURCES_CPP_TEST *.cpp)

add_executable(
  ${TEST_PROJECT}
  ${SOURCES_CPP_TEST}
)

target_link_libraries(${TEST_PROJECT} PRIVATE 
    GTest::gtest
    GTest::gtest_main)

gtest_add_tests(TARGET ${TEST_PROJECT}
    TEST_SUFFIX .noArgs
    TEST_LIST noArgsTests
)
set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 10)
