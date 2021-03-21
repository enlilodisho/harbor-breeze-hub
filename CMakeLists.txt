cmake_minimum_required(VERSION 3.16.3)

project(HarborBreezeHub VERSION 0.1.0
                        DESCRIPTION "Smart hub for harbor breeze ceiling fans."
                        LANGUAGES CXX)

configure_file(HarborBreezeHubConfig.h.in HarborBreezeHubConfig.h)

add_subdirectory(src)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR})