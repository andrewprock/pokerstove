#include "OmahaHighHandEvaluator.h"
#include <gtest/gtest.h>
#include <iostream>

using namespace pokerstove;
using namespace std;

TEST(OmahaHighHandEvaluator, Construct)
{
    OmahaHighHandEvaluator eval;
    EXPECT_EQ(true, eval.usesSuits());
    EXPECT_EQ(5, eval.boardSize());
}

TEST(OmahaHighHandEvaluator, RankEval)
{
    OmahaHighHandEvaluator oeval;
    CardSet hand("2c3cKhQd");
    CardSet board("2c3c4c");
    PokerEvaluation eval = oeval.evaluateRanks(hand, board);
    EXPECT_EQ(TWO_PAIR, eval.type());
    EXPECT_EQ(Rank("3"), eval.majorRank());
    EXPECT_EQ(Rank("2"), eval.minorRank());
}
