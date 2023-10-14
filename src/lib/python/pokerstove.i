%module pokerstove

%include <stdint.i>
%include <std_string.i>

// NOTE: game evaluators are available through the
//       PokerHandEvaluator factory
//
// peval interfaces not supported in the python bridge:
// * CardSetGenerators.h
// * PokerEvaluationTables.h
//
// penum interfaces not yet supported
// * CardDistribution.h
// * Odometer.h
// * PartitionEnumerator.h
// * ShowdownEnumerator.h

%{
#include "pokerstove/peval/Rank.h"
#include "pokerstove/peval/Suit.h"
#include "pokerstove/peval/Card.h"
#include "pokerstove/peval/CardSet.h"
#include "pokerstove/peval/PokerHand.h"
#include "pokerstove/peval/PokerEvaluation.h"
#include "pokerstove/peval/PokerEvaluationTables.h"
#include "pokerstove/peval/PokerHandEvaluation.h"
#include "pokerstove/peval/PokerHandEvaluator.h"
#include "pokerstove/peval/Holdem.h"
#include "pokerstove/penum/SimpleDeck.hpp"
#include "pokerstove/penum/CardDistribution.h"
%}

// take care of some shared pointer issues
// TODO: migrate to std::shared_ptr
%include <boost_shared_ptr.i>
%shared_ptr(pokerstove::PokerHandEvaluator)


%include "pokerstove/peval/Rank.h"
%include "pokerstove/peval/Suit.h"
%include "pokerstove/peval/Card.h"
%include "pokerstove/peval/CardSet.h"
%include "pokerstove/peval/PokerHand.h"
%include "pokerstove/peval/PokerEvaluation.h"
%include "pokerstove/peval/PokerEvaluationTables.h"
%include "pokerstove/peval/PokerHandEvaluation.h"
%include "pokerstove/peval/PokerHandEvaluator.h"
%include "pokerstove/peval/Holdem.h"
%include "pokerstove/penum/SimpleDeck.hpp"
%include "pokerstove/penum/CardDistribution.h"


// support __repr__() with extension boilerplate
%extend pokerstove::Rank {
std::string __repr__()
{
    return $self->str();
}
}

%extend pokerstove::Suit {
std::string __repr__()
{
    return $self->str();
}
}

%extend pokerstove::Card {
std::string __repr__()
{
    return $self->str();
}
}

%extend pokerstove::CardSet {
std::string __repr__()
{
    return $self->str();
}
}

%extend pokerstove::PokerHand {
std::string __repr__()
{
    return $self->str();
}
}

%extend pokerstove::PokerEvaluation {
std::string __repr__()
{
    return $self->str();
}
}

%extend pokerstove::PokerHandEvaluation {
std::string __repr__()
{
    return $self->str();
}
}

%extend pokerstove::SimpleDeck {
std::string __repr__()
{
    return $self->str();
}
}

