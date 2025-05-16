cmake_minimum_required (VERSION 3.29)

project ("csv_parser" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PARSER_LIB ${PROJECT_NAME}lib)

set(SRC_PATH ./src)

include_directories(${PROJECT_NAME} ./include)

enable_testing()

add_subdirectory(tests)

add_library(
    ${PARSER_LIB}
    STATIC
    "${SRC_PATH}/CSVFile.cpp"
)

add_executable (
    ${PROJECT_NAME}
    "${SRC_PATH}/main.cpp"
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${PARSER_LIB})
