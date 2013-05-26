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
    switch (c[0])
    {
        case '2':      encode(RANK_TWO);   return;
        case '3':      encode(RANK_THREE); return;
        case '4':      encode(RANK_FOUR);  return;
        case '5':      encode(RANK_FIVE);  return;
        case '6':      encode(RANK_SIX);   return;
        case '7':      encode(RANK_SEVEN); return;
        case '8':      encode(RANK_EIGHT); return;
        case '9':      encode(RANK_NINE);  return;
        case 't':      encode(RANK_TEN);   return;
        case 'T':      encode(RANK_TEN);   return;
        case 'j':
        case 'J':      encode(RANK_JACK);  return;
        case 'q':
        case 'Q':      encode(RANK_QUEEN); return;
        case 'k':
        case 'K':      encode(RANK_KING);  return;
        case 'a':
        case 'A':      encode(RANK_ACE);   return;
    }
    throw std::domain_error(string("rank parse error: " + c).c_str());
}

bool Rank::isRankChar(char c)
{
    switch (c)
    {
        case '2':    case '3':    case '4':    case '5':
        case '6':    case '7':    case '8':    case '9':
        case 't':    case 'T':    case 'j':    case 'J':
        case 'q':    case 'Q':    case 'k':    case 'K':
        case 'a':    case 'A':
            return true;
    }
    return false;
}
