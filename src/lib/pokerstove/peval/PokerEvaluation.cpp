/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 * $Id: PokerEvaluation.cpp 2649 2012-06-30 04:53:24Z prock $
 */
#include "PokerEvaluation.h"
#include "Card.h"
#include "PokerEvaluationTables.h"
#include <boost/format.hpp>
#include <iostream>
#include <pokerstove/util/lastbit.h>
#include <stdexcept>

using namespace std;
using namespace pokerstove;

// some useful consts that don't need to be exposd
const int MAJOR_MASK = 0xF << MAJOR_SHIFT;
const int MINOR_MASK = 0xF << MINOR_SHIFT;
const int KICKER_MASK = 0x1FFF;

PokerEvaluation::PokerEvaluation()
    : _evalcode(0)
{}

PokerEvaluation::PokerEvaluation(int ecode)
    : _evalcode(ecode)
{}

int PokerEvaluation::code() const { return _evalcode; }

int PokerEvaluation::reducedCode() const
{
    if (isFlipped())
    {
        PokerEvaluation e = *this;
        e.flip();
        return e.reducedCode2to7();
    }

    if (_evalcode == 0)
        return 0;

    switch (type())
    {
        case NO_PAIR:
        case ONE_PAIR:
        case THREE_FLUSH:
        case FLUSH:
        {
            PokerEvaluation e(_evalcode);
            int kick1 = topRankTable[kickerBits()];
            int kick2 = topRankTable[kickerBits() ^ 0x01 << kick1];
            int kbits = (0x01 << kick1) | (0x01 << kick2);
            e.setKickerBits(kbits);
            return e.code();
        }

        case FULL_HOUSE:
        case THREE_OF_A_KIND:
        case FOUR_OF_A_KIND:
            return showdownCode();

        default:
            return code();
    }
}

int PokerEvaluation::reducedCode2to7() const
{
    if (_evalcode == 0)
        return 0;
    switch (type())
    {
        case NO_PAIR:
        {
            PokerEvaluation e(_evalcode);
            int kick1 = topRankTable[kickerBits()];
            int kick2 = topRankTable[kickerBits() ^ 0x01 << kick1];
            int kick3 = topRankTable[kickerBits() ^ 0x01 << kick1 ^ 0x01 << kick2];
            int kbits = (0x01 << kick1) | (0x01 << kick2);
            if (kick1 < 10)
                kbits |= (0x01 << kick3);
            e.setKickerBits(kbits);
            e.flip();
            return e.code();
        }

        case THREE_OF_A_KIND:
        {
            PokerEvaluation e(_evalcode);
            vector<int> ranks(3);
            ranks[0] = botRankTable[kickerBits()];
            ranks[1] = botRankTable[kickerBits() ^ 0x01 << ranks[0]];
            ranks[2] = e.majorRank().code();
            sort(ranks.begin(), ranks.end());
            e = PokerEvaluation((TWO_PAIR << VSHIFT) ^
                                (ranks[2] << MAJOR_SHIFT) ^
                                (ranks[1] << MINOR_SHIFT) ^ (0x01 << ranks[0]));
            e.flip();
            return e.code();
        }

        case ONE_PAIR:
        {
            PokerEvaluation e(_evalcode);
            vector<int> ranks(4);
            uint16_t kickers = kickerBits();
            ranks[0] = lastbit(kickers);
            kickers ^= 0x01 << ranks[0];
            ranks[1] = lastbit(kickers);
            kickers ^= 0x01 << ranks[1];
            ranks[2] = lastbit(kickers);
            kickers ^= 0x01 << ranks[2];
            ranks[3] = e.majorRank().code();
            sort(ranks.begin(), ranks.end());
            e = PokerEvaluation((ONE_PAIR << VSHIFT) ^
                                (ranks[2] << MAJOR_SHIFT) ^ (0x01 << ranks[0]) ^
                                (0x01 << ranks[1]));
            e.flip();
            return e.code();
        }

        case TWO_PAIR:
        {
            PokerEvaluation e(_evalcode);
            vector<int> ranks(3);
            uint16_t kickers = kickerBits();
            ranks[0] = lastbit(kickers);
            ranks[1] = e.minorRank().code();
            ranks[2] = e.majorRank().code();
            sort(ranks.begin(), ranks.end());
            e = PokerEvaluation((TWO_PAIR << VSHIFT) ^
                                (ranks[2] << MAJOR_SHIFT) ^
                                (ranks[1] << MINOR_SHIFT) ^ (0x01 << ranks[0]));
            e.flip();
            return e.code();
        }

        case FLUSH:
        {
            PokerEvaluation e(_evalcode);
            int kick1 = topRankTable[kickerBits()];
            int kbits = (0x01 << kick1);
            e.setKickerBits(kbits);
            e.flip();
            return e.code();
        }

        case FULL_HOUSE:
        {
            PokerEvaluation e(_evalcode);
            vector<int> ranks(2);
            ranks[0] = e.minorRank().code();
            ranks[1] = e.majorRank().code();
            sort(ranks.begin(), ranks.end());
            e = PokerEvaluation((FULL_HOUSE << VSHIFT) ^
                                (ranks[1] << MAJOR_SHIFT) ^
                                (ranks[0] << MINOR_SHIFT));
            e.flip();
            return e.code();
        }

        case FOUR_OF_A_KIND:
        {
            PokerEvaluation e(_evalcode);
            vector<int> ranks(2);
            uint16_t kickers = kickerBits();
            ranks[0] = lastbit(kickers);
            ranks[1] = e.majorRank().code();
            e = PokerEvaluation((FOUR_OF_A_KIND << VSHIFT) ^
                                (ranks[1] << MAJOR_SHIFT) ^ (0x01 << ranks[0]));
            e.flip();
            return e.code();
        }

        default:
            return code();
    }
}

