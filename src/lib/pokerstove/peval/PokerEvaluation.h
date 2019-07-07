/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 * $Id: PokerEvaluation.h 2649 2012-06-30 04:53:24Z prock $
 */
#ifndef PEVAL_POKEREVALUATION_H_
#define PEVAL_POKEREVALUATION_H_

#include <iosfwd>
#include <climits>
#include <boost/operators.hpp>
#include "Rank.h"

namespace pokerstove
{
  class PokerHandEvaluator;
  class PokerHand;

  // this should probably be a nested enum
  const int NO_PAIR               =  0;
  const int ONE_PAIR              =  1;
  const int THREE_FLUSH           =  2;
  const int THREE_STRAIGHT        =  3;
  const int TWO_PAIR              =  4;
  const int THREE_OF_A_KIND       =  5;
  const int THREE_STRAIGHT_FLUSH  =  6;
  const int STRAIGHT              =  7;
  const int FLUSH                 =  8;
  const int FULL_HOUSE            =  9;
  const int FOUR_OF_A_KIND        = 10;
  const int STRAIGHT_FLUSH        = 11;

  const int NUM_EVAL_TYPES        = 12;
  const int FULL_HAND_SIZE        =  5;
  const int MAX_EVAL_HAND_SIZE    =  7;

  const int VSHIFT = 24;
  const int MAJOR_SHIFT = 20;
  const int MINOR_SHIFT = 16;
  const int ACE_LOW_BIT = 0x01 << Rank::NUM_RANK;

  /**
   * An endocing of poker evaluations which preserves order.
   *
   */
  class PokerEvaluation
  {
  public:
    PokerEvaluation ();
    explicit PokerEvaluation (int ecode); //!< for codes saved for later use, like in a file
    PokerEvaluation (int type,            //!< Manually create high hand evaluation
                     int major,
                     int minor,
                     int kickers);

    std::string str () const;    //!< semantic meaning of the evaluation
    std::string bitstr () const; //!< bit string of the evaluation code. debugging.
    int code () const;           //!< the bit representation

    /**
     * This is a showdown code, useful for comparing to other hands instead
     * of finding out all information about the hand.  Kickers are stripped
     * from hands like four of a kind and trips, and the minor rank of full
     * houses stripped.  This method uses the assumption that there are
     * only four cards with a given rank.
     */
    int showdownCode () const;

    /**
     * Reduce the showdown code significantly.  For hands which use
     * multiple kickers, only the two most significant kickers are included.
     */
    int reducedCode () const;

    /**
     * Reduce the showdown code significantly.  For hands which use
     * multiple kickers, only the two most significant kickers are included.
     */
    int reducedCode2to7 () const;

    /**
     * return the hand type, NO_PAIR, STRAIGHT, etc...
     */
    int type () const;

    /**
     * return the primary rank associated with the evaluation.
     * examples:
     * - 333KK returns 3
     * - AA234 returns A
     *
     * note: if there is no major rank, Rank::Two is returned
     */
    Rank majorRank () const;

    /**
     * return the primary rank associated with the evaluation.
     * examples:
     * - 333KK returns K
     * - AA234 returns
     *
     * note: if there is no minor rank, Rank::Two is returned
     */
    Rank minorRank () const;

    /**
     * Change wheels to 2345A high in deuce to seven lowball
     */
    void fixWheel2to7 (int rankMask);

    /**
     * return a bitmask of the kickers for the hand
     */
    int  kickerBits () const;

    // boost/operators.hpp creates closure
    bool operator==(const PokerEvaluation& e) const { return _evalcode == e._evalcode; }
    bool operator!=(const PokerEvaluation& e) const { return _evalcode != e._evalcode; }
    bool operator<=(const PokerEvaluation& e) const { return _evalcode <= e._evalcode; }
    bool operator< (const PokerEvaluation& e) const { return _evalcode <  e._evalcode; }
    bool operator> (const PokerEvaluation& e) const { return _evalcode >  e._evalcode; }
    void operator++()                    { ++_evalcode; }
    void operator--()                    { --_evalcode; }

    std::string toStringCannon () const;

  private:
    // table generation code, these should not be in this class.  make them private for now
    // TODO: find a better place for these
    static void generateLowballLookupA5 ();
    static void generateBottomRankMask ();

    // some of these methods might be useful to expose, but generally the PokerEvaluation is meant to
    // be just for comparing hands, not classifying them
    void setKickerBits (int k);
    void setMajorRank (int r);
    void setMinorRank (int r);
    void playAceLow ();
    void playAceHigh ();
    bool acePlaysLow () const;

    // the "low" evaluations get flipped so that best hand ordering is maintained
    void flip ()            { _evalcode = INT_MAX - _evalcode; }
    bool isFlipped () const { return (_evalcode > INT_MAX>>1); }        // should use <numerics>

    // this function needs friend access to rank
    std::string makeRankString (int r, bool acelow) const;

    std::string strKickers (int n) const;
    std::string strTop (int n) const;
    std::string strBot (int n) const;

    friend class CardSet;
    friend class PokerHand;

    /**
     * the integer code is organized bitwise as:
     *  21 0987654 32109876 54321098 76543210
     *        TTTT MMMMmmmm BBAkkkkk kkkkkkkk
     *
     * T = type bits [0,12] (pair, trips, flush, etc...)
     * M = major rank
     * m = minor rank
     * A = ace swings low bit 0/1
     * k = kicker set
     * . = unused bit
     * B = badugi bits = N-1 cards in the badugi
     *
     * If the A bit is set to 1, then the
     * bit mapping of the ranks is shifted
     * from: AKQJT98765432
     * to:   KQJT98765432A
     *
     * likewise the integer mapping for the
     * major/minor rank codes is shifted as
     * well.
     *
     * In the case that a hand represents a "lowball"
     * hand the entire bitstring will be flipped so
     * the the ordering is reversed.  That will
     * make the comparison 75432 > K5432 true.
     */
    int _evalcode;
  };

}  // namespace pokerstove

//
// Extraction operators.
//
std::ostream& operator<<(std::ostream& sout, const pokerstove::PokerEvaluation& e);

#endif  // PEVAL_POKEREVALUATION_H_
