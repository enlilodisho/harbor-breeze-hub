set(Headers
    ComponentMasterForTests.h
    EventDispatcherForTests.h
    FakeComponent.h
)

set(Sources
    ExampleTests.cpp
    InterruptsControllerTests.cpp
    RFReceiverTests.cpp
    RFTransmitterTests.cpp
    SSLLParserTests.cpp
)

add_executable(${TEST_NAME} ${Sources} ${Headers})
target_link_libraries(${TEST_NAME} PUBLIC
    gtest_main
    ${LIBRARY_NAME}
)
target_include_directories(${TEST_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_test(
    NAME ${TEST_NAME}
    COMMAND ${TEST_NAME}
)