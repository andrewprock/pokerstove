/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 * $Id: PokerHand.cpp 2649 2012-06-30 04:53:24Z prock $
 */
#include "PokerHand.h"
#include "CardSet.h"
#include "Holdem.h"
#include "PokerEvaluation.h"
#include "PokerEvaluationTables.h"
#include <algorithm>
#include <cassert>
#include <stdexcept>

using namespace std;
using namespace pokerstove;

PokerHand::PokerHand() { clear(); }

CardSet PokerHand::cardSet() const
{
    CardSet cs;
    for (uint8_t i = 0; i < _ncards; i++)
        cs.insert(Card(_cards[i]));
    return cs;
}

CardSet PokerHand::cardSet(size_t first, size_t len) const
{
    CardSet cs;
    size_t last = min(first + len, static_cast<size_t>(_ncards));
    for (size_t i = first; i < last; i++)
        cs.insert(_cards[i]);
    return cs;
}

PokerHand::PokerHand(const CardSet& s)
{
    clear();
    append(s);
}

PokerHand::PokerHand(const string& s)
{
    clear();
    fromString(s);
}

void PokerHand::clear() { _ncards = 0; }

// if we have few enough cards, we'll spit it out in order, otherwise
// just output them as a sorted set.
string PokerHand::str() const
{
    string out = "";
    for (int i = 0; i < _ncards; i++)
        out += _cards[i].str();
    return out;
}

string PokerHand::preflopstr() const
{
    if (_ncards != 2)
        throw std::runtime_error("incorrect number of cards for hold'em preflop canon");
    Card c0 = Card(_cards[0]);
    Card c1 = Card(_cards[1]);

    if (c0.rank() == c1.rank())
        return c0.rank().str() + c0.rank().str();

    if (c0.rank() < c1.rank())
        swap(c0, c1);

    if (c0.suit() == c1.suit())
        return c0.rank().str() + c1.rank().str() + "s";
    return c0.rank().str() + c1.rank().str() + "o";
}

// parsing a string has a specific meaning.  All cards found in the
// string, regardless of the other contents of the string, will be
// extracted and populate the hand.
void PokerHand::fromString(const string& instr)
{
    clear();
    size_t slen = (size_t)instr.length();
    if (slen == 0)
        return;
    if (slen == 1)
        return;
    // throw std::invalid_argument (string("PokerHand parse error: " +
    // instr).c_str());

    for (size_t i = 0; i < slen - 1; i++)
    {
        Card c;
        if (c.fromString(instr.substr(i)))
        {
            append(c);
            i++;
        }
    }
}

size_t PokerHand::size() const { return static_cast<size_t>(_ncards); }

bool PokerHand::contains(const Card& c) const
{
    for (uint8_t i = 0; i < _ncards; i++)
        if (_cards[i] == c)
            return true;
    return false;
}

void PokerHand::append(const Card& c)
{
    if (contains(c))
        return;

    if (_ncards < MAX_PHCARDS)
        _cards[_ncards++] = c;
}

void PokerHand::append(const CardSet& cs)
{
    vector<Card> cards = cs.cards();
    for (size_t i = 0; i < cards.size(); i++)
        append(cards[i]);
}

void PokerHand::append(const PokerHand& h)
{
    for (int i = 0; i < h._ncards; i++)
        append(h._cards[i]);
}

void PokerHand::remove(const Card& c)
{
    for (uint8_t i = 0, j = 0; i < _ncards; i++, j++)
    {
        if (c == _cards[i])
        {
            j++;
            _ncards--;
        }
        _cards[i] = _cards[j];
    }
}

Card& PokerHand::operator[](size_t index) { return _cards[index]; }

const Card& PokerHand::operator[](size_t index) const { return _cards[index]; }

void PokerHand::sort() const
{
    std::sort(_cards.begin(), _cards.begin() + _ncards);
}

void PokerHand::pushCardToFront(size_t nth) const
{
    if (nth >= _ncards)
        return;
    Card c = _cards[nth];
    for (size_t i = nth; i > 0; i--)
        _cards[i] = _cards[i - 1];
    _cards[0] = c;
}

static bool rankgreater(Card c1, Card c2)
{
    if (c1.rank() > c2.rank())
        return true;
    return false;
}

void PokerHand::sortRanks() const
{
    std::sort(_cards.begin(), _cards.begin() + _ncards, rankgreater);
}

void PokerHand::sortEval() const
{
    PokerHand oldh = *this;
    PokerHand newh;
    PokerEvaluation eval = cardSet().evaluateHigh();
    Rank r;
    bool dominor = true;
    Rank straighttop = Rank::Five();

    oldh.sortRanks();
    switch (eval.type())
    {
            // these types have major only
        case ONE_PAIR:
        case THREE_OF_A_KIND:
        case FOUR_OF_A_KIND:
            dominor = false;

            // these have major and minor
        case TWO_PAIR:
        case FULL_HOUSE:
            r = eval.majorRank();
            while (oldh.cardSet().contains(r))
            {
                Card c = oldh.cardSet().find(r);
                newh.append(c);
                oldh.remove(c);
            }
            if (dominor)
            {
                r = eval.minorRank();
                while (oldh.cardSet().contains(r))
                {
                    Card c = oldh.cardSet().find(r);
                    newh.append(c);
                    oldh.remove(c);
                }
            }
            // we pass through here, no break

        case NO_PAIR:
        case THREE_FLUSH:
        case THREE_STRAIGHT_FLUSH:
        case FLUSH:
            newh.append(oldh);
            break;

            // straights may require the ace to swing low
        case THREE_STRAIGHT:
            straighttop = Rank::Three();
        case STRAIGHT:
        case STRAIGHT_FLUSH:
            newh = oldh;
            r = eval.majorRank();
            if (r == straighttop)
            {
                Card c = newh.cardSet().find(Rank::Ace());
                newh.remove(c);
                newh.append(c);
            }
            break;
    }

    for (int i = 0; i < _ncards; i++)
        _cards[i] = newh._cards[i];
}

void PokerHand::remove(const CardSet& cs)
{
    vector<Card> cards = cs.cards();
    for (size_t i = 0; i < cards.size(); i++)
        remove(cards[i]);
}

void PokerHand::remove(const PokerHand& ph)
{
    for (uint8_t i = 0; i < ph._ncards; i++)
        remove(ph._cards[i]);
}

vector<Card> PokerHand::cards() const
{
    vector<Card> out;
    for (uint8_t i = 0; i < _ncards; i++)
        out.push_back(_cards[i]);
    return out;
}
