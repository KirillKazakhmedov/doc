find_package(GTest REQUIRED)
include(GoogleTest)

set(TEST_PROJECT ${PROJECT_NAME}_test)

file(GLOB SOURCES_CPP_TEST *.cpp)

add_executable(
  ${TEST_PROJECT}
  ${SOURCES_CPP_TEST}
)

target_include_directories(${TEST_PROJECT} PRIVATE 
${INC_DIR}
)

target_link_libraries(${TEST_PROJECT} PRIVATE 
    GTest::gtest
    GTest::gmock)

gtest_discover_tests(${TEST_PROJECT})
