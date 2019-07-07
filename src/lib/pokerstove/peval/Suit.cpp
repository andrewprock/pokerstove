/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 * $Id: Suit.cpp 2649 2012-06-30 04:53:24Z prock $
 */
#include "Suit.h"
#include "Card.h"
#include <boost/format.hpp>
#include <iostream>
#include <stdexcept>

using namespace std;
using namespace pokerstove;

Suit::display Suit::__suitStringType = SUIT_ASCII;

void Suit::setSuitStringType(Suit::display s) { __suitStringType = s; }

Suit::display Suit::getSuitStringType() { return __suitStringType; }

string Suit::decodeSuitANSI_EXT(int c) const
{
    if (c < 0)
        return "x";

    // use extended ANSI colors to print cards
    // see: http://www.vias.org/linux-knowhow/lnag_05_05_04.html
    switch (c)
    {
        case 0:
            return "\033[32m\05\033[0m";
        case 1:
            return "\033[36m\04\033[0m";
        case 2:
            return "\033[31m\03\033[0m";
        case 3:
            return "\033[33m\06\033[0m";
        default:
            return "#";
    }
    return "?";
}

string Suit::decodeSuitUnicode(int c) const
{
    if (c < 0)
        return "x";

    switch (c)
    {
        case 0:
            // return "\033[32m\2663\033[0m";
            return "\033[32mc\033[0m";
        case 1:
            // return "\033[36m\2666\033[0m";
            return "\033[36md\033[0m";
        case 2:
            // return "\033[31m\2665\033[0m";
            return "\033[31mh\033[0m";
        case 3:
            // return "\033[33m\2660\033[0m";
            return "\033[33ms\033[0m";
        default:
            return "#";
    }
    return "?";
}

string Suit::decodeSuitASCII(int c) const
{
    if (c < 0)
        return "x";

    switch (c)
    {
        case 0:
            return "c";
        case 1:
            return "d";
        case 2:
            return "h";
        case 3:
            return "s";
        default:
            return "?";
    }
    return "?";
}

string Suit::decodeSuitASCII_EXT(int card) const
{
    if (card < 0)
        return "x";

    int c = '?';
    switch (card)
    {
        case 0:
            c = 5;
            break;
        case 1:
            c = 4;
            break;
        case 2:
            c = 3;
            break;
        case 3:
            c = 6;
            break;
        default:
            c = '?';
    }

    string buf = (boost::format("%c") % c).str();  // boost::format may be overkill
    return buf;
}

string Suit::decodeSuitHTML(int c) const
{
    switch (c)
    {
        case 0:
            return "&clubs;";
        case 1:
            return "&diams;";
        case 2:
            return "&hearts;";
        case 3:
            return "&spades;";
        default:
            return "?";
    }
}

string Suit::decodeSuit4ColorHTML(int c) const
{
    string suit = "<font ";

    switch (c)
    {
        case 0:
            suit += "color=#008000>";
            break;
        case 1:
            suit += "color=#3333FF>";
            break;
        case 2:
            suit += "color=#FF0000>";
            break;
        case 3:
            suit += "color=#000000>";
            break;
        default:
            return "?";
    }

    suit += decodeSuitHTML(c);
    suit += "</font>";

    return suit;
}

string Suit::decodeSuit2ColorHTML(int c) const
{
    string suit = "<font ";

    switch (c)
    {
        case 1:
        case 2:
            suit += "color=#FF0000>";
            break;

        case 0:
        case 3:
            suit += "color=#000000>";
            break;
        default:
            return "?";
    }

    suit += decodeSuitHTML(c);
    suit += "</font>";

    return suit;
}

string Suit::decodeSuit(int c) const
{
    switch (__suitStringType)
    {
        case SUIT_ASCII:
            return decodeSuitASCII(c);

        case SUIT_ASCII_EXTENDED:
            return decodeSuitASCII_EXT(c);

        case SUIT_HTML_2COLOR:
            return decodeSuit2ColorHTML(c);

        case SUIT_HTML_4COLOR:
            return decodeSuit4ColorHTML(c);

        case SUIT_HTML:
            return decodeSuitHTML(c);

        case SUIT_ANSI_EXT_COLOR:
            return decodeSuitANSI_EXT(c);

        case SUIT_UNICODE:
            return decodeSuitUnicode(c);

        case SUIT_PREFLOP_CANNON:
            throw std::domain_error("direct suit decoding not supported for SUIT_PREFLOP_CANNON");
    }
    return "?";
}

int Suit::suit_code(char c)
{
    switch (c)
    {
        case 'c':
        case 'C':
            return Suit::ClubVal();
        case 'd':
        case 'D':
            return Suit::DiamondVal();
        case 'h':
        case 'H':
            return Suit::HeartVal();
        case 's':
        case 'S':
            return Suit::SpadeVal();
    };
    throw std::invalid_argument((string("Suit, parse error: ") + c).c_str());
}

Suit::Suit(const std::string& str)
    : _suit(suit_code(str[0]))
{}

Suit::Suit(uint8_t c)
{
    if (isSuitChar(c))
        _suit = suit_code(c);
    else
        _suit = c;
}

string Suit::str() const { return decodeSuit(_suit); }

void Suit::fromString(const string& c) { _suit = suit_code(c[0]); }

bool Suit::isSuitChar(char c)
{
    switch (c)
    {
        case 'c':    case 'C':
        case 'd':    case 'D':
        case 'h':    case 'H':
        case 's':    case 'S':
            return true;
    };
    return false;
}
