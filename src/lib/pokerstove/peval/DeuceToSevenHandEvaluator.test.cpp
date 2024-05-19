#include "DeuceToSevenHandEvaluator.h"
#include <gtest/gtest.h>
#include <iostream>

using namespace pokerstove;
using namespace std;

TEST(DeuceToSevenEval, Construct)
{
    DeuceToSevenHandEvaluator eval;
    EXPECT_EQ(true, eval.usesSuits());
    EXPECT_EQ(0, eval.boardSize());
}

TEST(DeuceToSevenEval, NutLow)
{
    DeuceToSevenHandEvaluator eval;
    CardSet c1("2c3c4c5c7s");
    CardSet c2("2c3c4c5c7c"); // flush
    string s1 = eval.evaluate(c1).str();
    string s2 = eval.evaluate(c2).str();

    // nut low is better than a flush
    EXPECT_EQ(eval.evaluateEquity(c1, c2), 1.0);
}

TEST(DeuceToSevenEval, AceHigh)
{
    DeuceToSevenHandEvaluator eval;
    CardSet h3("2c3c4c5cAs"); // NOT a straight
    CardSet h4("2c3c4c5c5s"); // one pair
    EXPECT_GT(h3, h4);

}
