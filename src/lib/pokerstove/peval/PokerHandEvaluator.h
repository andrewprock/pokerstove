/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 * $Id: PokerHandEvaluator.h 2649 2012-06-30 04:53:24Z prock $
 */
#ifndef PEVAL_POKERHANDEVALUATOR_H_
#define PEVAL_POKERHANDEVALUATOR_H_

#include "CardSet.h"
#include "PokerHandEvaluation.h"
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>

const unsigned int BOARD_SIZE = 5;

namespace pokerstove
{
/**
 * What is actually stored in the equity result is up to the evalutor
 * being used.  Usually it is either wins/ties, or m1/m2
 */
struct EquityResult
{
    double winShares;
    double tieShares;
    double equity;
    double equity2;  // second moment of equity

    explicit EquityResult()
        : winShares(0.0)
        , tieShares(0.0)
        , equity(0.0)
        , equity2(0.0)
    {}

    EquityResult& operator+=(const EquityResult& other)
    {
        winShares += other.winShares;
        tieShares += other.tieShares;
        return *this;
    }

    std::string str() const
    {
        std::string ret = boost::lexical_cast<std::string>(winShares) + " " +
                          boost::lexical_cast<std::string>(tieShares) + " " +
                          boost::lexical_cast<std::string>(equity) + " " +
                          boost::lexical_cast<std::string>(equity2);
        return ret;
    }
};

/**
 * A base class for all simple hand evaluation classes.  All we are
 * trying to do here is to abstract the hand evaluation.  No
 * rollouts of any kind are done in here.
 */
class PokerHandEvaluator
{
public:
    virtual ~PokerHandEvaluator();

    /**
     * Construction of subclasses must be done through this Factory.
     * The subclass is identified by the first letter of the input
     * string according to the table below
     *
     * supported games:
     * - 'h'    hold'em (or high if no board)
     * - 'k'    Kansas City lowball (2-7)
     * - 'l'    lowball (A-5)
     * - '3'    three card poker
     * - 'O'    omaha high
     * - 'r'    razz
     * - 's'    stud
     * - 'q'    stud high/low no qualifier
     * - 'd'    draw high
     * - 't'    triple draw lowball (2-7)
     * - 'T'    triple draw lowball (A-5)
     * - 'o'    omaha/high low
     * - 'e'    stud/8
     * - 'b'    badugi
     */
    typedef boost::shared_ptr<PokerHandEvaluator> eval_ptr;
    static eval_ptr alloc(const std::string& strid);

    /**
     * The generic evaluation method.  returns the evaluation for this
     * hand.
     * @see PokerHandEvaluation
     */
    virtual PokerHandEvaluation evaluateHand(const CardSet& hand,
                                             const CardSet& board = CardSet(0)) const = 0;

    virtual PokerHandEvaluation evaluate(const CardSet& hand,
                                         const CardSet& board = CardSet(0))
    {
        return evaluateHand(hand, board);
    }

    /**
     * This version is for when you know you are only interested in
     * non-split pot games.
     */
    virtual PokerEvaluation eval(const CardSet& hand,
                                 const CardSet& board = CardSet(0))
    {
        return evaluateHand(hand, board).high();
    }

    virtual size_t handSize () const = 0;            //!< return the maximum size of a players hand
    virtual size_t boardSize () const = 0;           //!< return the maximum size of the community cards
    virtual size_t evaluationSize () const = 0;      //!< return 1 for high only, 2 for high low
    virtual size_t numDraws () const { return 0; }   //!< return the maximum size of a players hand

    virtual PokerEvaluation evaluateRanks (const CardSet & hand,
                                           const CardSet& board=CardSet(0)) const
    {
      return evaluateHand (hand, board).eval (0);
    }

    virtual PokerEvaluation evaluateSuits (const CardSet & hand,
                                           const CardSet& board=CardSet(0)) const
    {
      return evaluateHand (hand, board).eval (0);
    }

    virtual bool usesSuits() const { return _useSuits; }

    void useSuits(bool use) { _useSuits = use; }

    /**
     * used to add "draws" to draw games
     */
    virtual void setNumDraws(size_t sz)
    {
        throw std::runtime_error("not implemented");
    }

    /**
     * Given a set of showdown hands, return the corresponding number of
     * shares of the pot each hand is rewarded.  The shares are accumulated
     * in the result parameter.
     *
     * This method does not re-initialize the result vector, rather it
     * uses it to accumulate the equity results.
     *
     * This method is designed to minimize stack allocation, note that the
     * eval vector is temporary storage allocated externally.
     *
     * @hands the list of hands to be evaluated
     * @board the community cards to use, defaults to no board
     * @evals storage for the evaluations (hack for speed)
     * @result where to accumulate the shares
     *
     * A couple of subtle notes, the number of hands to be evaluated is
     * determined by the size of the evals vector, which *must* be the same
     * size as the result vector.  The hands vector is allowe to be larger
     * than that.  The board may or may not be used depending on how
     * evaluateHand is implemented.
     */
    void evaluateShowdown(const std::vector<CardSet>& hands,
                          const pokerstove::CardSet& board,
                          std::vector<PokerHandEvaluation>& evals,
                          std::vector<EquityResult>& result,
                          double weight = 1.0) const;

protected:
    PokerHandEvaluator();

private:
    // non-copyable
    PokerHandEvaluator(const PokerHandEvaluator&);
    PokerHandEvaluator& operator=(const PokerHandEvaluator&);

    // for printing
    std::string _subclassID;

    // we can turn on and off suit evaluation if we choose
    bool _useSuits;
};

// for planned refactoring
typedef PokerHandEvaluator GameEvaluator;

}  // namespace pokerstove

#endif  // PEVAL_POKERHANDEVALUATOR_H_
