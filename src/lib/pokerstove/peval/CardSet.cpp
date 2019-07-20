/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 * $Id: CardSet.cpp 2649 2012-06-30 04:53:24Z prock $
 */
#include "CardSet.h"
#include "Card.h"
#include "PokerEvaluation.h"
#include "PokerEvaluationTables.h"
#include "Rank.h"
#include "Suit.h"
#include <algorithm>
#include <array>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/math/special_functions/binomial.hpp>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <limits>
#include <pokerstove/util/combinations.h>
#include <set>
#include <vector>

using namespace std;
using namespace boost;
using namespace pokerstove;
using namespace pokerstove;

// some suit mask macros
#define SMASK(n) (static_cast<int>(_cardmask >> (n)*Rank::NUM_RANK) & 0x1FFF)

#if 1
#define C() (static_cast<int>(_cardmask >> (0) * Rank::NUM_RANK) & 0x1FFF)
#define D() (static_cast<int>(_cardmask >> (1) * Rank::NUM_RANK) & 0x1FFF)
#define H() (static_cast<int>(_cardmask >> (2) * Rank::NUM_RANK) & 0x1FFF)
#define S() (static_cast<int>(_cardmask >> (3) * Rank::NUM_RANK) & 0x1FFF)
#else
#define C() SMASK(0)
#define D() SMASK(1)
#define H() SMASK(2)
#define S() SMASK(3)
#endif

#define RMASK() (C() | D() | H() | S())

#define LOWBALL_ROTATE_RANKS(ranks)                \
    ((((ranks) & ~(1 << Rank::AceVal())) << 1) |   \
     (((ranks) >> Rank::AceVal()) & 0x01))

/**
 * ctors
 */

CardSet::CardSet()
    : _cardmask(0)
{}

CardSet::CardSet(const CardSet& cs)
    : _cardmask(cs._cardmask)
{}

CardSet::CardSet(const Card& c)
    : _cardmask(ONE64 << c._card)
{}

CardSet::CardSet(const string& c)
    : _cardmask(0)
{
    fromString(c);
}

string CardSet::str() const
{
    string out = "";
    uint64_t v = _cardmask;
    while (v) {
        Card card(lastbit64(v));
        out += card.str();
        v &= v - 1;  // clear the least significant bit set
    }
    return out;
}

// TODO: use same loop as in ::str()
string CardSet::rankstr() const
{
    string out = "";
    for (size_t i = 0; i < STANDARD_DECK_SIZE; i++) {
        uint64_t mask = ONE64 << i;
        if (_cardmask & mask) {
            Card c(i);
            out += c.rank().str();
        }
    }
    sort(out.begin(), out.end());
    return out;
}

string CardSet::toRankBitString() const
{
    string out = "";
    for (size_t i = 0; i < STANDARD_DECK_SIZE; i++) {
        if (_cardmask & ONE64 << i)
            out += "1";
        else
            out += ".";
        if (((i + 1) % Rank::NUM_RANK) == 0)
            out += " ";
    }
    return out;
}

size_t CardSet::countSuits() const
{
    int ns = 0;
    for (size_t i = 0; i < Suit::NUM_SUIT; i++)
        if (SMASK(i) > 0)
            ns++;
    return ns;
}

size_t CardSet::countMaxSuit() const
{
    vector<size_t> suit(Suit::NUM_SUIT);
    suit[0] = (nRanksTable[SMASK(0)]);
    suit[1] = (nRanksTable[SMASK(1)]);
    suit[2] = (nRanksTable[SMASK(2)]);
    suit[3] = (nRanksTable[SMASK(3)]);

    return *std::max_element(suit.begin(), suit.end());
}

size_t CardSet::size() const
{
    uint64_t v = _cardmask;
    size_t c;
    for (c = 0; v; c++) {
        v &= v - 1;  // clear the least significant bit set
    }
    return c;
}

void CardSet::fromString(const string& instr)
{
    clear();

    for (size_t i = 0; i < instr.size(); i += 2) {
        // skip whitespace
        if (instr[i] == ' ') {
            i -= 1;
            continue;
        }
        int code = Rank::rank_code(instr[i]) +
                   Suit::suit_code(instr[i + 1]) * Rank::NUM_RANK;
        uint64_t mask = (ONE64 << code);
        if (_cardmask & mask) {
            clear();  // card duplication is an error, no hand parsed
            return;
        }
        _cardmask |= mask;
    }
}

CardSet& CardSet::insert(const CardSet& c)
{
    _cardmask |= c._cardmask;
    return *this;
}

CardSet CardSet::rotateSuits(int c, int d, int h, int s) const
{
    return CardSet(
               static_cast<uint64_t>(suitMask(Suit::Clubs()))    << Rank::NUM_RANK * c |
               static_cast<uint64_t>(suitMask(Suit::Diamonds())) << Rank::NUM_RANK * d |
               static_cast<uint64_t>(suitMask(Suit::Hearts()))   << Rank::NUM_RANK * h |
               static_cast<uint64_t>(suitMask(Suit::Spades()))   << Rank::NUM_RANK * s);
}

void CardSet::flipSuits()
{
    *this = rotateSuits(3, 2, 1, 0);
}

