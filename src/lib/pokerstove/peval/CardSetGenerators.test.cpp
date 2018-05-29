#include "CardSetGenerators.h"
#include <gtest/gtest.h>

using namespace pokerstove;

TEST(CardSetGeneratorsTest, CardSetSizes)
{
    EXPECT_EQ(52, pokerstove::createCardSet(1).size());
    EXPECT_EQ(1326, pokerstove::createCardSet(2).size());
    EXPECT_EQ(22100, pokerstove::createCardSet(3).size());
}

TEST(CardSetGeneratorsTest, CanonSetSizes)
{
    EXPECT_EQ(13, pokerstove::createCardSet(1, Card::SUIT_CANONICAL).size());
    EXPECT_EQ(169, pokerstove::createCardSet(2, Card::SUIT_CANONICAL).size());
    EXPECT_EQ(1755, pokerstove::createCardSet(3, Card::SUIT_CANONICAL).size());
}

TEST(CardSetGeneratorsTest, RankSetSizes)
{
    EXPECT_EQ(13, pokerstove::createCardSet(1, Card::RANK).size());
    EXPECT_EQ(91, pokerstove::createCardSet(2, Card::RANK).size());
    EXPECT_EQ(455, pokerstove::createCardSet(3, Card::RANK).size());
}
