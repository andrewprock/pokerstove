/**
 * Copyright (c) 2013 Andrew Prock. All rights reserved.
 */
#include "CardSetGenerators.h"
#include "Card.h"
#include <pokerstove/util/combinations.h>

namespace pokerstove
{

std::set<CardSet>
createCardSets(size_t numCards)
{
    std::set<CardSet> ret;
    combinations cards(52,numCards);
    do
    {
        CardSet hand;
        for (size_t i=0; i<numCards; i++)
        {
            hand.insert (Card(cards[i]));
        }
        ret.insert(hand);
    }
    while (cards.next());
    return ret;
}

std::set<CardSet>
createCanonSets(size_t numCards)
{
    std::set<CardSet> ret;
    combinations cards(52,numCards);
    do
    {
        CardSet hand;
        for (size_t i=0; i<numCards; i++)
        {
            hand.insert (Card(cards[i]));
        }
        ret.insert(hand.canonize());
    }
    while (cards.next());
    return ret;
}

std::set<CardSet>
createRankSets(size_t numCards)
{
    std::set<CardSet> ret;
    combinations cards(52,numCards);
    do
    {
        CardSet hand;
        for (size_t i=0; i<numCards; i++)
        {
            hand.insert (Card(cards[i]));
        }
        ret.insert(hand.canonizeRanks());
    }
    while (cards.next());
    return ret;
}

}
