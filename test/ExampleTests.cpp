#include <gtest/gtest.h>

struct ExampleTests : public ::testing::Test
{
    virtual void SetUp() override
    {

    }

    virtual void TearDown() override
    {

    }
};

TEST_F(ExampleTests, FakePassingTest)
{
    bool x = true;
    ASSERT_TRUE(x);
}