/**
 * Copyright (c) 2013 Andrew Prock. All rights reserved.
 */
#include "card_set_generators.h"

#include <pokerstove/util/combinations.h>

#include "card.h"

namespace pokerstove {

std::set<CardSet> createCardSet(size_t numCards, Card::Grouping grouping) {
  std::set<CardSet> ret;
  combinations cards(52, numCards);
  do {
    CardSet hand;
    for (size_t i = 0; i < numCards; i++) {
      hand.insert(Card(cards[i]));
    }
    switch (grouping) {
      case Card::RANK_SUIT:
        ret.insert(hand);
        break;
      case Card::SUIT_CANONICAL:
        ret.insert(hand.canonize());
        break;
      case Card::RANK:
        ret.insert(hand.canonizeRanks());
        break;
    };

  } while (cards.next());
  return ret;
}

}  // namespace pokerstove
