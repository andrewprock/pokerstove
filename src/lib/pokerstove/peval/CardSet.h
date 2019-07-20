/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 * $Id: CardSet.h 2649 2012-06-30 04:53:24Z prock $
 */
#ifndef PEVAL_CARDSET_H_
#define PEVAL_CARDSET_H_

#include <cstdint>
#include <iosfwd>
#include <string>
#include <vector>
#include "Rank.h"
#include "Suit.h"

namespace pokerstove
{
// forward declares
class Card;
class PokerEvaluation;

const size_t STANDARD_DECK_SIZE = Rank::NUM_RANK * Suit::NUM_SUIT;

/**
 * The CardSet is a compact representation of an unordered set of
 * cards.  All evaluation is done at the CardSet level.
 *
 * It can be used to store set of cards such as seen cards, dead
 * cards, folded cards, door cards, and whatever generic set of cards
 * you may be interested in.
 *
 * CardSet is implemented with efficiency in mind, so when selecting
 * from representations for cards, use a CardSet if speed and size
 * efficiency is important.  Other containers may be less efficient in
 * terms of size, speed, or both.
 */
class CardSet
{
public:
public:
    CardSet();                                              //!< defaults to the empty set
    CardSet(const CardSet& cs);                             //!< copy constructor (default)
    CardSet(const std::string& c);                          //!< parse cards untill fail
    explicit CardSet(const Card& c);                        //!< create a set with one card
    explicit CardSet(uint64_t mask) : _cardmask(mask) {}

    void clear() { _cardmask = 0; }                         //!< empty the set
    void fill() { _cardmask = ~(0xffffffffffffffff << STANDARD_DECK_SIZE); } //!< put all cards into the set
    size_t size() const;                                    //!< return number of cards in set

    uint64_t mask() const { return _cardmask; }             //!< 1 bit per card

    std::vector<Card> cards() const;                        //!< break into Cards
    std::vector<CardSet> cardSets() const;                  //!< break into one card/CardSet

    /**
     * Card related methods.
     */
    CardSet& insert(const Card& c);                         //!< add one card
    CardSet& insert(const CardSet& c);                      //!< equivalent to |=
    CardSet& remove(const Card& c);                         //!< remove a card
    CardSet& remove(const CardSet& c);
    bool contains(const Card& c) const;                     //!< is a card in the set
    bool contains(const CardSet& c) const;                  //!< is a card in the set
    bool disjoint(const CardSet& c) const { return (_cardmask & c._cardmask) == 0; }
    bool intersects(const CardSet& c) const { return !disjoint(c); }

    /**
     * Rank related methods.
     */
    size_t  countRanks() const;
    size_t  count(const Rank& r) const;
    bool    contains(const Rank& r) const;
    Card    find(const Rank& r) const;          //!< return some card with rank r, in suit order
    int     rankMask() const;                   //!< one bit set for each rank in hand, 13 max
    bool    hasStraight() const;
    Rank    topRank() const;                    //!< return the highest rank in hand
    Rank    bottomRank() const;                 //!< return lowest rank in hand (A plays high)
    size_t  countMaxRank() const;               //!< return the number of the most common rank
    bool    insertRanks(const CardSet& rset);   //!< add ranks to hand, any suit
    CardSet canonizeRanks() const;              //!< create a canonical rank representation, output suits are to be ignored

    /**
     * Suit related methods.
     */
    size_t  countSuits() const;                             //!< number of different suits in hand
    size_t  count(const Suit& s) const;                     //!< returns the length of the specified suit
    size_t  countMaxSuit() const;                           //!< returns the length of the longest suit
    bool    contains(const Suit& s) const;
    Rank    flushRank(const Suit& s) const;                 //!< return the highest rank of specified suit
    int     suitMask(const Suit& s) const;
    CardSet canonize() const;                               //!< transform suits to canonical form
    CardSet canonize(const CardSet& other) const;           //!< canonize relative to other hand
    CardSet rotateSuits(int c, int d, int h, int s) const;  //!< [0..4] for each suit's new suit
    void    flipSuits();                                    //!< invert suit order {cdhs} -> {shdc}

