/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 */
#ifndef PEVAL_CARDSETGENERATORS_H_
#define PEVAL_CARDSETGENERATORS_H_

#include <set>
#include "CardSet.h"

namespace pokerstove
{

/**
 * Create the set of all card sets where the size of each rank set has
 * numCards.  
 *
 * For example, with an argument of 2, the set of all 52c2 two card
 * combinations will be generated.
 */
std::set<CardSet> createCardSets(size_t numCards);

/**
 * Create the set of all canonical card sets where the size of each rank
 * set has numCards.  These cards are all set to the canonical suit set for
 * the given suit combination, suits matter, but suit permutations do not.
 *
 * For example, with an argument of 2, the set of all 13c2+13=169 two card
 * combinations will be generated.  These are the same as the 169 preflop
 * hold'em hand types, but with arbitrarily assigned suits.  For example
 * AQs is represented by AcQc
 */
std::set<CardSet> createCanonSets(size_t numCards);

/**
 * Create the set of all rank sets where the size of each rank set has
 * numCards.  
 *
 * For example, with an argument of 2, a set of size 91 will be generated,
 * with each element representing a CardSet with two cards, set to the
 * canonical rank representation.
 *
 * See also, CardSet::canonizeRanks()
 */
std::set<CardSet> createRankSets(size_t numCards);

}

#endif  // PEVAL_CARDSETGENERATORS_H_