CardSet CardSet::canonize() const
{
    int smasks[Suit::NUM_SUIT];
    int i = 0;
    for (Suit s = Suit::begin(); s < Suit::end(); ++s)
        smasks[i++] = suitMask(s);

    sort(smasks, smasks + Suit::NUM_SUIT);

    return CardSet(static_cast<uint64_t>(smasks[3]) |
                   static_cast<uint64_t>(smasks[2]) << Rank::NUM_RANK |
                   static_cast<uint64_t>(smasks[1]) << Rank::NUM_RANK * 2 |
                   static_cast<uint64_t>(smasks[0]) << Rank::NUM_RANK * 3);
}

CardSet CardSet::canonize(const CardSet& other) const
{
    CardSet cother = other.canonize();
    vector<int> perms = findSuitPermutation(other, cother);
    CardSet hand = *this;
    CardSet chand = hand.rotateSuits(perms[0], perms[1], perms[2], perms[3]);
    return chand;
}

// This is super slow, make it faster
bool CardSet::insertRanks(const CardSet& rset)
{
    // try the fastest way first
    if ((_cardmask & rset._cardmask) == 0) {
        _cardmask |= rset._cardmask;
        return true;
    }

    // now try shifting by flipping around suits
    for (size_t s = 1; s < Suit::NUM_SUIT; s++) {
        CardSet flipped(rset);
        flipped.flipSuits();
        if ((_cardmask & flipped._cardmask) == 0) {
            _cardmask |= flipped._cardmask;
            return true;
        }
    }

    // ok, just do it the slow way
    for (Rank r = Rank::Two(); r <= Rank::Ace(); ++r) {
        size_t nrin = rset.count(r);
        if (nrin == 0)
            continue;

        if (count(r) + nrin > Suit::NUM_SUIT)
            return false;

        for (Suit s = Suit::Clubs(); s <= Suit::Spades(); ++s)
        {
            Card c(r, s);
            if (!contains(c)) {
                insert(c);
                nrin--;
            }
            if (nrin == 0)
                break;
        }
    }
    return true;
}

CardSet CardSet::canonizeRanks() const
{
    // this is very slow, optimize if it winds up in an inner loop
    CardSet ret;
    string ranks = rankstr();
    for (size_t i = 0; i < ranks.size(); i++) {
        for (Suit s = Suit::Clubs(); s <= Suit::Spades(); ++s) {
            Card candidate(Rank(ranks.substr(i, 1)), s);
            if (!ret.contains(candidate)) {
                ret.insert(candidate);
                break;
            }
        }
    }
    return ret;
}

CardSet& CardSet::insert(const Card& c)
{
    _cardmask |= (ONE64 << c.code());
    return *this;
}

vector<Card> CardSet::cards() const
{
    vector<Card> out(size());
    size_t n = 0;
    for (size_t i = 0; i < STANDARD_DECK_SIZE; i++)
        if (_cardmask & (ONE64 << i))
            out[n++] = Card(i);
    return out;
}

vector<CardSet> CardSet::cardSets() const
{
    vector<CardSet> out(size());
    size_t n = 0;
    for (size_t i = 0; i < STANDARD_DECK_SIZE; i++)
        if (_cardmask & (ONE64 << i))
            out[n++] = CardSet(ONE64 << i);
    return out;
}

CardSet& CardSet::remove(const CardSet& c)
{
    _cardmask ^= c._cardmask;
    return *this;
}

CardSet& CardSet::remove(const Card& c)
{
    _cardmask ^= ONE64 << c.code();
    return *this;
}

bool CardSet::contains(const CardSet& c) const
{
    return (c._cardmask == (_cardmask & c._cardmask));
}

bool CardSet::contains(const Card& c) const
{
    uint64_t bit = ONE64 << c.code();
    return ((bit & _cardmask) != 0);
}

bool CardSet::contains(const Rank& r) const
{
    return (RMASK() & r.rankBit()) > 0;
}

Card CardSet::find(const Rank& r) const
{
    for (uint8_t i = 0; i < Suit::NUM_SUIT; i++) {
        if (SMASK(i) & r.rankBit())
            return Card(r, Suit(i));
    }
    return Card();
}

bool CardSet::contains(const Suit& s) const
{
    if (SMASK(s.code()) > 0)
        return true;
    return false;
}

int CardSet::rankMask() const
{
    return RMASK();
}

size_t CardSet::countRanks() const
{
    return nRanksTable[C() | D() | H() | S()];
}

int CardSet::suitMask(const Suit& s) const
{
    return SMASK(s.code());
}

size_t CardSet::count(const Rank& r) const
{
#if 1
    // this version is faster, if less obvious
    int c = C();
    int d = D();
    int h = H();
    int s = S();

    int four_mask = c & d & h & s;
    if (four_mask & r.rankBit())
        return 4;

    int three_mask =
        ((c & d) | (h & s)) &
        ((c & h) | (d & s));
    if (three_mask & r.rankBit())
        return 3;

    int rankmask = c | d | h | s;
    int two_mask = rankmask ^ (c ^ d ^ h ^ s);
    if (two_mask & r.rankBit())
        return 2;

    if (rankmask & r.rankBit())
        return 1;

    return 0;
#else
    size_t ret = 0;
    for (size_t i = 0; i < Suit::NUM_SUIT; i++)
        if (r.rankBit() & SMASK(i))
            ret++;
    return ret;
#endif
}

bool CardSet::hasStraight() const
{
    if (straightTable[RMASK()] > 0)
        return true;
    return false;
}

