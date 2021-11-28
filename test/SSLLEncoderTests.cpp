//
// Created by enlil on 11/18/21.
//
#include "HarborBreezeHub/SSLLEncoder.h"

#include <gtest/gtest.h>

struct SSLLEncoderTests : public ::testing::Test
{
    std::unique_ptr<SSLLEncoder> ssllEncoder;

    void SetUp() override
    {

        ssllEncoder = std::make_unique<SSLLEncoder>(400, 500, 850, 950, 10400);
    }

    void TearDown() override
    {

    }
};

TEST_F(SSLLEncoderTests, GetTimingsFromBinaryTest)
{
    std::string binary = "10110010010110010110110010110010110110110010011001011011011001011001011001";
    std::vector<unsigned int> expectedTimings { 400, 500, 850, 950, 400, 950, 400, 500, 850, 950, 400, 500, 850, 500, 850, 950, 400, 500, 850, 950, 400, 500, 850, 500, 850, 500, 850, 950, 400, 950, 850, 950, 400, 500, 850, 500, 850, 500, 850, 950, 400, 500, 850, 950, 400, 500, 850, 950, 400 };
    std::vector<unsigned int> actualTimings;
    ASSERT_TRUE(ssllEncoder->getTimingsFromBinaryString(binary, actualTimings).success);
    ASSERT_EQ(actualTimings, expectedTimings);
}

TEST_F(SSLLEncoderTests, GetTimingsFromDataStringTest)
{
    std::string dataString = "SSLLSLSSLLSSLSLLSSLLSSLSLSLLSLLLSSLSLSLLSSLLSSLLS";
    std::vector<unsigned int> expectedTimings { 400, 500, 850, 950, 400, 950, 400, 500, 850, 950, 400, 500, 850, 500, 850, 950, 400, 500, 850, 950, 400, 500, 850, 500, 850, 500, 850, 950, 400, 950, 850, 950, 400, 500, 850, 500, 850, 500, 850, 950, 400, 500, 850, 950, 400, 500, 850, 950, 400 };
    std::vector<unsigned int> actualTimings;
    ASSERT_TRUE(ssllEncoder->getTimingsFromDataString(dataString, actualTimings).success);
    ASSERT_EQ(actualTimings, expectedTimings);
}