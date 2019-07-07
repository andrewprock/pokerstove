/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 * $Id: StudHandEvaluator.h 2649 2012-06-30 04:53:24Z prock $
 */
#ifndef PEVAL_STUDHANDEVALUATOR_H_
#define PEVAL_STUDHANDEVALUATOR_H_

#include "PokerHandEvaluator.h"

namespace pokerstove
{
/**
 * A specialized hand evaluator for hold'em.  Not as slow.
 */
class StudHandEvaluator : public PokerHandEvaluator
{
public:
    virtual PokerHandEvaluation evaluateHand(const CardSet& hand, const CardSet&) const
    {
        // return hand.evaluateHighRanks ();
        return PokerHandEvaluation(hand.evaluateHigh());
    }

    virtual PokerEvaluation evaluateRanks(const CardSet& hand, const CardSet& board = CardSet(0)) const
    {
        return hand.evaluateHighRanks();
    }

    virtual PokerEvaluation evaluateSuits(const CardSet& hand, const CardSet& board = CardSet(0)) const
    {
        return hand.evaluateHighFlush();
    }

    virtual size_t handSize() const { return 7; }
    virtual size_t boardSize() const { return 0; }
    virtual size_t evaluationSize() const { return 1; }
};

}  // namespace pokerstove
#endif  // PEVAL_STUDHANDEVALUATOR_H_
