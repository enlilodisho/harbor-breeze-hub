//
// Created by enlil on 11/18/21.
//
#include "EventDispatcherForTests.h"
#include "HarborBreezeHub/SSLLDecoder.h"

#include <gtest/gtest.h>

struct SSLLDecoderTests : public ::testing::Test
{
    std::unique_ptr<SSLLDecoder> ssllDecoder;
    std::unique_ptr<EventDispatcherForTests> eventDispatcherForTests;

    void SetUp() override
    {

        ssllDecoder = std::make_unique<SSLLDecoder>("SSLLDecoder", 60,
                                                   400, 500, 850, 950, 10400);
        eventDispatcherForTests = std::make_unique<EventDispatcherForTests>();
        ssllDecoder->setEventDispatcher(eventDispatcherForTests.get());
    }

    void TearDown() override
    {

    }
};

TEST_F(SSLLDecoderTests, GetBinaryFromTimingsTest)
{
    std::vector<unsigned int> timings { 400, 500, 850, 950, 400, 950, 400, 500, 850, 950, 400, 500, 850, 500, 850, 950, 400, 500, 850, 950, 400, 500, 850, 500, 850, 500, 850, 950, 400, 950, 850, 950, 400, 500, 850, 500, 850, 500, 850, 950, 400, 500, 850, 950, 400, 500, 850, 950, 400 };
    std::string expectedBinary = "10110010010110010110110010110010110110110010011001011011011001011001011001";
    std::string actualBinary;
    ASSERT_TRUE(ssllDecoder->getBinaryStringFromTimings(timings, actualBinary).success);
    ASSERT_EQ(actualBinary, expectedBinary);
}

TEST_F(SSLLDecoderTests, GetTimingsFromBinaryTest)
{
    std::string binary = "10110010010110010110110010110010110110110010011001011011011001011001011001";
    std::vector<unsigned int> expectedTimings { 400, 500, 850, 950, 400, 950, 400, 500, 850, 950, 400, 500, 850, 500, 850, 950, 400, 500, 850, 950, 400, 500, 850, 500, 850, 500, 850, 950, 400, 950, 850, 950, 400, 500, 850, 500, 850, 500, 850, 950, 400, 500, 850, 950, 400, 500, 850, 950, 400 };
    std::vector<unsigned int> actualTimings;
    ASSERT_TRUE(ssllDecoder->getTimingsFromBinaryString(binary, actualTimings).success);
    ASSERT_EQ(actualTimings, expectedTimings);
}

TEST_F(SSLLDecoderTests, GetDataStringFromTimingsTest)
{
    std::vector<unsigned int> timings { 400, 500, 850, 950, 400, 950, 400, 500, 850, 950, 400, 500, 850, 500, 850, 950, 400, 500, 850, 950, 400, 500, 850, 500, 850, 500, 850, 950, 400, 950, 850, 950, 400, 500, 850, 500, 850, 500, 850, 950, 400, 500, 850, 950, 400, 500, 850, 950, 400 };
    std::string expectedDataString = "SSLLSLSSLLSSLSLLSSLLSSLSLSLLSLLLSSLSLSLLSSLLSSLLS";
    std::string actualDataString;
    ASSERT_TRUE(ssllDecoder->getDataStringFromTimings(timings, actualDataString).success);
    ASSERT_EQ(actualDataString, expectedDataString);
}

TEST_F(SSLLDecoderTests, GetTimingsFromDataStringTest)
{
    std::string dataString = "SSLLSLSSLLSSLSLLSSLLSSLSLSLLSLLLSSLSLSLLSSLLSSLLS";
    std::vector<unsigned int> expectedTimings { 400, 500, 850, 950, 400, 950, 400, 500, 850, 950, 400, 500, 850, 500, 850, 950, 400, 500, 850, 950, 400, 500, 850, 500, 850, 500, 850, 950, 400, 950, 850, 950, 400, 500, 850, 500, 850, 500, 850, 950, 400, 500, 850, 950, 400, 500, 850, 950, 400 };
    std::vector<unsigned int> actualTimings;
    ASSERT_TRUE(ssllDecoder->getTimingsFromDataString(dataString, actualTimings).success);
    ASSERT_EQ(actualTimings, expectedTimings);
}