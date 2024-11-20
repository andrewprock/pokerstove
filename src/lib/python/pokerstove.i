%module pokerstove

%include <stdint.i>
%include <std_string.i>
%include <std_vector.i>

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

%template(CardVector) std::vector<pokerstove::Card>;
%template(PokerEvaluationVector) std::vector<pokerstove::PokerEvaluation>;

// take care of some shared pointer issues
%include <std_shared_ptr.i>
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
std::string __repr__() {
    return $self->str();
}

long __hash__() {
    return (long)$self->code();
}
}

%extend pokerstove::Suit {
std::string __repr__() {
    return $self->str();
}

long __hash__() {
    return (long)$self->code();
}
}

%extend pokerstove::Card {
std::string __repr__()
{
    return $self->str();
}

long __hash__() {
    return (long)$self->code();
}
}

%extend pokerstove::CardSet {
std::string __repr__() {
    return $self->str();
}

// this will truncate if sizeof(long) < 8 bytes. This should be ok
// as hashes are not expected to be unique for python.
long __hash__() {
    return (long)$self->mask();
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

