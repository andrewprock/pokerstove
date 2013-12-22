/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 * $Id: UniversalHandEvaluator.h 2649 2012-06-30 04:53:24Z prock $
 */
#ifndef PEVAL_UNIVERSALHANDEVALUATOR_H_
#define PEVAL_UNIVERSALHANDEVALUATOR_H_

// This is a rule based hand evaluator which can be used to construct
// evalutors for most kinds of poker evaluation types.  Because it is
// a generic code base, it is fairly slow, but allows for rule driven
// poker games to be evaluated, and thus is more flexible.
//
// The current configurable parameters are:
//  hero hand: min/max cards
//  board: min/max cards
//  eval a: high/low/227/A25/Badugi/3CP
//  eval b: high/low/227/A25/Badugi/3CP

#include <vector>
#include <boost/lexical_cast.hpp>
#include <pokerstove/util/combinations.h>
#include "Card.h"
#include "CardSet.h"
#include "PokerHandEvaluator.h"

namespace pokerstove
{
typedef PokerEvaluation(CardSet::*evalFunction)() const;

/**
 * A generic poker game hand evaluator, from which nearly all poker evaluators can be made.  This class
 * is used as the default class type in the PokerHandEvaluator factory.
 */
class UniversalHandEvaluator : public PokerHandEvaluator
{
public:

    /**
     * There is only one constructor which takes all the parameters.
     * The evaluation varieties are specified by pointer to member
     * functions to the CardSet class.
     *
     * @param heromin smallest possible hero hand
     * @param heromax largest possible hero hand
     * @param boardmin smallest possible board
     * @param boardmax largest possible board
     * @param herouse num cards hero must use (e.g. omaha->2)
     * @param evalA primary evaluation function, all games must have
     * @param evalB secondary evaluation function, most games use none (null)
     */
    UniversalHandEvaluator(int heromin, int heromax,
                           int boardmin, int boardmax,
                           int herouse,
                           evalFunction evalA, evalFunction evalB)

        : PokerHandEvaluator()
        , _heromin(heromin)
        , _heromax(heromax)
        , _boardmin(boardmin)
        , _boardmax(boardmax)
        , _herouse(herouse)
        , _evalA(evalA)
        , _evalB(evalB)
    {
        if (evalA == evalFunction(NULL))
            throw std::invalid_argument("UnivHandEval: first evaluator (A) must be non-null");

        if (evalB == evalFunction(NULL))
            _evalsperhand = 1;
        else
            _evalsperhand = 2;
    }

    virtual size_t handSize() const  { return _heromax;  }
    virtual size_t boardSize() const { return _boardmax; }

    virtual size_t evaluationSize() const
    {
        if (_evalB == evalFunction(NULL))
            return 1;
        return 2;
    }

    virtual PokerHandEvaluation evaluateHand(const CardSet& hand, const CardSet& board) const
    {
        PokerEvaluation eval[2];

        // check to see if the input hand is consistent with the game
        if (hand.size() < _heromin ||
            hand.size() > _heromax)
            throw std::invalid_argument(std::string("UnivHandEval: "
                                                    + boost::lexical_cast<std::string>(uint(hand.size()))
                                                    + ": invalid number of pocket cards"));
        CardSet h = hand;

        std::vector<CardSet> hand_candidates;
        std::vector<CardSet> board_candidates;
        std::vector<CardSet> eval_candidates;

        // now check the board, it's a distribution
        size_t bz = board.size();
        if ((bz < _boardmin && bz > 0) ||
            bz > _boardmax)
            throw std::invalid_argument(std::string("UnivHandEval: "
                                                    + boost::lexical_cast<std::string>((uint)board.size())
                                                    + " unsupported number of board cards"));

        // generate the possible sub parts, the reference example is omaha
        // where a player must use two cards from their hand, and three
        // from the board.
        fillSubsets(hand_candidates, _herouse, h);
        fillSubsets(board_candidates, boardSize()-_herouse, board);

        // combine the subset candidates to create all possible sets of evaluations
        // at the river in omaha, this should produce (4c2)*(5c3) = 6*10 = 60 candidates
        for (uint i=0; i<hand_candidates.size(); i++)
            for (uint j=0; j<board_candidates.size(); j++)
            {
                eval_candidates.push_back(hand_candidates[i] | board_candidates[j]);
            }

        // evaluation of the first type.  we do a quick evaluation
        // of the one candidate which *must* be there, and then if
        // there are more candidates, we just run through them updating
        // as we find better ones.
        eval[0] = ((eval_candidates[0]).*(_evalA))();
        for (uint i=1; i<eval_candidates.size(); i++)
        {
            PokerEvaluation e = ((eval_candidates[i]).*(_evalA))();
            if (e > eval[0])
                eval[0] = e;
        }

        // if it's a one dimensional evaluation we are done
        if (_evalB != evalFunction(NULL))
        {
            // second dimension of the evaulation, usually low in a high/low game.
            eval[1] = PokerEvaluation();
            for (uint i=0; i<eval_candidates.size(); i++)
            {
                PokerEvaluation e = ((eval_candidates[i]).*(_evalB))();
                if (e > eval[1])
                {
                    eval[1] = e;
                }
            }
        }
        return PokerHandEvaluation(eval[0],eval[1]);
    }

    void fillSubsets(std::vector<CardSet> & candidates, size_t subsetsize, CardSet cards) const
    {
        if (subsetsize > cards.size())
        {
            candidates.push_back(CardSet());
            return;
        }
        if (subsetsize == 0)
        {
            candidates.push_back(cards);
            return;
        }

        std::vector<Card> clist = cards.cards();
        pokerstove::combinations cc(static_cast<uint>(clist.size()), static_cast<uint>(subsetsize));
        do
        {
            CardSet cand;
            for (size_t i=0; i<subsetsize; i++)
            {
                cand.insert(clist[cc[static_cast<uint>(i)]]);
            }
            candidates.push_back(cand);
        }
        while (cc.nextcomb());
    }

    virtual size_t evalsPerHand() const { return _evalsperhand; }

private:
    size_t _heromin;
    size_t _heromax;
    size_t _boardmin;
    size_t _boardmax;
    size_t _herouse;
    evalFunction _evalA;
    evalFunction _evalB;
    int _evalsperhand;
};

}

#endif  // PEVAL_UNIVERSALHANDEVALUATOR_H_
