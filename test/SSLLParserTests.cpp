//
// Created by enlil on 11/18/21.
//
#include "EventDispatcherForTests.h"
#include "HarborBreezeHub/SSLLParser.h"

#include <gtest/gtest.h>

struct SSLLParserTests : public ::testing::Test
{
    std::unique_ptr<SSLLParser> ssllParser;
    std::unique_ptr<EventDispatcherForTests> eventDispatcherForTests;

    void SetUp() override
    {

        ssllParser = std::make_unique<SSLLParser>("SSLLParser", 60,
                                                  400, 500,850, 950, 10400);
        eventDispatcherForTests = std::make_unique<EventDispatcherForTests>();
        ssllParser->setEventDispatcher(eventDispatcherForTests.get());
    }

    void TearDown() override
    {

    }
};

TEST_F(SSLLParserTests, FakePassingTest)
{
    ASSERT_TRUE(true);
}