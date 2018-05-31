/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 * $Id: OmahaEightHandEvaluator.h 2649 2012-06-30 04:53:24Z prock $
 */
#ifndef POKERSTOVE_PEVAL_OMAHA_EIGHT_HAND_EVALUATOR_H_
#define POKERSTOVE_PEVAL_OMAHA_EIGHT_HAND_EVALUATOR_H_

#include <boost/math/special_functions/binomial.hpp>

#include "holdem.h"
#include "poker_evaluation_tables.h"
#include "poker_hand_evaluator.h"

inline int bottomRanks(int x, int n) {
  int ret = 0;
  for (int i = 0; i < n; i++) {
    if (x == 0)
      return ret;
    ret |= (-x & x);
    x ^= (-x & x);
  }
  return ret;
}

inline int flipAce(int ranks) {
  return ((ranks & ~(1 << 12)) << 1) | ((ranks >> 12) & 0x01);
}

inline int unflipAce(int ranks) {
  return (ranks >> 1) | ((ranks & 0x01) << 12);
}

namespace pokerstove {
/**
 * A specialized hand evaluator for hold'em.  Not as slow.
 */
class OmahaEightHandEvaluator : public PokerHandEvaluator {
 public:
  static const int NUM_OMAHA_POCKET = 4;
  static const int NUM_OMAHA_FLOP = 3;
  static const int NUM_OMAHA_RIVER = 5;
  static const int NUM_OMAHA_HAND_USE = 2;
  static const int NUM_OMAHA_FLUSH_BOARD = 3;

  /*
    How to do the evaluation using the universal evaluator
  UniversalHandEvaluator (int heromin, int heromax,
                          int boardmin, int boardmax,
                          int herouse,
                          evalFunction evalA, evalFunction evalB)

                          4,4,3,5,2,

   */

  virtual PokerHandEvaluation evaluateHand(const CardSet& hand,
                                           const CardSet& board) const {
    PokerEvaluation eval[2];

    // generate the possible sub hands, player hand candidates are all
    // 4c2 combinations of hands cards,
    // board candidates are all Nc3 board candidates, where N is the size
    // of the board
    double combos = boost::math::binomial_coefficient<double>(board.size(), 3);
    std::vector<CardSet> board_candidates(static_cast<size_t>(combos));
    std::vector<CardSet> hand_candidates(6);
    fillHands(hand_candidates, hand);
    fillBoards(board_candidates, board);

    for (size_t i = 0; i < hand_candidates.size(); i++)
      for (size_t j = 0; j < board_candidates.size(); j++) {
        PokerEvaluation e =
            CardSet(hand_candidates[i] | board_candidates[j]).evaluateHigh();
        if (e > eval[0])
          eval[0] = e;
      }

    // evaluate the low using brec's technique, see:
    // http://groups.google.com/group/rec.gambling.poker/msg/e8a3a7698d51f04a?dmode=source
    //
    // Nominally, it takes 60 evaluations to find a player's Omaha-8 low --
    // 4C2=6 sets of 2 hole cards times 5C3=10 sets of 3 board cards.
    // However, each of the 6 sets of hole cards can be evaluated in one
    // multi-step operation as follows:
    //
    //     Let H be a bit vector (typically an integer variable) of the
    //     ranks of
    // the two hole cards; H has 1 or 2 bits set;
    //     Let B be a bit vector of the ranks of the five board cards; B has
    //     2 to 5
    // bits set
    //     Let L3[v] be a bit vector of the lowest (up to) 3 bits set in v;
    //     L3[v]
    // can be obtained by table lookup;
    //     Let L5[v] be a bit vector of the lowest 5 bits set in v, or a
    //     special
    // "no low" value to indicate that v has fewer than 5 bits set or that
    // not all bits in v represent ranks of 8 or lower;  L5[v] can be
    // obtained by table lookup.
    //
    // Then the low value, represented as a bit vector of ranks, is
    //     L5[L3[B & (~H)] | H]
    // where &, ~, and | represent bitwise AND, NOT, and OR respectively.
    // This represents the operation of finding the lowest three board ranks
    // not present in the hole cards, and adding the hole cards to make the
    // 5-card low hand.

    int bmask = flipAce(board.rankMask() & 0x107F);
    if (nRanksTable[bmask] >= 3) {
      for (size_t i = 0; i < hand_candidates.size(); i++) {
        int hmask = flipAce(hand_candidates[i].rankMask() & 0x107F);
        if (nRanksTable[hmask] < 2)
          continue;
        CardSet lowRanks(unflipAce(
            bottomRanks(bottomRanks(bmask & (~hmask), 3) | hmask, 5)));
        PokerEvaluation e = lowRanks.evaluate8LowA5();
        if (e > eval[1]) {
          eval[1] = e;
        }
      }
    }

    return PokerHandEvaluation(eval[0], eval[1]);
  }