int PokerEvaluation::showdownCode() const
{
    if (_evalcode == 0)
        return 0;

    int minor = 0;
    switch (type())
    {
        case FULL_HOUSE:  // fall through case, prints deuces full nicer
            if (majorRank() == Rank::Two())
                minor = 1;
        case THREE_OF_A_KIND:
        case FOUR_OF_A_KIND:
        {
            PokerEvaluation e(_evalcode);
            e.setMinorRank(minor);
            e.setKickerBits(0);
            return e.code();
        }

        default:
            return code();
    }
}

int PokerEvaluation::type() const { return _evalcode >> VSHIFT; }

int PokerEvaluation::kickerBits() const { return _evalcode & KICKER_MASK; }
Rank PokerEvaluation::majorRank() const { return Rank((_evalcode >> MAJOR_SHIFT) & 0x0F); }
Rank PokerEvaluation::minorRank() const { return Rank((_evalcode >> MINOR_SHIFT) & 0x0F); }

void PokerEvaluation::setKickerBits(int k) { _evalcode = (_evalcode & ~KICKER_MASK) | k; }
void PokerEvaluation::setMajorRank(int r)  { _evalcode = (_evalcode & ~MAJOR_MASK) | r << MAJOR_SHIFT; }
void PokerEvaluation::setMinorRank(int r)  { _evalcode = (_evalcode & ~MINOR_MASK) | r << MINOR_SHIFT; }

// this converts the evaluation to one where
// the ace is valued as low instead of high,
// thus 2 > A internally.
void PokerEvaluation::playAceLow()
{
    if (acePlaysLow())
        return;

    _evalcode |= ACE_LOW_BIT;

    // make room for the ace in kicker land, and if
    // we have an ace, push it to the bottom
    int kickers = kickerBits();
    kickers = kickers << 1;
    if (kickers & ACE_LOW_BIT)
        kickers++;
    setKickerBits(kickers);

    switch (type())
    {
        case TWO_PAIR:
        case FULL_HOUSE:
            setMinorRank((minorRank().code() + 1) % Rank::NUM_RANK);

        case ONE_PAIR:
        case THREE_OF_A_KIND:
        case STRAIGHT:
        case FOUR_OF_A_KIND:
        case STRAIGHT_FLUSH:
            setMajorRank((majorRank().code() + 1) % Rank::NUM_RANK);
            break;
    };
}

void PokerEvaluation::playAceHigh()
{
    if (acePlaysLow() == false)
        return;

    _evalcode ^= ACE_LOW_BIT;

    // make room for the ace in kicker land, and if
    // we have an ace, push it to the bottom
    int kickers = kickerBits();
    if (kickers & 0x01)
        kickers |= ACE_LOW_BIT;
    kickers = kickers >> 1;
    setKickerBits(kickers);
}

bool PokerEvaluation::acePlaysLow() const
{
    if (_evalcode & ACE_LOW_BIT)
        return true;
    return false;
}

void PokerEvaluation::fixWheel2to7(int rankMask)
{
    if (type() == STRAIGHT && majorRank() == Rank::Five())
    {
        _evalcode = (NO_PAIR << VSHIFT) ^ rankMask;
    }
    else if (type() == STRAIGHT_FLUSH && majorRank() == Rank::Five())
    {
        _evalcode = (FLUSH << VSHIFT) ^ rankMask;
    }
}

string PokerEvaluation::makeRankString(int r, bool acelow) const
{
    if (acelow)
    {
        if (r == 0)
            r = Rank::NUM_RANK - 1;
        else
            r--;
    }

    Rank rank(r);
    return rank.str();
}

string PokerEvaluation::strKickers(int n) const
{
    string out;
    for (int i = Rank::NUM_RANK - 1; i >= 0; i--)
        if ((n & (0x01 << i)) > 0)
        {
            //      Rank r(i);
            //      out += r.toString ();
            out += makeRankString(i, acePlaysLow());
        }
    return out;
}