// this is a rough draft of the evaluateHigh routine, the idea is that
// it should be AFAP.  However, this is a general 1-7 card evaluator, so
// it cannot be too fast.  We could unwrap the method calls here for the
// PokerHand, maybe it would speed things up.
//
// note, there are no function calls in this function
PokerEvaluation CardSet::evaluateHigh() const
{
    // first the easy stuff
    int c = C();
    int d = D();
    int h = H();
    int s = S();
    int rankmask = c | d | h | s;

    if (nRanksTable[rankmask] >= 5) {
        int sranks = 0;
        int suitindex = -1;
        if (nRanksTable[c] >= 5) {
            suitindex = Suit::ClubVal();
            sranks = c;
        } else if (nRanksTable[d] >= 5) {
            suitindex = Suit::DiamondVal();
            sranks = d;
        } else if (nRanksTable[h] >= 5) {
            suitindex = Suit::HeartVal();
            sranks = h;
        } else if (nRanksTable[s] >= 5) {
            suitindex = Suit::SpadeVal();
            sranks = s;
        }
        if (suitindex >= 0) {
            int strval = straightTable[sranks];
            if (strval > 0)
                return PokerEvaluation((STRAIGHT_FLUSH << VSHIFT) ^
                                       strval << MAJOR_SHIFT);
            else
                return PokerEvaluation((FLUSH << VSHIFT) ^
                                       topFiveRanksTable[sranks]);
        }
        int strval = straightTable[rankmask];
        if (strval > 0)
            return PokerEvaluation((STRAIGHT << VSHIFT) ^
                                   (strval << MAJOR_SHIFT));
    }

    int ncards = nRanksTable[c] + nRanksTable[d] + nRanksTable[h] + nRanksTable[s];
    int ndups = ncards - nRanksTable[rankmask];

    switch (ndups) {
        case 0: { // no pair
            return PokerEvaluation((NO_PAIR << VSHIFT) ^
                                   topFiveRanksTable[rankmask]);
        }
        break;

        case 1: { // one pair
            int two_mask = rankmask ^ (c ^ d ^ h ^ s);
            int topind = topRankTable[two_mask];
            int kickers = topThreeRanksTable[rankmask ^ (0x01 << topind)];
            return PokerEvaluation((ONE_PAIR << VSHIFT) ^
                                   (topind << MAJOR_SHIFT) ^ kickers);
        }
        break;

        case 2: {
            int two_mask = rankmask ^ (c ^ d ^ h ^ s);

            if (two_mask) { // two pair
                int topind = topRankTable[two_mask];
                int botind = botRankTable[two_mask];
                int kicker = topRankTable[rankmask ^ two_mask];
                if (kicker >= 0)
                    return PokerEvaluation(
                               (TWO_PAIR << VSHIFT) ^ (topind << MAJOR_SHIFT) ^
                               (botind << MINOR_SHIFT) ^ (0x01 << kicker));
                else
                    return PokerEvaluation((TWO_PAIR << VSHIFT) ^
                                           (topind << MAJOR_SHIFT) ^
                                           (botind << MINOR_SHIFT));
            } else {
                int three_mask =
                    ((c & d) | (h & s)) &
                    ((c & h) | (d & s));
                int topind = topRankTable[three_mask];
                int kickers = rankmask ^ (0x01 << topind);
                int kbits = 0;
                if (kickers > 0)
                    kbits = 0x01 << topRankTable[kickers];
                if (kbits >= 0 && ((kickers ^ kbits) > 0))
                    kbits ^= 0x01 << topRankTable[kickers ^ kbits];
                return PokerEvaluation((THREE_OF_A_KIND << VSHIFT) ^
                                       (topind << MAJOR_SHIFT) ^ kbits);
            }
        }
        break;

        default: {
            int four_mask = c & d & h & s;
            if (four_mask) {
                int topind = topRankTable[four_mask];
                int kicker = rankmask;
                kicker ^= (0x01 << topind);
                kicker = topRankTable[kicker];
                if (kicker >= 0)
                    return PokerEvaluation((FOUR_OF_A_KIND << VSHIFT) ^
                                           (topind << MAJOR_SHIFT) ^
                                           (0x01 << kicker));
                else
                    return PokerEvaluation((FOUR_OF_A_KIND << VSHIFT) ^
                                           (topind << MAJOR_SHIFT));
            }

            int two_mask = rankmask ^ (c ^ d ^ h ^ s);
            if (nRanksTable[two_mask] != ndups) {
                int three_mask =
                    ((c & d) | (h & s)) &
                    ((c & h) | (d & s));
                int topind = topRankTable[three_mask];
                if (two_mask > 0) {
                    int botind = topRankTable[two_mask];
                    return PokerEvaluation((FULL_HOUSE << VSHIFT) ^
                                           (topind << MAJOR_SHIFT) ^
                                           (botind << MINOR_SHIFT));
                } else {
                    int botind = topRankTable[three_mask ^ 0x01 << topind];
                    return PokerEvaluation((FULL_HOUSE << VSHIFT) ^
                                           (topind << MAJOR_SHIFT) ^
                                           (botind << MINOR_SHIFT));
                }
            }

            int topind = topRankTable[two_mask];
            int botind = topRankTable[two_mask ^ 0x01 << topind];
            int kicker = rankmask ^ 0x01 << topind ^ 0x01 << botind;
            kicker = topRankTable[kicker];
            if (kicker >= 0)
                return PokerEvaluation(
                           (TWO_PAIR << VSHIFT) ^ (topind << MAJOR_SHIFT) ^
                           (botind << MINOR_SHIFT) ^ (0x01 << kicker));
            else
                return PokerEvaluation((TWO_PAIR << VSHIFT) ^
                                       (topind << MAJOR_SHIFT) ^
                                       (botind << MINOR_SHIFT));
        }
        break;
    }

    cerr << "oops\n";
    return PokerEvaluation(0);
}

