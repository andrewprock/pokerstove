/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 */
#include "CardDistribution.h"

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/math/special_functions/binomial.hpp>
#include <iostream>
#include <vector>
#include <pokerstove/peval/Card.h>
#include <pokerstove/util/combinations.h>

using namespace std;
using namespace pokerstove;

CardDistribution::CardDistribution()
    : _handList(1, CardSet())
    , _weights()
{
    _weights[CardSet()] = 1.0;
}

CardDistribution::CardDistribution(const CardSet& cs)
    : _handList(1, cs)
    , _weights()
{
    _weights[cs] = 1.0;
}

CardDistribution::CardDistribution(const CardDistribution& cd)
{
    *this = cd;
}

CardDistribution& CardDistribution::operator=(const CardDistribution& other)
{
    _handList = other._handList;
    _weights = other._weights;
    return *this;
}

size_t CardDistribution::size() const
{
    return _handList.size();
}

string CardDistribution::str() const
{
    string ret;
    for (size_t i = 0; i < _handList.size(); i++) {
        const CardSet& hand = _handList[i];
        ret += (i > 0 ? "," : "")
            + (boost::format("%s=%.3f") % hand.str() % weight(hand)).str();
    }
    return ret;
}

void CardDistribution::clear()
{
    _handList.clear();
    _weights.clear();
}

CardDistribution CardDistribution::data() const
{
    return *this;
}

void CardDistribution::fill(int n)
{
    CardSet cards;
    cards.fill();
    fill(cards, n);
}

void CardDistribution::fill(const CardSet& cs, int n)
{
    vector<Card> cards = cs.cards();
    int setsize = static_cast<int>(cards.size());
    combinations hands(setsize, n);
    int vsize = boost::math::binomial_coefficient<double>(setsize, n);
    clear();
    _handList.reserve(vsize);

    do {
        CardSet cs;
        for (int i = 0; i < n; i++)
            cs.insert(cards[hands[i]]);
        _handList.push_back(cs);
        _weights[cs] = 1.0;
    } while (hands.next());
}

const CardSet& CardDistribution::operator[](size_t index) const
{
    if (index >= _handList.size())
        throw std::runtime_error("CardDistribution::operator: bounds error");
    return _handList[index];
}

const double& CardDistribution::operator[](const CardSet& hand) const
{
    static const double kStaticZero = 0.0;  // for hands not in distribution
    map<CardSet, double>::const_iterator it = _weights.find(hand);
    if (it == _weights.end())
        return kStaticZero;
    return it->second;
}

double& CardDistribution::operator[](const CardSet& hand)
{
    return _weights[hand];
}

// this is a simple wrapper to make reading the code easier
const double& CardDistribution::weight(const CardSet& hand) const
{
    return (*this)[hand];
}

bool CardDistribution::parse(const std::string& input)
{
    clear();

    // trap the special "unrealized/empty" distribution which
    // has no specific cards, and is equivalent to a "random" dist
    // TODO: verify that this is the internal representation we want for a
    // random hand
    if (input == ".") {
        CardSet hand;
        if (hand.size() != 0)
            return false;
        _weights[hand] = 1.0;
        _handList.push_back(hand);
        return true;
    }

    vector<string> hands;
    boost::split(hands, input, boost::is_any_of(","));
    for (const string& h : hands) {
        // handle the weight
        double weight = 1.0;
        if (boost::contains(h, "=")) {
            // trap for the case where the input ends with "="
            string::size_type weightPos = h.rfind("=") + 1;
            if (weightPos == h.size())
                return false;
            weight = boost::lexical_cast<double>(h.substr(weightPos));
        }

        // handle the hand one card at a time.
        string handstr = h.substr(0, h.rfind("="));
        if (handstr.size() % 2 != 0)
            return false;
        CardSet hand;
        for (size_t i = 0; i < handstr.size(); i += 2) {
            Card c;
            if (!c.fromString(handstr.substr(i, i + 2)))
                return false;
            if (hand.contains(c))
                return false;
            hand.insert(c);
        }

        // final check and
        if (hand.size() == 0)
            return false;
        _weights[hand] = weight;
        _handList.push_back(hand);
    }
    return true;
}

std::string CardDistribution::display() const
{
    return str();
}

void CardDistribution::removeCards(const CardSet& dead)
{
    for (size_t i = 0; i < _handList.size(); i++)
        if (_handList[i].intersects(dead))
            _weights[_handList[i]] = 0.0;
}

double CardDistribution::weight() const
{
    double total = 0.0;
    for (std::pair<CardSet, double> w : _weights) {
        total += w.second;
    }
    return total;
}
