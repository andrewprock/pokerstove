#include "HoldemHandEvaluator.h"
#include <gtest/gtest.h>
#include <iostream>

using namespace pokerstove;
using namespace std;

TEST(HoldemHandEvaluator, Construct)
{
    HoldemHandEvaluator eval;
    EXPECT_EQ(true, eval.usesSuits());
    EXPECT_EQ(5, eval.boardSize());
}

TEST(HoldemHandEvaluator, RankEval)
{
    HoldemHandEvaluator heval;
    CardSet hand("2c3c");
    CardSet board("2c3c4c");
    PokerEvaluation eval = heval.evaluateRanks(hand, board);
    EXPECT_EQ(TWO_PAIR, eval.type());
    EXPECT_EQ(Rank("3"), eval.majorRank());
    EXPECT_EQ(Rank("2"), eval.minorRank());
}
