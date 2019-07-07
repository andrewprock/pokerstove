/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 * $Id: Rank.h 2649 2012-06-30 04:53:24Z prock $
 */
#ifndef PEVAL_RANK_H_
#define PEVAL_RANK_H_

#include <cstdint>
#include <string>

namespace pokerstove
{
/**
 * A rank class.  We take great care to manage the type to limit the
 * values of Ranks that can be created.  There is some leakage in
 * the fact that certain classes have to be friends to use the more
 * powerfull methods.
 */
class Rank
{
public:
    static const unsigned int NUM_RANK = 13;
    static const unsigned int cardianlity = 13;

    // manage the values of possible ranks strictly
    static const Rank Two()   { return Rank(RANK_TWO); }
    static const Rank Three() { return Rank(RANK_THREE); }
    static const Rank Four()  { return Rank(RANK_FOUR); }
    static const Rank Five()  { return Rank(RANK_FIVE); }
    static const Rank Six()   { return Rank(RANK_SIX); }
    static const Rank Seven() { return Rank(RANK_SEVEN); }
    static const Rank Eight() { return Rank(RANK_EIGHT); }
    static const Rank Nine()  { return Rank(RANK_NINE); }
    static const Rank Ten()   { return Rank(RANK_TEN); }
    static const Rank Jack()  { return Rank(RANK_JACK); }
    static const Rank Queen() { return Rank(RANK_QUEEN); }
    static const Rank King()  { return Rank(RANK_KING); }
    static const Rank Ace()   { return Rank(RANK_ACE); }

    // standalone fast parsting
    static int rank_code(char c);

    /**
     * Default constructor initializes suit to Rank::Two()
     */
    Rank() : _rank(RANK_TWO) {}

    /**
     * Create from input string.
     * @see fromString
     */
    explicit Rank(const std::string& str);

    /**
     * This is a bad interface, use at your own risk.  The reason this is
     * bad is because:
     * - breaks encapsulation, you need to know how the rank is encoded
     * - conflates ints and chars, and can cause weird things to happen
     *
     * To mitigate this, the input value is interpreted differently for
     * different values.  In the range of [0,12] it is interprted as an
     * int.  If it is the ascii value for a character in the string:
     * "23456789TJQKAtjqka", it is interprted as a char.
     *
     * Because ascii code 50 is '2' and 51 is '3', the input code cannot
     * represent arbitrary cards in the range of a 52 card deck.  Thus
     * integer interpretations are strictly tied to rank.
     *
     * In the case that the input value is not in the ranges given above,
     * the code is set to an invalid code, possibly 0xFF
     */
    explicit Rank(uint8_t c);

    /**
     * Encode rank as a string.  A string of length one from [2-9TJQKA]
     * will be returned.
     */
    std::string str() const;

    /**
     * Parse from input string. Only the first letter of the input
     * string is examined, and must be in [2-9tTjJqQkKaA] for a valid
     * Rank object to be created.
     */
    void fromString(const std::string& s);

    /**
     * Give access to the operators that one might use if one knew the
     * encoding without requiring actual knowledge of the encoding.
     * These operators are guaranteed to work consistendtly based on the
     * relative ordering: 2<3<4<5<6<7<8<9<T<J<Q<K<A.
     * note that these operators always assume that Ace is high.
     */
    bool operator==(const Rank& r) const { return _rank == r._rank; }
    bool operator<=(const Rank& r) const { return _rank <= r._rank; }
    bool operator< (const Rank& r) const { return _rank <  r._rank; }
    bool operator> (const Rank& r) const { return _rank >  r._rank; }
    void operator++()                    { ++_rank; }
    void operator--()                    { --_rank; }

private:
    /**
     * We break encapsulation with these private methods.  The
     * semantics are that the card rank is encoded as
     * cardcode%NUM_RANK.  We should do our best to limit/remove these
     * functions.  They are declared in the header as a reminder that
     * they break encapsulation, and that they should be used only as
     * a last resort.  Probably the best solution is to limit access
     * to these functions to friends.
     */
    void    encode(uint8_t c) { _rank = c%NUM_RANK; }          // was encode
    uint8_t code() const      { return _rank; }
    int     rankBit() const   { return 0x01 << _rank; }

    static bool isRankChar(char c);

    friend class Card;
    friend class CardSet;
    friend class PokerEvaluation;

    uint8_t _rank;

    static uint8_t TwoVal()   { return  0; }
    static uint8_t ThreeVal() { return  1; }
    static uint8_t FourVal()  { return  2; }
    static uint8_t FiveVal()  { return  3; }
    static uint8_t SixVal()   { return  4; }
    static uint8_t SevenVal() { return  5; }
    static uint8_t EightVal() { return  6; }
    static uint8_t NineVal()  { return  7; }
    static uint8_t TenVal()   { return  8; }
    static uint8_t JackVal()  { return  9; }
    static uint8_t QueenVal() { return 10; }
    static uint8_t KingVal()  { return 11; }
    static uint8_t AceVal()   { return 12; }

    static const int RANK_TWO          = 0;
    static const int RANK_THREE        = 1;
    static const int RANK_FOUR         = 2;
    static const int RANK_FIVE         = 3;
    static const int RANK_SIX          = 4;
    static const int RANK_SEVEN        = 5;
    static const int RANK_EIGHT        = 6;
    static const int RANK_NINE         = 7;
    static const int RANK_TEN          = 8;
    static const int RANK_JACK         = 9;
    static const int RANK_QUEEN        =10;
    static const int RANK_KING         =11;
    static const int RANK_ACE          =12;
};

}

#endif  // PEVAL_RANK_H_