// same as evaluateHigh, but with the non-flush logic
// elided
PokerEvaluation CardSet::evaluateHighFlush() const
{
    // first the easy stuff
    int c = C();
    int d = D();
    int h = H();
    int s = S();
    int rankmask = c | d | h | s;

    if (nRanksTable[rankmask] >= 5) {
        int sranks = 0;
        int suitindex = -1;
        if (nRanksTable[c] >= 5) {
            suitindex = Suit::ClubVal();
            sranks = c;
        } else if (nRanksTable[d] >= 5) {
            suitindex = Suit::DiamondVal();
            sranks = d;
        } else if (nRanksTable[h] >= 5) {
            suitindex = Suit::HeartVal();
            sranks = h;
        } else if (nRanksTable[s] >= 5) {
            suitindex = Suit::SpadeVal();
            sranks = s;
        }
        if (suitindex >= 0) {
            int strval = straightTable[sranks];
            if (strval > 0)
                return PokerEvaluation((STRAIGHT_FLUSH << VSHIFT) ^
                                       strval << MAJOR_SHIFT);
            else
                return PokerEvaluation((FLUSH << VSHIFT) ^
                                       topFiveRanksTable[sranks]);
        }
    }
    return PokerEvaluation(0);
}

// evaluteHighRanks, same as evaluateHigh, but with the flush logic
// elided
PokerEvaluation CardSet::evaluateHighRanks() const
{
    // first the easy stuff
    int c = C();
    int d = D();
    int h = H();
    int s = S();
    int rankmask = c | d | h | s;

    if (nRanksTable[rankmask] >= 5) {
        int strval = straightTable[rankmask];
        if (strval > 0)
            return PokerEvaluation((STRAIGHT << VSHIFT) ^
                                   (strval << MAJOR_SHIFT));
    }

    int ncards = nRanksTable[c] + nRanksTable[d] + nRanksTable[h] + nRanksTable[s];
    int ndups = ncards - nRanksTable[rankmask];

    switch (ndups) {
    case 0: { // no pair
        return PokerEvaluation((NO_PAIR << VSHIFT) ^
                               topFiveRanksTable[rankmask]);
    }
    break;

    case 1: { // one pair
        int two_mask = rankmask ^ (c ^ d ^ h ^ s);
        int topind = topRankTable[two_mask];
        int kickers = topThreeRanksTable[rankmask ^ (0x01 << topind)];
        return PokerEvaluation((ONE_PAIR << VSHIFT) ^
                               (topind << MAJOR_SHIFT) ^ kickers);
    }
    break;

    case 2: {
        int two_mask = rankmask ^ (c ^ d ^ h ^ s);

        if (two_mask) { // two pair
            int topind = topRankTable[two_mask];
            int botind = botRankTable[two_mask];
            int kicker = topRankTable[rankmask ^ two_mask];
            if (kicker >= 0)
                return PokerEvaluation((TWO_PAIR << VSHIFT) ^
                                       (topind << MAJOR_SHIFT) ^
                                       (botind << MINOR_SHIFT) ^
                                       (0x01 << kicker));
            else
                return PokerEvaluation((TWO_PAIR << VSHIFT) ^
                                       (topind << MAJOR_SHIFT) ^
                                       (botind << MINOR_SHIFT));
        } else {
            int three_mask =
                ((c & d) | (h & s)) &
                ((c & h) | (d & s));
            int topind = topRankTable[three_mask];
            int kickers = rankmask ^ (0x01 << topind);
            int kbits = 0;
            if (kickers > 0)
                kbits = 0x01 << topRankTable[kickers];
            if (kbits >= 0 && ((kickers ^ kbits) > 0))
                kbits ^= 0x01 << topRankTable[kickers ^ kbits];
            return PokerEvaluation((THREE_OF_A_KIND << VSHIFT) ^
                                   (topind << MAJOR_SHIFT) ^ kbits);
        }
    }
    break;

    default: {
        int four_mask = c & d & h & s;
        if (four_mask) {
            int topind = topRankTable[four_mask];
            int kicker = rankmask;
            kicker ^= (0x01 << topind);
            kicker = topRankTable[kicker];
            if (kicker >= 0)
                return PokerEvaluation((FOUR_OF_A_KIND << VSHIFT) ^
                                       (topind << MAJOR_SHIFT) ^
                                       (0x01 << kicker));
            else
                return PokerEvaluation((FOUR_OF_A_KIND << VSHIFT) ^
                                       (topind << MAJOR_SHIFT));
        }

        int two_mask = rankmask ^ (c ^ d ^ h ^ s);
        if (nRanksTable[two_mask] != ndups) {
            int three_mask =
                ((c & d) | (h & s)) &
                ((c & h) | (d & s));
            int topind = topRankTable[three_mask];
            if (two_mask > 0) {
                int botind = topRankTable[two_mask];
                return PokerEvaluation((FULL_HOUSE << VSHIFT) ^
                                       (topind << MAJOR_SHIFT) ^
                                       (botind << MINOR_SHIFT));
            } else {
                int botind = topRankTable[three_mask ^ 0x01 << topind];
                return PokerEvaluation((FULL_HOUSE << VSHIFT) ^
                                       (topind << MAJOR_SHIFT) ^
                                       (botind << MINOR_SHIFT));
            }
        }

        int topind = topRankTable[two_mask];
        int botind = topRankTable[two_mask ^ 0x01 << topind];
        int kicker = rankmask ^ 0x01 << topind ^ 0x01 << botind;
        kicker = topRankTable[kicker];
        if (kicker >= 0)
            return PokerEvaluation((TWO_PAIR << VSHIFT) ^
                                   (topind << MAJOR_SHIFT) ^
                                   (botind << MINOR_SHIFT) ^
                                   (0x01 << kicker));
        else
            return PokerEvaluation((TWO_PAIR << VSHIFT) ^
                                   (topind << MAJOR_SHIFT) ^
                                   (botind << MINOR_SHIFT));
    }
    break;
    }

    cerr << "oops\n";
    return PokerEvaluation(0);
}