string PokerEvaluation::strTop(int n) const
{
    int i = (n >> 20) & 0x0F;
    //    Rank r(i);
    //    return r.toString ();
    return makeRankString(i, acePlaysLow());
}

string PokerEvaluation::strBot(int n) const
{
    int i = (n >> 16) & 0x0F;
    //    Rank r(i);
    //    return r.toString ();
    return makeRankString(i, acePlaysLow());
}

string PokerEvaluation::bitstr() const
{
    string ret;
    int n = _evalcode;
    for (int i = 31; i >= 0; i--)
    {
        if ((n & (0x01 << i)) > 0)
            ret += "1";
        else
            ret += "0";
        if (i % 8 == 0)
            ret += " ";
    }
    return ret;
}

string PokerEvaluation::str() const
{
    string ret;
    int n = _evalcode;

    if (n == 0)
        return "";

    if (n != 0)
    {
        if (isFlipped())
        {
            PokerEvaluation e(_evalcode);
            e.flip();
            return e.str();
        }
        // ret += " ";
        ret += toStringCannon();
    }
    return ret;
}

string PokerEvaluation::toStringCannon() const
{
  const string highcard       = "high card:    ";
  const string onepair        = "one pair:     ";
  const string threeflush     = "three flush:  ";
  const string threestraight  = "three str8:   ";
  const string twopair        = "two pair:     ";
  const string trips          = "trips:        ";
  const string threestr8flush = "3 str8 flush: ";
  const string straight       = "straight:     ";
  const string flush          = "flush:        ";
  const string fullhouse      = "full house:   ";
  const string quads          = "quads:        ";
  const string straightflush  = "str8 flush:   ";

    string ret;
    int n = _evalcode;

    if (isFlipped())
    {
        PokerEvaluation e(_evalcode);
        e.flip();
        return e.toStringCannon();
    }

    string hand;
    string topr;
    string botr;
    string kick;

    int val = n >> 24;
    switch (val)
    {
        case NO_PAIR:
            hand += highcard;
            kick += strKickers(n);
            break;

        case ONE_PAIR:
            hand += onepair;
            topr += strTop(n);
            kick += strKickers(n);
            break;

        case THREE_FLUSH:
            hand += threeflush;
            kick += strKickers(n);
            break;

        case THREE_STRAIGHT:
            hand += threestraight;
            topr += strTop(n);
            break;

        case TWO_PAIR:
            hand += twopair;
            topr += strTop(n);
            botr += strBot(n);
            kick += strKickers(n);
            break;

        case THREE_OF_A_KIND:
            hand += trips;
            topr += strTop(n);
            kick += strKickers(n);
            break;

        case THREE_STRAIGHT_FLUSH:
            hand += threestr8flush;
            topr += strTop(n);
            break;

        case STRAIGHT:
            hand += straight;
            topr += strTop(n);
            break;

        case FLUSH:
            hand += flush;
            topr += strKickers(n);
            break;

        case FULL_HOUSE:
            hand += fullhouse;
            topr += strTop(n);
            botr += strBot(n);
            break;

        case FOUR_OF_A_KIND:
            hand += quads;
            topr += strTop(n);
            botr += strKickers(n);
            break;

        case STRAIGHT_FLUSH:
            hand += straightflush;
            kick += strTop(n);
            break;
    }

    string ranks = topr + botr + kick;
    ret = (boost::format("%s %-5s") % hand % ranks).str();

    return ret;
}

void PokerEvaluation::generateLowballLookupA5()
{
    cout << "const int lowballA5Ranks[] = {\n";
    // this is a 13 bit lookup table
    for (int i = 0; i <= 0x1FFF; i++)
    {
        // ok, we are passing in plain old rankbits, but
        // geting A-5 lowball bits out, let's convert first off
        int bits = i << 1;
        if (bits & ACE_LOW_BIT)
            bits = bits - ACE_LOW_BIT + 1;

        int lowbits = 0;
        int nbits = 0;
        for (int b = 0; b < 13; b++)
        {
            if (bits & 0x01 << b)
            {
                lowbits += 0x01 << b;
                nbits++;
            }
            if (nbits == 5)
                break;
        }
        cout << boost::format("    %6d, // %s\n") % lowbits % toBitString(lowbits);
    }
    cout << "};\n";
}

void PokerEvaluation::generateBottomRankMask()
{
    cout << "const int bottomRankMask[] = {\n";
    // this is a 13 bit lookup table
    for (int i = 0; i <= 0x1FFF; i++)
    {
        int bits = i;
        int lowbits = 0;
        for (int b = 0; b < 13; b++)
        {
            if (bits & 0x01 << b)
            {
                lowbits += 0x01 << b;
                break;
            }
        }
        cout << boost::format("    %6d, // %s\n") % lowbits % toBitString(lowbits);
    }
    cout << "};\n";
}

std::ostream& operator<<(std::ostream& sout, const pokerstove::PokerEvaluation& e)
{
    sout << e.str();
    return sout;
}
