/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 * $Id: PokerHand.h 2649 2012-06-30 04:53:24Z prock $
 */
#ifndef PEVAL_POKERHAND_H_
#define PEVAL_POKERHAND_H_

#include <array>
#include "Card.h"
#include "CardSet.h"

#define MUTABLE

/*! \namespace
 * A namespace to wrap up all of the poker evaluation code.
 */
namespace pokerstove
{
  // we might possibly template the size here for performance
  // reasons, although if performance is an issue, it's probably
  // better to use a CardSet
  const int MAX_PHCARDS = 13;

  /**
   * A PokerHand is an ordered collection of cards.  The size of
   * a standard PokerHand is more limited to MAX_PHCARDS instead of
   * being allowed to be hold a full deck.
   *
   * This representation of cards is generally slow.  If speed is of
   * primary concern, a CardSet object should be used, either by
   * conversion (cardSet method) or in place of a PokerHand.
   */
  class PokerHand
  {
  public:
    PokerHand ();

    /**
     * Create a PokerHand from a CardSet.  Since a CardSet is not
     * ordered, the induced order of the cards is arbitrary.
     */
    explicit PokerHand (const CardSet & s);

    /**
     * Create a PokerHand by parsing the input string.  The order of
     * the Cards will be the same as in the input string.
     * @see Rank::Rank(const std::string & s)
     * @see Suit::Suit(const std::string & s)
     */
    explicit PokerHand (const std::string & s);

    /**
     * remove all cards
     */
    void clear ();

    /**
     * create the string for the PokerHand.
     * @see Suit::display
     * @see Suit::setSuitStringType()
     */
    std::string str () const;

    /**
     * create the hold'em preflop string (AA, AKo, QJs, etc...)
     */
    std::string preflopstr () const;

    size_t  size () const;                               //!< return the number of cards in hand
    CardSet cardSet () const;                         //!< sometimes we want to work with the compact set
    CardSet cardSet (size_t first, size_t len) const; //!< return a subset of the cards
    std::vector<Card> cards () const;                 //!< extract cards in generic format

    bool contains (const Card& c) const;

    /**
     * Append Card to hand.  Duplicates not appended.
     */
    void append  (const Card & c);

    /**
     * Append Cards in CardSet to hand.  Order appended is arbitrary. Duplicates not appended.
     */
    void append  (const CardSet& cs);

    /**
     * Append Cards in PokerHand order.  Duplicates not appended.
     */
    void append  (const PokerHand & h);

    /**
     * If Card is in hand, remove it.  Shift cards to fill in hole.  O(size)
     */
    void remove  (const Card& c);

    /**
     * If Card is in hand, remove it.  Shift cards to fill in hole.  O(this.size*cs.size)
     */
    void remove  (const CardSet& cs);

    /**
     * If Card is in hand, remove it.  Shift cards to fill in hole.  O(this.size*h.size)
     */
    void remove  (const PokerHand & h);

    const Card& operator[] (size_t index) const;        //!< get const card by index,
          Card& operator[] (size_t index);              //!< get card by index,

    /**
     * Re-order the hand. This changes the order, but are considered
     * const since ONLY the order of the cards change.
     */
#ifdef MUTABLE
    void pushCardToFront (size_t nth) const;
    void sort () const;
    void sortRanks () const;
    void sortEval () const;
#else
    PokerHand sortSuits () const;
    PokerHand sortRanks () const;
    PokerHand sortEval () const;
#endif

  protected:
    /**
     * Parse a poker hand from a string.  All card tokens found in the entire string will
     * be added to the hand.  This allows for embeded spaces, or other embeded characters.
     * Strings of zero length will create an empty hand, strings of length 1 will raise an
     * exception.
     */
    void fromString (const std::string & s);


  private:
#ifdef MUTABLE
    mutable  //!< _cards can be mutable to handle sorts, better solution?
#endif
    std::array<Card, MAX_PHCARDS> _cards;

    uint8_t _ncards;
};

}  // namespace pokerstove

inline std::ostream& operator<<(std::ostream& sout, const pokerstove::PokerHand & h)
{
  sout << h.str();
  return sout;
}

#endif  // PEVAL_POKERHAND_H_
