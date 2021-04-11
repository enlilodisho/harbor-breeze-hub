set(Sources
    ExampleTests.cpp
    FanManagerTests.cpp
)

add_executable(${TEST_NAME} ${Sources})
target_link_libraries(${TEST_NAME} PUBLIC
    gtest_main
    ${LIBRARY_NAME}
)
target_include_directories(${TEST_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_test(
    NAME ${TEST_NAME}
    COMMAND ${TEST_NAME}
)