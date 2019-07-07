/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 * $Id: Card.h 2649 2012-06-30 04:53:24Z prock $
 */
#ifndef PEVAL_CARD_H_
#define PEVAL_CARD_H_

#include <cstdint>
#include <string>

namespace pokerstove
{
// forward declarations,
class Suit;
class Rank;

/**
 * This card class is designed to make the manipulation and
 * abstraction of cards as friendly as possible.
 *
 * The Card class is an external representation, and there is no
 * guarantee that Card objects are used internally in the library.
 * The goal of the representation issues which may cause portability
 * and translation problems.  They also store some partial evals.
 *
 * transforming suits to strings can be controlled so that the string
 * generated is suitable for the display format.  This does *not*
 * apply to string parsing.  Strings may only be parsed from the set
 * of characters: {cdhs}
 */
class Card
{
public:
    enum Grouping
    {
        RANK_SUIT,
        SUIT_CANONICAL,
        RANK,
    };

    Card();
    explicit Card(const std::string& str);
    explicit Card(const Rank& r, const Suit& s);

    /**
     * use of this constructor is discouraged, the safest use is to
     * (re)construct cards from saved codes.
     */
    explicit Card(uint8_t cindex);

    Rank rank() const;
    Suit suit() const;

    /**
     * use of this method is discouraged
     */
    int code() const;

    /**
     * string representation of card,
     * @see Suit::display
     * @see Suit::setSuitStringType()
     */
    std::string str() const;

    /**
     * provide checked parsing of the first two chars of the input string.
     */
    bool fromString(const std::string& s);

    /**
     * required for storage in STL containers
     */
    bool operator==(const Card& r) const { return _card == r._card; }

    /**
     * required for storage in STL containers
     */
    bool operator<(const Card& r) const;

private:
    static uint8_t encode(Rank r, Suit s);

    friend class CardSet;

    uint8_t _card;
};
}  // namespace pokerstove

#endif  // PEVAL_CARD_H_
