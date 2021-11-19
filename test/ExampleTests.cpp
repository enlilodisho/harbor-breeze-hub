#include <gtest/gtest.h>

struct ExampleTests : public ::testing::Test
{
    void SetUp() override
    {

    }

    void TearDown() override
    {

    }
};

TEST_F(ExampleTests, FakePassingTest)
{
    bool x = true;
    ASSERT_TRUE(x);
}