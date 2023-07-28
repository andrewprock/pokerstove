#include "SimpleDeck.hpp"

#include <gtest/gtest.h>

using namespace pokerstove;
using namespace std;

TEST(SimpleDeck, tautology) { EXPECT_EQ(1, 1); }

TEST(SimpleDeck, remove_card)
{
    SimpleDeck d;
    EXPECT_EQ(d.size(), 52);

    CardSet clubThree = CardSet("3c");
    CardSet clubAce = CardSet("Ac");
    CardSet fullDeck;

    d.remove(clubThree);
    EXPECT_EQ(d.size(), 51);
    EXPECT_EQ(d.dead(), clubThree);

    d.remove(clubAce);
    EXPECT_EQ(d.size(), 50);
    EXPECT_EQ(d.dead().size(), 2);
    EXPECT_EQ(d.dead(), clubThree | clubAce);
    EXPECT_FALSE(d.deal(50).contains(clubAce));
}

TEST(SimpleDeck, shuffe)
{
    SimpleDeck shuffled;
    SimpleDeck unshuffled;

    shuffled.shuffle();

    EXPECT_EQ(shuffled.size(), unshuffled.size());

    // check that the first 26 cards are different.
    // there is some very small fininte chance this fails
    uint64_t shuffled26 = shuffled.deal(26).mask();
    uint64_t unshuffled26 = unshuffled.deal(26).mask();
    EXPECT_NE(shuffled26, unshuffled26);

    // pull the rest of the cards out of the deck
    // we should have a full deck
    shuffled26 |= shuffled.deal(26).mask();
    unshuffled26 |= unshuffled.deal(26).mask();
    EXPECT_EQ(shuffled26, unshuffled26);
}
