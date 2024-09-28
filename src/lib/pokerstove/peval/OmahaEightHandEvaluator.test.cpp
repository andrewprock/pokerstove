#include "OmahaEightHandEvaluator.h"
#include <gtest/gtest.h>
#include <iostream>

using namespace pokerstove;
using namespace std;

TEST(OmahaEightHandEvaluator, Construct)
{
    OmahaEightHandEvaluator eval;
    EXPECT_EQ(true, eval.usesSuits());
    EXPECT_EQ(5, eval.boardSize());
}

TEST(OmahaEightHandEvaluator, NoBoard)
{
    OmahaEightHandEvaluator oeval;
    // KQ high and 44, with no low
    CardSet c1("2c3cKhQd");
    CardSet c2("4c5c4hTc");
    CardSet board;

    int KQbits = 0x01 << Rank::rank_code('K') | 0x01 << Rank::rank_code('Q');
    EXPECT_EQ(0, oeval.evaluateHand(c1, board).low().code());
    EXPECT_EQ(NO_PAIR, oeval.evaluateHand(c1, board).eval().type());
    EXPECT_EQ(KQbits, oeval.evaluateHand(c1, board).eval().kickerBits());

    EXPECT_EQ(0, oeval.evaluateHand(c2, board).low().code());
    EXPECT_EQ(ONE_PAIR, oeval.evaluateHand(c2, board).eval().type());
    EXPECT_EQ(Rank::Four(), oeval.evaluateHand(c2, board).eval().majorRank());
}

TEST(OmahaEightHandEvaluator, OneCardBoard)
{
    OmahaEightHandEvaluator oeval;
    // KQ high and 44, with no low
    CardSet c1("2c3cKhQd");
    CardSet c2("4c5c4hTc");
    CardSet board("Ks");

    int KQbits = 0x01 << Rank::rank_code('K') | 0x01 << Rank::rank_code('Q');
    EXPECT_EQ(0, oeval.evaluateHand(c1, board).low().code());
    EXPECT_EQ(ONE_PAIR, oeval.evaluateHand(c1, board).eval().type());
    EXPECT_EQ(Rank::King(), oeval.evaluateHand(c1, board).eval().majorRank());

    EXPECT_EQ(0, oeval.evaluateHand(c2, board).low().code());
    EXPECT_EQ(ONE_PAIR, oeval.evaluateHand(c2, board).eval().type());
    EXPECT_EQ(Rank::Four(), oeval.evaluateHand(c2, board).eval().majorRank());
}

TEST(OmahaEightHandEvaluator, SplitPot)
{
    OmahaEightHandEvaluator eval;
    // both hands should evaluate to broadway
    CardSet c1("2c3cKhQd");
    CardSet c2("4c5cAhTc");
    CardSet board("AcKcQhJsTd");
    EXPECT_EQ(eval.evaluateEquity(c1, c2, board), 0.5);
}

TEST(OmahaEightHandEvaluator, SplitHighLow)
{
    OmahaEightHandEvaluator eval;
    // First hand should get low, second high
    CardSet c1("4c3cKhQd");
    CardSet c2("4h5cQhTc");
    CardSet board("AcKc6cJs2d");
    EXPECT_EQ(eval.evaluateEquity(c1, c2, board), 0.5);
}

TEST(OmahaEightHandEvaluator, ThreeQuartered)
{
    OmahaEightHandEvaluator eval;
    // First hand should split high, win low
    CardSet c1("4c3sKhQd");
    CardSet c2("4h5cQhKh");
    CardSet board1("Ac2d6cKcJs");
    EXPECT_EQ(eval.evaluateEquity(c1, c2, board1), 0.75);

    // First hand should split low, lose high
    CardSet c3("4h3dQhTc");
    CardSet c4("4c3cKhQd");
    CardSet board2("Ac2d6cKcJs");
    EXPECT_EQ(eval.evaluateEquity(c3, c4, board2), 0.25);
}