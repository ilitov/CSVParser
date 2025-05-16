cmake_minimum_required(VERSION 3.29)

project(csv_tests)

Include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.17.0
)

FetchContent_MakeAvailable(googletest)

add_executable(
    ${PROJECT_NAME}
    process_commands_tests.cpp
    csvfile_tests.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE GTest::gtest_main)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PARSER_LIB})

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
