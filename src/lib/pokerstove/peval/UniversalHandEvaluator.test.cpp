#include "UniversalHandEvaluator.h"
#include <gtest/gtest.h>
#include <iostream>

using namespace pokerstove;
using namespace std;

TEST(UniversalHandEvaluator, Construct)
{
    UniversalHandEvaluator eval(4,4,3,5,2,&CardSet::evaluateHigh, NULL);
    EXPECT_EQ(true, eval.usesSuits());
    EXPECT_EQ(5, eval.boardSize());
}

TEST(UniversalHandEvaluator, FiveCardOmaha)
{
    UniversalHandEvaluator oeval(5,5,3,5,2,&CardSet::evaluateHigh, NULL);
    CardSet hand("2d3dKhQd8h");
    CardSet board("2c3c4c");
    PokerEvaluation eval = oeval.eval(hand, board);
    EXPECT_EQ(TWO_PAIR, eval.type());
    EXPECT_EQ(Rank("3"), eval.majorRank());
    EXPECT_EQ(Rank("2"), eval.minorRank());
}

TEST(UniversalHandEvaluator, SixCardOmaha)
{
    UniversalHandEvaluator oeval(6,6,3,5,2,&CardSet::evaluateHigh, NULL);
    CardSet hand("2d3dKhQd8h8d");
    CardSet board("2c3c4c");
    PokerEvaluation eval = oeval.eval(hand, board);
    EXPECT_EQ(TWO_PAIR, eval.type());
    EXPECT_EQ(Rank("3"), eval.majorRank());
    EXPECT_EQ(Rank("2"), eval.minorRank());
}
