cmake_minimum_required(VERSION 3.16.3)

set(PROJECT_NAME HarborBreezeHub)
set(LIBRARY_NAME ${PROJECT_NAME})
set(EXECUTABLE_NAME HBHub)

project(${PROJECT_NAME} VERSION 0.1.0
                        DESCRIPTION "Smart hub for harbor breeze ceiling fans."
                        LANGUAGES CXX)

configure_file(HarborBreezeHubConfig.h.in HarborBreezeHubConfig.h)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)

# Add linker flag for boost::filesystem
set(CMAKE_CXX_FLAGS "-lboost_filesystem")

option(PACKAGE_TESTS "Build the tests" ON)
if (PACKAGE_TESTS)
    set(TEST_NAME ${PROJECT_NAME}Tests)
    enable_testing()
    add_subdirectory(libs/external/googletest)
    add_subdirectory(test)
endif()

add_subdirectory(libs/component-event-system)
add_subdirectory(src)

target_include_directories(${LIBRARY_NAME} PUBLIC ${PROJECT_BINARY_DIR})