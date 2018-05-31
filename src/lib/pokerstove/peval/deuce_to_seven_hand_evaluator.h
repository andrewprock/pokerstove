/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 * $Id: DeuceToSevenHandEvaluator.h 2649 2012-06-30 04:53:24Z prock $
 */
#ifndef POKERSTOVE_PEVAL_DEUCE_TO_SEVEN_HAND_EVALUATOR_H_
#define POKERSTOVE_PEVAL_DEUCE_TO_SEVEN_HAND_EVALUATOR_H_

#include "poker_hand_evaluator.h"

namespace pokerstove {
/**
 * A specialized hand evaluator for hold'em.  Not as slow.
 */
class DeuceToSevenHandEvaluator : public PokerHandEvaluator {
 public:
  DeuceToSevenHandEvaluator() : PokerHandEvaluator(), _numDraws(0) {}

  virtual PokerHandEvaluation evaluateHand(const CardSet& hand,
                                           const CardSet&) const {
    if (usesSuits())
      return PokerHandEvaluation(hand.evaluateLow2to7());
    else
      return PokerHandEvaluation(hand.evaluateRanksLow2to7());
  }

  virtual PokerEvaluation evaluateRanks(
      const CardSet& hand, const CardSet& board = CardSet(0)) const {
    return hand.evaluateRanksLow2to7();
  }

  virtual PokerEvaluation evaluateSuits(
      const CardSet& hand, const CardSet& board = CardSet(0)) const {
    return hand.evaluateSuitsLow2to7();
  }

  virtual size_t handSize() const { return 5; }
  virtual size_t boardSize() const { return 0; }
  virtual size_t evaluationSize() const { return 1; }
  virtual size_t numDraws() const { return _numDraws; }
  virtual void setNumDraws(size_t sz) { _numDraws = sz; }

 private:
  size_t _numDraws;
};

}  // namespace pokerstove

#endif  // POKERSTOVE_PEVAL_DEUCE_TO_SEVEN_HAND_EVALUATOR_H_
