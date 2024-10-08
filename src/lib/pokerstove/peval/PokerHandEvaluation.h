/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 * $Id: PokerHandEvaluation.h 2649 2012-06-30 04:53:24Z prock $
 */
#ifndef PEVAL_POKERHANDEVALUATION_H_
#define PEVAL_POKERHANDEVALUATION_H_

#include <stdexcept>
#include <iostream>
#include <vector>
#include "PokerEvaluation.h"

/**
 * The difference between a PokerEvaluation and a PokerHandEvaluation
 * is that a PokerHandEvaluation supports split pot games where (up to) two
 * evaluations are supported.
 */
namespace pokerstove
{
  /**
   * A class which allows multiple evaluations to be bundled.
   */
  class PokerHandEvaluation
  {
  public:
    PokerHandEvaluation ()
      : eval1()
      , eval2()
    {}

    explicit PokerHandEvaluation (const PokerEvaluation& e1)
      : eval1(e1)
      , eval2()
    {}

    PokerHandEvaluation (const PokerEvaluation& e1, const PokerEvaluation& e2)
      : eval1(e1)
      , eval2(e2)
    {}

    std::string str () const
    {
      if (highlow())
        return std::string (eval1.str() + "\n" + eval2.str());
      else
        return eval1.str();
    }

    PokerEvaluation high() const
    {
      return eval1;
    }

    PokerEvaluation low() const
    {
      return eval2;
    }

    bool highlow() const
    {
      return eval2 != PokerEvaluation();
    }

    int size() const
    {
      return highlow() ? 2 : 1;
    }

    std::vector<PokerEvaluation> evals() const
    {
      std::vector<PokerEvaluation> evals;
      evals.push_back(eval1);
      if (highlow())
        evals.push_back(eval2);
      return evals;
    }

    bool empty() const
    {
      return (eval1 == PokerEvaluation(0));
    }

    /**
     * return the evalution for the hand.  the first evaluation will
     * be returned by default.
     */
    PokerEvaluation eval(size_t n=0) const
    {
      if (n == 0)
        return eval1;
      else if (n == 1)
        return eval2;
      else
        throw std::runtime_error ("invalid evaluation requested");
    }

  private:
    PokerEvaluation eval1;
    PokerEvaluation eval2;
  };

  inline
  double shares(const PokerHandEvaluation& hero, const PokerHandEvaluation& villain)
  {
    if (!hero.highlow() && !villain.highlow())
      {
        if (hero.high() > villain.high())
          return 1.0;
        if (hero.high() < villain.high())
          return 0.0;
        return 0.5;
      }
    else  // high/low
      {
        // scoop/ed
        if (hero.high() > villain.high() && hero.low() > villain.low())
          return 1.0;
        if (hero.high() < villain.high() && hero.low() < villain.low())
          return 0.0;

        // split pots
        if ((hero.high() >  villain.high() && hero.low() <  villain.low()) ||
            (hero.high() <  villain.high() && hero.low() >  villain.low()) ||
            (hero.high() == villain.high() && hero.low() == villain.low()))
          return 0.5;

        // three quarters
        if ((hero.high() == villain.high() && hero.low() >  villain.low()) ||
            (hero.high() >  villain.high() && hero.low() == villain.low()))
          return 0.75;

        // quartered
        if ((hero.high() == villain.high() && hero.low() <  villain.low()) ||
            (hero.high() <  villain.high() && hero.low() == villain.low()))
          return 0.25;
      }
    return -1.0;
  }
}

#ifndef SWIG
inline std::ostream& operator<<(std::ostream& sout, const pokerstove::PokerHandEvaluation& e)
{
  sout << e.str();
  return sout;
}
#endif


#endif  // PEVAL_POKERHANDEVALUATION_H_
