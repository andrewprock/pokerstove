/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 * $Id$
 */
#ifndef PENUM_SIMPLE_DECK_H_
#define PENUM_SIMPLE_DECK_H_

#include <algorithm>
#include <array>
#include <functional>
#include <pokerstove/peval/Card.h>
#include <pokerstove/peval/CardSet.h>
#include <pokerstove/peval/Rank.h>  // needed for NUM_RANK
#include <pokerstove/peval/Suit.h>  // needed for NUM_SUIT
#include <pokerstove/util/lastbit.h>
#include <string>

namespace pokerstove
{

/**
 * used for removing cards from the deck
 */
struct isLive : public std::binary_function<pokerstove::CardSet,
                                            pokerstove::CardSet,
                                            bool>
{
    bool operator()(const CardSet& c, const CardSet& dead) const
    {
        return !dead.contains(c);
    }
};

/**
 * A very simple deck of the cards.
 */
class SimpleDeck
{
public:
    /**
     * construct a deck that is in-order
     */
    SimpleDeck()
    {
        for (uint8_t i = 0; i < STANDARD_DECK_SIZE; i++)
        {
            _deck[i] = CardSet(Card(i));
        }
        reset();
    }

    /**
     * put all dealt cards back into deck, don't reorder
     */
    void reset() { _current = STANDARD_DECK_SIZE; }

    /**
     * number of cards left in the deck
     */
    size_t size() const { return _current; }

    /**
     * print cards in deck with un/dealt divider
     */
    std::string str() const
    {
        std::string ret;
        for (uint i = 0; i < STANDARD_DECK_SIZE; i++)
        {
            if (i == _current)
                ret += "/";
            ret = ret + _deck[i].str();
        }
        if (_current == STANDARD_DECK_SIZE)
            ret += "/";
        return ret;
    }

    pokerstove::CardSet deal(size_t ncards)
    {
        // TODO: fix and test this code, edge cases clearly at risk here
        if (ncards == 0)
            return pokerstove::CardSet();
        _current -= static_cast<uint>(ncards);
        CardSet* pcur = &_deck[_current];
        const CardSet* pend = pcur + ncards;
        CardSet cards(*pcur++);
        while (pcur < pend)
            cards |= *pcur++;
        return cards;
    }

    pokerstove::CardSet dead() const
    {
        pokerstove::CardSet cs;
        for (size_t i = _current; i < STANDARD_DECK_SIZE; i++)
            cs.insert(_deck[i]);
        return cs;
    }

    /**
     * Move all cards which are not live to the end of the deck
     */
    void remove(const pokerstove::CardSet& cards)
    {
        int decr = CardSet(cards | dead()).size();
        stable_partition(_deck.begin(), _deck.end(), bind2nd(isLive(), cards));
        _current = STANDARD_DECK_SIZE - decr;
    }

    /**
     * look at ith card from the top of the deck
     */
    CardSet operator[](size_t i) const { return _deck[i]; }

    void shuffle()
    {
        std::random_shuffle(_deck.begin(), _deck.end());
        reset();  //_current = 0;
    }

    /**
     * peek at the set of cards defined by the mask
     */
    CardSet peek(uint64_t mask) const
    {
        #ifdef WIN32
        // disable the unary negation of unsigned int
        #pragma warning(disable : 4146)
        #endif

        CardSet ret;

        uint32_t lower = static_cast<uint32_t>(mask & UINT32_C(0xFFFFFFFF));
        uint32_t upper =
            static_cast<uint32_t>((mask & UINT64_C(0xFFFFFFFF00000000)) >> 32);

        while (lower)
        {
            ret |= _deck[lastbit(lower)];
            lower ^= (lower & -lower);
        }
        const CardSet* top = &_deck[32];
        while (upper)
        {
            ret |= top[lastbit(upper)];
            upper ^= (upper & -upper);
        }

        #ifdef WIN32
        // set back to default
        #pragma warning(default : 4146)
        #endif

        return ret;
    }

private:
    // these are the data which track info about the deck
    std::array<CardSet, STANDARD_DECK_SIZE> _deck;
    size_t _current;
};
}  // namespace pokerstove

#endif  // PENUM_SIMPLE_DECK_H_