// this is just evaluateHigh without the straight or flush code
PokerEvaluation CardSet::evaluatePairing() const
{
    // first the easy stuff
    int c = C();
    int d = D();
    int h = H();
    int s = S();
    int rankmask = c | d | h | s;
    int ncards =
        nRanksTable[c] + nRanksTable[d] + nRanksTable[h] + nRanksTable[s];
    int ndups = ncards - nRanksTable[rankmask];

    switch (ndups) {
    case 0: { // no pair
        return PokerEvaluation((NO_PAIR << VSHIFT) ^
                               topFiveRanksTable[rankmask]);
    }
    break;

    case 1: { // one pair
        int two_mask = rankmask ^ (c ^ d ^ h ^ s);
        int topind = topRankTable[two_mask];
        int kickers = topThreeRanksTable[rankmask ^ (0x01 << topind)];
        return PokerEvaluation((ONE_PAIR << VSHIFT) ^
                               (topind << MAJOR_SHIFT) ^
                                kickers);
    }
    break;

    case 2: {
        int two_mask = rankmask ^ (c ^ d ^ h ^ s);

        if (two_mask) { // two pair
            int topind = topRankTable[two_mask];
            int botind = botRankTable[two_mask];
            int kicker = topRankTable[rankmask ^ two_mask];
            if (kicker >= 0)
                return PokerEvaluation((TWO_PAIR << VSHIFT) ^
                                       (topind << MAJOR_SHIFT) ^
                                       (botind << MINOR_SHIFT) ^
                                       (0x01 << kicker));
            else
                return PokerEvaluation((TWO_PAIR << VSHIFT) ^
                                       (topind << MAJOR_SHIFT) ^
                                       (botind << MINOR_SHIFT));
        } else {
            int three_mask =
                ((c & d) | (h & s)) &
                ((c & h) | (d & s));
            int topind = topRankTable[three_mask];
            int kickers = rankmask ^ (0x01 << topind);
            int kbits = 0;
            if (kickers > 0)
                kbits = 0x01 << topRankTable[kickers];
            if (kbits >= 0 && ((kickers ^ kbits) > 0))
                kbits ^= 0x01 << topRankTable[kickers ^ kbits];
            return PokerEvaluation((THREE_OF_A_KIND << VSHIFT) ^
                                   (topind << MAJOR_SHIFT) ^
                                   kbits);
        }
    }
    break;

    default: {
        int four_mask = c & d & h & s;
        if (four_mask) {
            int topind = topRankTable[four_mask];
            int kicker = rankmask;
            kicker ^= (0x01 << topind);
            kicker = topRankTable[kicker];
            if (kicker >= 0)
                return PokerEvaluation((FOUR_OF_A_KIND << VSHIFT) ^
                                       (topind << MAJOR_SHIFT) ^
                                       (0x01 << kicker));
            else
                return PokerEvaluation((FOUR_OF_A_KIND << VSHIFT) ^
                                       (topind << MAJOR_SHIFT));
        }

        int two_mask = rankmask ^ (c ^ d ^ h ^ s);
        if (nRanksTable[two_mask] != ndups) {
            int three_mask =
                ((c & d) | (h & s)) &
                ((c & h) | (d & s));
            int topind = topRankTable[three_mask];
            if (two_mask > 0) {
                int botind = topRankTable[two_mask];
                return PokerEvaluation((FULL_HOUSE << VSHIFT) ^
                                       (topind << MAJOR_SHIFT) ^
                                       (botind << MINOR_SHIFT));
            } else {
                int botind = topRankTable[three_mask ^ 0x01 << topind];
                return PokerEvaluation((FULL_HOUSE << VSHIFT) ^
                                       (topind << MAJOR_SHIFT) ^
                                       (botind << MINOR_SHIFT));
            }
        }

        int topind = topRankTable[two_mask];
        int botind = topRankTable[two_mask ^ 0x01 << topind];
        int kicker = rankmask ^ 0x01 << topind ^ 0x01 << botind;
        kicker = topRankTable[kicker];
        if (kicker >= 0)
            return PokerEvaluation((TWO_PAIR << VSHIFT) ^
                                   (topind << MAJOR_SHIFT) ^
                                   (botind << MINOR_SHIFT) ^
                                   (0x01 << kicker));
        else
            return PokerEvaluation((TWO_PAIR << VSHIFT) ^
                                   (topind << MAJOR_SHIFT) ^
                                   (botind << MINOR_SHIFT));
    }
    break;
    }

    cerr << "oops\n";
    return PokerEvaluation(0);
}

