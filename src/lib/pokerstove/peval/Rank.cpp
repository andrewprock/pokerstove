/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 * $Id: Rank.cpp 2649 2012-06-30 04:53:24Z prock $
 */
#include <stdexcept>
#include "Rank.h"

using namespace std;
using namespace pokerstove;

Rank::Rank(const string& c)
{
    fromString(c);
}

Rank::Rank(uint8_t code)
{
    if (code <= RANK_ACE)
        _rank = code;
    else
        _rank = rank_code(code);
}

string Rank::str() const
{
    switch (_rank)
    {
        case RANK_TWO:      return "2";
        case RANK_THREE:    return "3";
        case RANK_FOUR:     return "4";
        case RANK_FIVE:     return "5";
        case RANK_SIX:      return "6";
        case RANK_SEVEN:    return "7";
        case RANK_EIGHT:    return "8";
        case RANK_NINE:     return "9";
        case RANK_TEN:      return "T";
        case RANK_JACK:     return "J";
        case RANK_QUEEN:    return "Q";
        case RANK_KING:     return "K";
        case RANK_ACE:      return "A";
    }
    return "?";
}

void Rank::fromString(const string& c)
{
    int code = rank_code(c[0]);
    if (code >= 0)
        _rank = code;
    else
        throw std::domain_error(string("rank parse error: " + c).c_str());
}

int Rank::rank_code(char c)
{
    switch (c)
    {
        case '2':      return Rank::RANK_TWO;
        case '3':      return Rank::RANK_THREE;
        case '4':      return Rank::RANK_FOUR;
        case '5':      return Rank::RANK_FIVE;
        case '6':      return Rank::RANK_SIX;
        case '7':      return Rank::RANK_SEVEN;
        case '8':      return Rank::RANK_EIGHT;
        case '9':      return Rank::RANK_NINE;
        case 't':
        case 'T':      return Rank::RANK_TEN;
        case 'j':
        case 'J':      return Rank::RANK_JACK;
        case 'q':
        case 'Q':      return Rank::RANK_QUEEN;
        case 'k':
        case 'K':      return Rank::RANK_KING;
        case 'a':
        case 'A':      return Rank::RANK_ACE;
    }
    return -1;
}

bool Rank::isRankChar(char c)
{
    if (rank_code(c) >= 0)
        return true;
    return false;
}