  /**
   * This is a convenience function which returns the Low evaluation only
   * for a specific pair of cards.
   */
  PokerEvaluation evaluateTwoCardLow(const CardSet& twocard,
                                     const CardSet& board) const {
    int bmask = flipAce(board.rankMask() & 0x107F);
    int hmask = flipAce(twocard.rankMask() & 0x107F);
    if (nRanksTable[hmask] < 2)
      return PokerEvaluation();
    CardSet lowRanks(
        unflipAce(bottomRanks(bottomRanks(bmask & (~hmask), 3) | hmask, 5)));
    return lowRanks.evaluate8LowA5();
  }

  PokerEvaluation evaluateLow(const CardSet& hand, const CardSet& board) const {
    PokerEvaluation eval;
    std::vector<CardSet> hand_candidates(6);
    fillHands(hand_candidates, hand);
    int bmask = flipAce(board.rankMask() & 0x107F);
    if (nRanksTable[bmask] >= 3) {
      for (size_t i = 0; i < hand_candidates.size(); i++) {
        int hmask = flipAce(hand_candidates[i].rankMask() & 0x107F);
        if (nRanksTable[hmask] < 2)
          continue;
        CardSet lowRanks(unflipAce(
            bottomRanks(bottomRanks(bmask & (~hmask), 3) | hmask, 5)));
        PokerEvaluation e = lowRanks.evaluate8LowA5();
        if (e > eval) {
          eval = e;
        }
      }
    }
    return eval;
  }

  void fillHands(std::vector<CardSet>& candidates, const CardSet& cards) const {
    std::vector<CardSet> clist = cards.cardSets();
    candidates[0] = (clist[0] | clist[1]);
    candidates[1] = (clist[0] | clist[2]);
    candidates[2] = (clist[0] | clist[3]);
    candidates[3] = (clist[1] | clist[2]);
    candidates[4] = (clist[1] | clist[3]);
    candidates[5] = (clist[2] | clist[3]);
  }

  void fillBoards(std::vector<CardSet>& candidates,
                  const CardSet& cards) const {
    std::vector<CardSet> clist = cards.cardSets();
    switch (clist.size()) {
      case 5:
        candidates[4] = (clist[0] | clist[1] | clist[4]);
        candidates[5] = (clist[0] | clist[2] | clist[4]);
        candidates[6] = (clist[0] | clist[3] | clist[4]);
        candidates[7] = (clist[1] | clist[2] | clist[4]);
        candidates[8] = (clist[1] | clist[3] | clist[4]);
        candidates[9] = (clist[2] | clist[3] | clist[4]);
        // fall through

      case 4:
        candidates[1] = (clist[0] | clist[1] | clist[3]);
        candidates[2] = (clist[0] | clist[2] | clist[3]);
        candidates[3] = (clist[1] | clist[2] | clist[3]);
        // fall through

      case 3:
        candidates[0] = (clist[0] | clist[1] | clist[2]);
    }
  }

  virtual size_t handSize() const { return NUM_OMAHA_POCKET; }
  virtual size_t boardSize() const { return BOARD_SIZE; }
  virtual size_t evaluationSize() const { return 2; }
};

}  // namespace pokerstove

#endif  // POKERSTOVE_PEVAL_OMAHA_EIGHT_HAND_EVALUATOR_H_
