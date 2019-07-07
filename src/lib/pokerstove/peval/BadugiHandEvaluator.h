/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 * $Id: BadugiHandEvaluator.h 2649 2012-06-30 04:53:24Z prock $
 */
#ifndef PEVAL_BADUGIHANDEVALUATOR_H_
#define PEVAL_BADUGIHANDEVALUATOR_H_

#include "PokerHandEvaluator.h"

namespace pokerstove
{
/**
 * A specialized hand evaluator for hold'em.  Not as slow.
 */
class BadugiHandEvaluator : public PokerHandEvaluator
{
public:
    BadugiHandEvaluator()
        : PokerHandEvaluator()
        , _numDraws(0)
    {}

    virtual PokerHandEvaluation evaluateHand(const CardSet& hand, const CardSet&) const
    {
        return PokerHandEvaluation(hand.evaluateBadugi());
    }

    virtual PokerEvaluation evaluateRanks(const CardSet& hand, const CardSet& board = CardSet(0)) const
    {
        throw std::runtime_error("BadugiHandEvaluator::evaluateRanks, not implemented");
    }

    virtual PokerEvaluation evaluateSuits(const CardSet& hand, const CardSet& board = CardSet(0)) const
    {
        throw std::runtime_error("BadugiHandEvaluator::evaluateSuits, not implemented");
    }

    virtual size_t handSize() const { return 4; }
    virtual size_t boardSize() const { return 0; }
    virtual size_t evaluationSize() const { return 1; }
    virtual size_t numDraws() const { return _numDraws; }
    virtual void setNumDraws(size_t sz) { _numDraws = sz; }

private:
    size_t _numDraws;
};

}  // namespace pokerstove
#endif  // PEVAL_BADUGIHANDEVALUATOR_H_
