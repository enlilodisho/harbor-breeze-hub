set(Sources
    ExampleTests.cpp
)

add_executable(${TEST_NAME} ${Sources})
target_link_libraries(${TEST_NAME} PUBLIC
    gtest_main
    ${LIBRARY_NAME}
)

add_test(
    NAME ${TEST_NAME}
    COMMAND ${TEST_NAME}
)