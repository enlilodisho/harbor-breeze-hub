cmake_minimum_required(VERSION 3.16.3)

set(PROJECT_NAME ComponentEventSystem)
set(LIBRARY_NAME ${PROJECT_NAME})

project(${PROJECT_NAME} VERSION 1.0
                        DESCRIPTION "C++ event system with components running on different threads"
                        LANGUAGES CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src)