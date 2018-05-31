/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 * $Id: Card.cpp 2649 2012-06-30 04:53:24Z prock $
 */
#include "card.h"

#include <iostream>

#include "rank.h"
#include "suit.h"

using pokerstove::Card;
using pokerstove::Rank;
using pokerstove::Suit;

Card::Card() : _card(0) {}

Card::Card(const std::string& str) : _card(0) { fromString(str); }

Card::Card(const Rank& r, const Suit& s) : _card(encode(r, s)) {}

Card::Card(uint8_t cindex) : _card(cindex) {}

Rank Card::rank() const { return Rank(_card % Rank::NUM_RANK); }

Suit Card::suit() const { return Suit(_card / Rank::NUM_RANK); }

std::string Card::str() const {
  static const std::string cardstrings =
      "2c3c4c5c6c7c8c9cTcJcQcKcAc"
      "2d3d4d5d6d7d8d9dTdJdQdKdAd"
      "2h3h4h5h6h7h8h9hThJhQhKhAh"
      "2s3s4s5s6s7s8s9sTsJsQsKsAs";
  return cardstrings.substr(_card * 2, 2);
}

bool Card::fromString(const std::string& str) {
  if (Rank::isRankChar(str[0]) && Suit::isSuitChar(str[1])) {
    _card = encode(Rank(str), Suit(str.substr(1)));
    return true;
  }
  return false;
}

int Card::code() const { return _card; }

uint8_t Card::encode(Rank r, Suit s) {
  return r.code() + s.code() * Rank::NUM_RANK;
}

bool Card::operator<(const Card& r) const {
  if ((_card % Rank::NUM_RANK) == (r._card % Rank::NUM_RANK))
    return _card / Rank::NUM_RANK < r._card / Rank::NUM_RANK;
  return _card % Rank::NUM_RANK < r._card % Rank::NUM_RANK;
}