// This one is not fully optimized
PokerEvaluation CardSet::evaluateLow2to7() const
{
    PokerEvaluation high;

    // if there are five or fewer cards, we just evaluate the high,
    // fix the wheel, and take the complement
    switch (size()) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        high = evaluateHigh();
        break;

    case 5:
        high = evaluateHigh();
        high.fixWheel2to7(rankMask());
        break;

    default:
        // this is a slow way to handle the general case.
        // TODO: specialize the code for the 6 and 7 card cases.
        vector<Card> cards = this->cards();
        combinations combo(size(), FULL_HAND_SIZE);
        PokerEvaluation best;
        do {
            CardSet candidate;
            for (size_t i = 0; i < static_cast<size_t>(FULL_HAND_SIZE); i++)
                candidate.insert(cards[combo[i]]);
            PokerEvaluation e = candidate.evaluateLow2to7();
            if (e > best)
                best = e;
        } while (combo.next());
        return best;
    }

    high.flip();
    return high;
}

// This one is not fully optimized
PokerEvaluation CardSet::evaluateRanksLow2to7() const
{
    PokerEvaluation high;
    PokerEvaluation h;

    // if there are five or fewer cards, we just evaluate the high,
    // fix the wheel, and take the complement
    switch (size()) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
            high = evaluateHighRanks();
            break;

        case 5:
            high = evaluateHighRanks();
            high.fixWheel2to7(rankMask());
            break;

        default:
            // this is a slow way to handle the general case.
            // TODO: specialize the code for the 6 and 7 card cases.
            vector<Card> cards = this->cards();
            combinations combo(size(), FULL_HAND_SIZE);
            PokerEvaluation best;
            do {
                CardSet candidate;
                for (size_t i = 0; i < static_cast<size_t>(FULL_HAND_SIZE); i++)
                    candidate.insert(cards[combo[i]]);
                PokerEvaluation e = candidate.evaluateRanksLow2to7();
                if (e > best)
                    best = e;
            } while (combo.next());
            return best;
    }

    high.flip();
    return high;
}

// This one is not fully optimized
PokerEvaluation CardSet::evaluateSuitsLow2to7() const
{
    PokerEvaluation high;

    // if there are five or fewer cards, we just evaluate the high,
    // fix the wheel, and take the complement
    switch (size()) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
            high = evaluateHighFlush();
            break;

        case 5:
            high = evaluateHighFlush();
            high.fixWheel2to7(rankMask());
            break;

        default:
            // this is a slow way to handle the general case.
            // TODO: specialize the code for the 6 and 7 card cases.
            vector<Card> cards = this->cards();
            combinations combo(size(), FULL_HAND_SIZE);
            PokerEvaluation best;
            do {
                CardSet candidate;
                for (size_t i = 0; i < static_cast<size_t>(FULL_HAND_SIZE); i++)
                    candidate.insert(cards[combo[i]]);
                PokerEvaluation e = candidate.evaluateSuitsLow2to7();
                if (e > best)
                    best = e;
            } while (combo.next());
            return best;
    }

    high.flip();
    return high;
}

// This one is not fully optimized
PokerEvaluation CardSet::evaluateLowA5() const
{
    // PokerEvaluation::generateLowballLookupA5();

    // this is a rank only evaluator, so we just get
    // the rank masks and then fill accordingly.  We
    // also have to shift the rank mask according to the
    // the fact that the ace swings low
    // this could probably be much faster

    int c = C();
    int d = D();
    int h = H();
    int s = S();
    int rankmask = c | d | h | s;

    int ncards = nRanksTable[c] + nRanksTable[d] + nRanksTable[h] + nRanksTable[s];
    int ndups = ncards - nRanksTable[rankmask];
    int nranks = nRanksTable[rankmask];

    // first the easy cases
    // 1) no duplicate ranks
    // 2) 5 or more ranks
    if (ndups == 0 || nranks >= FULL_HAND_SIZE) {
        PokerEvaluation ret((NO_PAIR << VSHIFT) ^ lowballA5Ranks[rankmask] ^ ACE_LOW_BIT);
        ret.flip();
        return ret;
    }
    // another easy case
    // *) we have five or fewer cards
    else if (ncards <= FULL_HAND_SIZE) {
        PokerEvaluation ret = evaluatePairing();
        ret.playAceLow();
        ret.flip();
        return ret;
    } else {
        // now the general case
        // the algorithm is as follows:
        // cycle through the "pairing list" till a total of five
        // cards are found
        uint64_t chash = 0;  // = rankmask;
        int rset[4];

        rset[0] = rankmask;
        rset[1] = rankmask ^ (c ^ d ^ h ^ s);                // mask of cards with 2/4 ranks
        rset[2] = ((c & d) | (h & s)) &((c & h) | (d & s));  // mask of cards with 3 ranks
        rset[3] = c & d & h & s;                             // fourmask
        rset[1] |= rset[2];                                  // add the threes back to teh twos

        rset[0] = LOWBALL_ROTATE_RANKS(rset[0]);
        rset[1] = LOWBALL_ROTATE_RANKS(rset[1]);
        rset[2] = LOWBALL_ROTATE_RANKS(rset[2]);
        rset[3] = LOWBALL_ROTATE_RANKS(rset[3]);

        // we add pairs first, then trips, then quads
        int nc = 0;
        for (int i = 0; i < 4; i++) {
            // do the ace first
            if (rset[i] & (0x01 << Rank::AceVal())) {
                chash |= (ONE64 << (Rank::AceVal() + Rank::NUM_RANK * i));
                nc++;
            }
            if (nc == FULL_HAND_SIZE) {
                break;
            }

            // now the rest of the ranks
            for (int r = 0; r < 11; r++) {
                if (rset[i] & (0x01 << r)) {
                    chash |= (ONE64 << (r + Rank::NUM_RANK * i));
                    nc++;
                }
                if (nc == FULL_HAND_SIZE) {
                    break;
                }
            }
            if (nc == FULL_HAND_SIZE) {
                break;
            }
        }

        CardSet paird(chash);
        PokerEvaluation ret(paird.evaluatePairing().code() | ACE_LOW_BIT);
        ret.flip();
        return ret;
    }
}

