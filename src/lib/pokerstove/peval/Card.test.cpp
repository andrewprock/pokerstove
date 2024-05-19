#include "Card.h"
#include "Rank.h"
#include "Suit.h"
#include <gtest/gtest.h>

using namespace pokerstove;

TEST(CardTest, constructors)
{
    EXPECT_EQ(1,1);

    Card c1("Ac");
    EXPECT_EQ(c1.str(), "Ac");

    Card c2(Rank::Ace(), Suit::Clubs());
    EXPECT_EQ(c1, c2);

    Card c3(12);
    EXPECT_EQ(c1, c3);
}