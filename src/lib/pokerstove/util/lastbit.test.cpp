#include "lastbit.h"
#include <gtest/gtest.h>

TEST(FirstBitTest, firstbit64)
{
    // firstbit arg is uint64_t
    // There are no overloads, so we don't have to worry about which
    // one is being tested.
    EXPECT_EQ(0, firstbit(0));
    EXPECT_EQ(0, firstbit(1));
    EXPECT_EQ(1, firstbit(2));
    EXPECT_EQ(1, firstbit(3));
    EXPECT_EQ(2, firstbit(4));
    EXPECT_EQ(4, firstbit(31));
    EXPECT_EQ(5, firstbit(32));

    for (int i = 1; i < 64; i++)
    {
        EXPECT_EQ(i - 1, firstbit((UINT64_C(1) << i) - 1));
        EXPECT_EQ(i, firstbit((UINT64_C(1) << i)));
    }
}

TEST(LastBitTest, lastbit32)
{
    // There are uint16_t, uint32_t and uint64_t overloads of lastbit.
    // Explicitly cast arg so we know which we are testing.
    EXPECT_EQ(0, lastbit(uint32_t(0)));
    EXPECT_EQ(0, lastbit(uint32_t(1)));
    EXPECT_EQ(1, lastbit(uint32_t(2)));
    EXPECT_EQ(0, lastbit(uint32_t(3)));
    EXPECT_EQ(2, lastbit(uint32_t(4)));
    EXPECT_EQ(0, lastbit(uint32_t(31)));
    EXPECT_EQ(5, lastbit(uint32_t(32)));
    EXPECT_EQ(2, lastbit(uint32_t(36)));

    for (int i = 1; i < 32; i++)
    {
        EXPECT_EQ(0, lastbit(uint32_t((UINT32_C(1) << i) - 1)));
        EXPECT_EQ(i, lastbit(uint32_t((UINT32_C(1) << i))));
        EXPECT_EQ(0, lastbit(uint32_t((UINT32_C(1) << i) + 1)));
        // Set a higher bit also.
        if (i < 30)
        {
            EXPECT_EQ(i, lastbit(uint32_t((UINT32_C(1) << i) |
                                          (UINT32_C(1) << (i + 2)))));
        }
    }
}

TEST(LastBitTest, lastbit16)
{
    // There are uint16_t, uint32_t and uint64_t overloads of lastbit.
    // Explicitly cast arg so we know which we are testing.
    EXPECT_EQ(0, lastbit(uint16_t(0)));
    EXPECT_EQ(0, lastbit(uint16_t(1)));
    EXPECT_EQ(1, lastbit(uint16_t(2)));
    EXPECT_EQ(0, lastbit(uint16_t(3)));
    EXPECT_EQ(2, lastbit(uint16_t(4)));
    EXPECT_EQ(0, lastbit(uint16_t(31)));
    EXPECT_EQ(5, lastbit(uint16_t(32)));
    EXPECT_EQ(2, lastbit(uint16_t(36)));

    for (int i = 1; i < 16; i++)
    {
        EXPECT_EQ(0, lastbit(uint16_t((UINT16_C(1) << i) - 1)));
        EXPECT_EQ(i, lastbit(uint16_t((UINT16_C(1) << i))));
        EXPECT_EQ(0, lastbit(uint16_t((UINT16_C(1) << i) + 1)));
        if (i < 14)
        {
            EXPECT_EQ(i, lastbit(uint16_t((UINT16_C(1) << i) |
                                          (UINT16_C(1) << (i + 2)))));
        }
    }
}

TEST(LastBitTest, lastbit64)
{
    // There are uint16_t, uint32_t and uint64_t overloads of lastbit.
    // Explicitly cast arg so we know which we are testing.
    EXPECT_EQ(0, lastbit(uint64_t(0)));
    EXPECT_EQ(0, lastbit(uint64_t(1)));
    EXPECT_EQ(1, lastbit(uint64_t(2)));
    EXPECT_EQ(0, lastbit(uint64_t(3)));
    EXPECT_EQ(2, lastbit(uint64_t(4)));
    EXPECT_EQ(0, lastbit(uint64_t(31)));
    EXPECT_EQ(5, lastbit(uint64_t(32)));
    EXPECT_EQ(2, lastbit(uint64_t(36)));

    for (int i = 1; i < 64; i++)
    {
        EXPECT_EQ(0, lastbit(uint64_t((UINT64_C(1) << i) - 1)));
        EXPECT_EQ(i, lastbit(uint64_t((UINT64_C(1) << i))));
        EXPECT_EQ(0, lastbit(uint64_t((UINT64_C(1) << i) + 1)));
        if (i < 62)
        {
            EXPECT_EQ(i, lastbit(uint64_t((UINT64_C(1) << i) |
                                          (UINT64_C(1) << (i + 2)))));
        }
    }
}

TEST(LastBit64Test, lastbit64)
{
    // No overloads for this function.
    EXPECT_EQ(0, lastbit64(0));
    EXPECT_EQ(0, lastbit64(1));
    EXPECT_EQ(1, lastbit64(2));
    EXPECT_EQ(0, lastbit64(3));
    EXPECT_EQ(2, lastbit64(4));
    EXPECT_EQ(0, lastbit64(31));
    EXPECT_EQ(5, lastbit64(32));
    EXPECT_EQ(2, lastbit64(36));

    for (int i = 1; i < 64; i++)
    {
        EXPECT_EQ(0, lastbit64((UINT64_C(1) << i) - 1));
        EXPECT_EQ(i, lastbit64((UINT64_C(1) << i)));
        EXPECT_EQ(0, lastbit64((UINT64_C(1) << i) + 1));
        if (i < 62)
        {
            EXPECT_EQ(i,
                      lastbit64((UINT64_C(1) << i) | (UINT64_C(1) << (i + 2))));
        }
    }
}