    /**
     * String conversions.  Note that the output produced depends on
     * Suit::setSuitStringType (Suit::display s).  Not all string types are
     * reparseable.  That is:
     *
     * CardSet (CardSet("Ac").str()) == CardSet("Ac")
     *
     * May not be true.
     *
     * It is guaranteed to be true if the Suit::display type is SUIT_ASCII
     */
    std::string str() const;
    std::string rankstr() const;                            //!< print sorted ranks with dupes
    std::string toRankBitString() const;

    /**
     * indexing utils
     */
    size_t colex() const;      //!< return a unique number based on cards
    size_t rankColex() const;  //!< return a unique number based on ranks

    /**
     * These are the basic building blocks of evaluation, they should
     * be fairly fast, but general, note there is a chance some of
     * these may break if given degenerate sets (with no cards, or
     * more than 7 cards) Every evaluation is ordered such that
     * (better hand) > (worse hand).  This holds even for (esp for)
     * lowball.  There is also a code which represents an null hand,
     * specifcally for the case of not making a qualifying 8 low.
     *
     * There is a tradeoff putting them here, by keeping them here we
     * preserve encapsulation but clutter up the class.  It might be
     * better to separate these out into utility functions.
     * e.g. PokerEvaluation evaluateHigh(const CardSet& c);
     */
    PokerEvaluation evaluateHigh() const;
    PokerEvaluation evaluateHighRanks() const;
    PokerEvaluation evaluateHighFlush() const;
    // PokerEvaluation evaluateHighThreeCard() const;
    PokerEvaluation evaluateLowA5() const;
    PokerEvaluation evaluate8LowA5() const;
    PokerEvaluation evaluateLow2to7() const;
    PokerEvaluation evaluateRanksLow2to7() const;
    PokerEvaluation evaluateSuitsLow2to7() const;
    PokerEvaluation evaluate3CP() const;
    PokerEvaluation evaluateBadugi() const;
    PokerEvaluation evaluatePairing() const;

    // sub evaluations

    /** return the number of outs to complete a straight
     * returns 8 for open ended
     * returns 4 for gutshot
     * returns 1 for runner runner
     */
    int evaluateStraightOuts() const;

    // overloaded operators, syntactic sugar. It is no secret that
    // this is a bitset, so exposing bit operations should be ok.
    // one thing that should be considered is implementing the &=, etc
    // operators for efficiency and generality using boost::opperators
    void    operator|=(const CardSet& c) { _cardmask |= c._cardmask; }
    void    operator^=(const CardSet& c) { _cardmask ^= c._cardmask; }
    bool    operator==(const CardSet& c) const { return _cardmask == c._cardmask; }
    bool    operator!=(const CardSet& c) const { return _cardmask != c._cardmask; }
    bool    operator<(const CardSet& c) const { return _cardmask < c._cardmask; }
    bool    operator>(const CardSet& c) const { return _cardmask > c._cardmask; }
    CardSet operator&(const CardSet& c) const { return CardSet(_cardmask & c._cardmask); }
    CardSet operator|(const CardSet& c) const { return CardSet(_cardmask | c._cardmask); }
    CardSet operator^(const CardSet& c) const { return CardSet(_cardmask ^ c._cardmask); }

    void swap(CardSet& c)
    {
        uint64_t t = c._cardmask;
        c._cardmask = _cardmask;
        _cardmask = t;
    }

protected:
    void
    fromString(const std::string& s);   //!< throws exception on parse failre
    bool isPaired() const;              //!< returns true if *any* two cards match rank
    bool isTripped() const;             //!< returns true if trips

private:
    //!< bit mask of cards in "canonical" order. [2c,3c ... Ac,Ad ... Ah ... Qs,Ks,As]
    uint64_t _cardmask;
};

////////////////////////////////////////////////////////////////////////////////
// Below are standalone methods related to CardSet objects.  They should
// probably be moved to a separate file as they don't directly manipulate
// CardSet data.
////////////////////////////////////////////////////////////////////////////////

/**
 * cannonize a hand relative to a board
 */
CardSet canonizeToBoard(const CardSet& board, const CardSet& hand);

std::vector<int> findSuitPermutation(const CardSet& source, const CardSet& dest);

}  // namespace pokerstove

/**
 * our little printer
 */
inline std::ostream& operator<<(std::ostream& sout, const pokerstove::CardSet & e)
{
  sout << e.str();
  return sout;
}

#endif  // PEVAL_CARDSET_H_