PokerEvaluation CardSet::evaluate8LowA5() const
{
    // bits 2-8+A are set here
    const int LOW_MASK = 0x107F;

    int c = C() & LOW_MASK;
    int d = D() & LOW_MASK;
    int h = H() & LOW_MASK;
    int s = S() & LOW_MASK;

    int rankmask = c | d | h | s;
    int nranks = nRanksTable[rankmask];

    if (nranks >= FULL_HAND_SIZE) {
        PokerEvaluation ret(((NO_PAIR << VSHIFT) ^
                            lowballA5Ranks[rankmask]) | ACE_LOW_BIT);
        ret.flip();
        return ret;
    }

    PokerEvaluation e;
    return e;
}

PokerEvaluation CardSet::evaluate3CP() const
{
    int c = C();
    int d = D();
    int h = H();
    int s = S();
    int rankmask = c | d | h | s;

    int ncards = nRanksTable[c] + nRanksTable[d] + nRanksTable[h] + nRanksTable[s];
    if (ncards > 3)
        throw std::invalid_argument("3CP evaluator only works on three cards");

    // check for three flush
    bool threeflush = countSuits() == 1 ? true : false;

    // then three straight
    const int THREE_WHEEL = 0x01 << Rank::AceVal() | 0x01 << Rank::TwoVal() | 0x01 << Rank::ThreeVal();
    int topr = topRankTable[rankmask];
    int botr = botRankTable[rankmask];
    int strr = -1;
    bool threestr = false;
    if (topr - botr == 2 && nRanksTable[rankmask] == 3) {
        threestr = true;
        strr = topr;
    } else if (rankmask == THREE_WHEEL) {
        threestr = true;
        strr = Rank::ThreeVal();
    }

    if (threeflush) {
        if (threestr)
            return PokerEvaluation((THREE_STRAIGHT_FLUSH << VSHIFT) ^ strr << MAJOR_SHIFT);
        else
            return PokerEvaluation((THREE_FLUSH << VSHIFT) ^ rankmask);
    } else if (threestr) {
        return PokerEvaluation((THREE_STRAIGHT << VSHIFT) ^ (strr << MAJOR_SHIFT));
    } else {
        return evaluatePairing();
    }
}

// three card badugis are better than two card badugis
static inline bool badugiless(int c1, int c2)
{
    int n1 = nRanksTable[c1];
    int n2 = nRanksTable[c2];
    if (n1 == n2)
        return c1 < c2;
    return n1 > n2;
}

/**
 * Badugi hand evaluator. To make this faster, I need a
 * bottomRankMask.
 *
 * To do a four card evaluation it suffices to pass through the suits
 * twice, once in each direction.  To do a general evaluation, all
 * suits must be traversed in all orders so that 4*3*2*1=24 suit
 * traversals will be done.  Suits of size zero can be ruled out and
 * those with size one can be ruled in.
 */
PokerEvaluation CardSet::evaluateBadugi() const
{
    // get our ranks orgainzed in lowball order by suit
    std::array<int, 4> suits = {
        {
            LOWBALL_ROTATE_RANKS(C()), LOWBALL_ROTATE_RANKS(D()),
            LOWBALL_ROTATE_RANKS(H()), LOWBALL_ROTATE_RANKS(S())
        }
    };

    // We try to save some time by being smart about which suits we loop
    // over.  Empty suits are ignored, and suits with one rank are used
    // as is.  At the end we sort to make next_permutation to work properly.
    std::array<int, 4> ind;
    int bmust = 0;
    size_t k = 0;
    for (size_t i = 0; i < suits.size(); i++) {
        switch (nRanksTable[suits[i]]) {
            case 1:
                bmust |= suits[i];
            // fall through
            case 0:
                break;
            default:
                ind[k++] = suits[i];
        };
    }
    sort(ind.begin(), ind.begin() + k);

    // now enter into the fray to find the best possible badugi.  suit
    // order in this loop matters, so we cycle through the permutations
    // of suit order.
    //
    // zeroing minbadugi works because it has no ranks and a one rank
    // badugi is less than a zero rank badugi.
    int minbadugi = 0;
    do {
        // explanation of bit ops, b=current badugi, s=suit being considered
        // (b|s)-b                          set of ranks in suit s not in the badugi
        // yet b |= botRankMask[(b|s)-b]    for each relevant suit do this:
        int branks = bmust;
        for (size_t i = 0; i < k; i++)
            branks |= bottomRankMask[(branks | ind[i]) - branks];

        if (badugiless(branks, minbadugi))
            minbadugi = branks;
    } while (next_permutation(ind.begin(), ind.begin() + k));

    // encode the badugi, it's a low hand where aces plays low.  we set
    // the number of missing suits in the badugi as the "major rank". the
    // value is chosen so that more cards maps to a lower value
    PokerEvaluation ret(((NO_PAIR << VSHIFT) ^ minbadugi) | ACE_LOW_BIT);
    ret.setMajorRank(Suit::NUM_SUIT - nRanksTable[minbadugi]);
    ret.flip();
    return ret;
}

