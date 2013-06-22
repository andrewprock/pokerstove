#include <gtest/gtest.h>
#include "CardSetGenerators.h"

TEST(CardSetGeneratorsTest, CardSetSizes)
{
    EXPECT_EQ(52, pokerstove::createCardSets(1).size());
    EXPECT_EQ(1326, pokerstove::createCardSets(2).size());
    EXPECT_EQ(22100, pokerstove::createCardSets(3).size());
}

TEST(CardSetGeneratorsTest, CanonSetSizes)
{
    EXPECT_EQ(13, pokerstove::createCanonSets(1).size());
    EXPECT_EQ(169, pokerstove::createCanonSets(2).size());
    EXPECT_EQ(1755, pokerstove::createCanonSets(3).size());
}

TEST(CardSetGeneratorsTest, RankSetSizes)
{
    EXPECT_EQ(13, pokerstove::createRankSets(1).size());
    EXPECT_EQ(91, pokerstove::createRankSets(2).size());
    EXPECT_EQ(455, pokerstove::createRankSets(3).size());
}

