cmake_minimum_required (VERSION 2.6)

project(BookTests)
SET(CMAKE_CXX_FLAGS "-O2 -Wall -pedantic -std=c++14")
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

set(${PROJECT_NAME}_SOURCES
    src/main.cpp
    src/math.cpp
    src/string.cpp
    src/datastruct.cpp
    src/flow.cpp
    src/graph.cpp
)
add_subdirectory("rapidcheck")

include_directories(../src)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES})
target_link_libraries(${PROJECT_NAME} rapidcheck)