bool CardSet::isPaired() const  // returns true if *any* two cards match rank
{
    int c = C();
    int d = D();
    int h = H();
    int s = S();
    int rankmask = c | d | h | s;

    int two_mask = rankmask ^ (c ^ d ^ h ^ s);
    if (two_mask > 0)
        return true;
    return false;
}

size_t CardSet::countMaxRank() const  // returns true if trips or quads
{
    int c = C();
    int d = D();
    int h = H();
    int s = S();

    int four_mask = c & d & h & s;
    if (four_mask)
        return 4;

    int three_mask =
        ((c & d) | (h & s)) &
        ((c & h) | (d & s));
    if (three_mask)
        return 3;

    int rankmask = c | d | h | s;
    int two_mask = rankmask ^ (c ^ d ^ h ^ s);
    if (two_mask)
        return 2;

    if (rankmask > 0)
        return 1;

    return 0;
}

bool CardSet::isTripped() const  // returns true if trips or quads
{
    int c = C();
    int d = D();
    int h = H();
    int s = S();

    int three_mask =
        ((c & d) | (h & s)) &
        ((c & h) | (d & s));

    if (three_mask > 0)
        return true;
    return false;
}

size_t CardSet::count(const Suit& s) const
{
    return nRanksTable[SMASK(s.code())];
}

Rank CardSet::flushRank(const Suit& s) const
{
    return Rank(topRankTable[SMASK(s.code())]);
}

Rank CardSet::topRank() const
{
    return Rank(topRankTable[RMASK()]);
}

Rank CardSet::bottomRank() const
{
    return Rank(botRankTable[RMASK()]);
}

double clampedChoose(int n, int m)
{
    if (n < m)
        return 0;
    return static_cast<size_t>(boost::math::binomial_coefficient<double>(n, m));
}

int CardSet::evaluateStraightOuts() const
{
    int sval = straightTable[RMASK()];
    if (sval > 0)  // straight on board, all cards make straight
        return STANDARD_DECK_SIZE;
    if (sval == -2)  // open-ended, eight outs
        return 8;
    if (sval == -1)  // gut-shot, four outs
        return 4;
    if (sval == -3)  // runner-runner, "1" out
        return 1;
    return 0;
}

// original version in r2488
size_t CardSet::rankColex() const
{
    size_t ret = 0;
    int slot = 0;
    int sz = 1;
    int c = C();
    int d = D();
    int h = H();
    int s = S();
    int rbit = 0x01;

    for (int i = 0; i < 13; i++) {
        if (rbit & c) ret += clampedChoose(slot++, sz++);
        if (rbit & d) ret += clampedChoose(slot++, sz++);
        if (rbit & h) ret += clampedChoose(slot++, sz++);
        if (rbit & s) ret += clampedChoose(slot++, sz++);
        slot++;
        rbit <<= 1;
    }
    return ret;
}

vector<int> pokerstove::findSuitPermutation(const CardSet& source, const CardSet& dest)
{
    vector<int> rot(4, -1);
    vector<int> taken(4, 0);

    int i = 0;
    for (Suit s = Suit::begin(); s < Suit::end(); ++s, ++i) {
        int j = 0;
        for (Suit t = Suit::begin(); t < Suit::end(); ++t, ++j) {
            if (source.suitMask(s) == dest.suitMask(t)) {
                if (taken[j] == 0) {
                    rot[i] = j;
                    taken[j] = 1;
                    break;
                }
            }
        }
    }
    return rot;
}

CardSet pokerstove::canonizeToBoard(const CardSet& board, const CardSet& hand)
{
    CardSet cboard = board.canonize();
    vector<int> perms = findSuitPermutation(board, cboard);
    CardSet chand = hand.rotateSuits(perms[0], perms[1], perms[2], perms[3]);
    return chand;
}

// some suit mask macros
#undef C
#undef D
#undef H
#undef S
#undef RMASK
#undef SUITMASK

size_t CardSet::colex() const
{
    vector<Card> cards = this->cards();
    size_t value = 0;
    for (size_t i = 0; i < cards.size(); i++) {
        size_t code = cards[i].code();
#ifdef __AP_MATH__
        value += tchoose(code, i + 1);
#else
        if (code >= i + 1)
            value += static_cast<size_t>(boost::math::binomial_coefficient<double>(code, i + 1));
#endif
    }
    return value;
}
