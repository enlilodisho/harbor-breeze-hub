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

TEST_F(SSLLParserTests, GetBinaryFromTimingsTest)
{
    std::vector<unsigned int> timings { 400, 500, 850, 950, 400, 950, 400, 500, 850, 950, 400, 500, 850, 500, 850, 950, 400, 500, 850, 950, 400, 500, 850, 500, 850, 500, 850, 950, 400, 950, 850, 950, 400, 500, 850, 500, 850, 500, 850, 950, 400, 500, 850, 950, 400, 500, 850, 950, 400 };
    std::string expectedBinary = "10110010010110010110110010110010110110110010011001011011011001011001011001";
    std::string actualBinary;
    ASSERT_TRUE(ssllParser->getBinaryStringFromTimings(timings, actualBinary).success);
    ASSERT_EQ(actualBinary, expectedBinary);
}

TEST_F(SSLLParserTests, GetTimingsFromBinaryTest)
{
    std::string binary = "10110010010110010110110010110010110110110010011001011011011001011001011001";
    std::vector<unsigned int> expectedTimings { 400, 500, 850, 950, 400, 950, 400, 500, 850, 950, 400, 500, 850, 500, 850, 950, 400, 500, 850, 950, 400, 500, 850, 500, 850, 500, 850, 950, 400, 950, 850, 950, 400, 500, 850, 500, 850, 500, 850, 950, 400, 500, 850, 950, 400, 500, 850, 950, 400 };
    std::vector<unsigned int> actualTimings;
    ASSERT_TRUE(ssllParser->getTimingsFromBinaryString(binary, actualTimings).success);
    ASSERT_EQ(actualTimings, expectedTimings);
}