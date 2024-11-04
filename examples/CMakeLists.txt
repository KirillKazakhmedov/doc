cmake_minimum_required (VERSION 3.15)

add_executable (event_example event_example.cpp)
add_executable (thread_pool_example thread_pool_example.cpp)

target_include_directories(event_example PRIVATE 
${INC_DIR}
)
target_link_libraries(event_example
core
)

target_include_directories(thread_pool_example PRIVATE 
${INC_DIR}
)
target_link_libraries(thread_pool_example
core
)

install(TARGETS event_example thread_pool_example
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION lib/${PROJECT_NAME}
)