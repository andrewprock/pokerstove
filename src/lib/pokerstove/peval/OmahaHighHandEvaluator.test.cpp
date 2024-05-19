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

TEST(OmahaHighHandEvaluator, SplitPot)
{
    OmahaHighHandEvaluator eval;
    // both hands should evaluate to broadway
    CardSet c1("2c3cKhQd");
    CardSet c2("4c5cAhTc");
    CardSet board("AcKcQhJsTd");
    EXPECT_EQ(eval.evaluateEquity(c1, c2, board), 0.5);
}