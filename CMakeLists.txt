cmake_minimum_required(VERSION 3.15)
project(core)

# Default to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

set(CURRENT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(INC_DIR ${CURRENT_DIR}/include)

find_package(jsoncpp REQUIRED)
find_package(Threads REQUIRED)

file(GLOB SOURCES
${CURRENT_DIR}/src/*.cpp
)

set(core ${PROJECT_NAME})

add_library(core SHARED
${SOURCES}
)
target_include_directories(core PRIVATE
${INC_DIR}
)
target_link_libraries(core
  jsoncpp_lib
  Threads::Threads
)

if(ENABLE_EXAMPLES)
  add_subdirectory(examples)
endif()

install(TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY ${INC_DIR}
  DESTINATION include/
)

if(ENABLE_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
