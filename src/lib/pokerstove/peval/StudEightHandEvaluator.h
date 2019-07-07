/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 * $Id: StudEightHandEvaluator.h 2649 2012-06-30 04:53:24Z prock $
 */
#ifndef PEVAL_STUDEIGHTHANDEVALUATOR_H_
#define PEVAL_STUDEIGHTHANDEVALUATOR_H_

#include "PokerHandEvaluator.h"

namespace pokerstove
{
/**
 * A specialized hand evaluator for hold'em.  Not as slow.
 */
class StudEightHandEvaluator : public PokerHandEvaluator
{
public:
    virtual PokerHandEvaluation evaluateHand(const CardSet& hand, const CardSet&) const
    {
        return PokerHandEvaluation(hand.evaluateHigh(), hand.evaluate8LowA5());
    }

    virtual size_t handSize() const { return 7; }
    virtual size_t boardSize() const { return 0; }
    virtual size_t evaluationSize() const { return 2; }
};

}  // namespace pokerstove
#endif  // PEVAL_STUDEIGHTHANDEVALUATOR_H_
