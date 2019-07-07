/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 */
#ifndef PEVAL_CARDSETGENERATORS_H_
#define PEVAL_CARDSETGENERATORS_H_

#include "Card.h"
#include "CardSet.h"
#include <set>

namespace pokerstove
{

/**
 * Create the set of all card sets where the size of each rank set has
 * numCards.
 *
 * Card::Grouping determines what sort of set is produced.
 *
 * Card::RANK_SUIT: This is the usual set of cards.  For example, with an
 * argument of 2, the set of all 52c2 two card combinations will be
 * generated.
 *
 * Card::SuitCanonical: Here suits are mapped to a canonical
 * representation.  With an argument of 2, the set of all 13c2+13=169 two
 * card combinations will be generated.  These are the same as the 169
 * preflop hold'em hand types, but with arbitrarily assigned suits.  For
 * example AQs is represented by AcQc
 *
 * Card::Rank: The set of cards with no suit information.  For example,
 * with an argument of 2, a set of size 91 will be generated, with each
 * element representing a CardSet with two cards, set to the canonical rank
 * representation.
 */
std::set<CardSet> createCardSet(size_t numCards,
                                Card::Grouping grouping = Card::RANK_SUIT);

}  // namespace pokerstove

#endif  // PEVAL_CARDSETGENERATORS_H_
