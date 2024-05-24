#include "PokerEvaluation.h"
#include "CardSet.h"
#include <gtest/gtest.h>

using namespace pokerstove;


TEST(PokerEvaluation, High)
{
    CardSet cards("AcAs8c8hKd");    // dead man's hand

    PokerEvaluation eval = cards.evaluateHigh();
    EXPECT_EQ(eval.type(), TWO_PAIR);
    EXPECT_EQ(eval.majorRank(), Rank::Ace());
    EXPECT_EQ(eval.minorRank(), Rank::Eight());
    EXPECT_EQ(true, true);
}
