/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 * $Id: PokerHandEvaluator_Alloc.cpp 2649 2012-06-30 04:53:24Z prock $
 */
#include <stdexcept>
#include "HoldemHandEvaluator.h"
#include "StudHandEvaluator.h"
#include "RazzHandEvaluator.h"
#include "StudEightHandEvaluator.h"
#include "OmahaHighHandEvaluator.h"
#include "OmahaEightHandEvaluator.h"
#include "DeuceToSevenHandEvaluator.h"
#include "DrawHighHandEvaluator.h"
#include "BadugiHandEvaluator.h"
//#include "LowballA5HandEvaluator.h"
//#include "ThreeCardPokerHandEvaluator.h"

#include "UniversalHandEvaluator.h"

using namespace std;
using namespace boost;
using namespace pokerstove;

boost::shared_ptr<PokerHandEvaluator> PokerHandEvaluator::alloc (const string & strid)
{
  boost::shared_ptr<PokerHandEvaluator> ret;
  switch (strid[0])
    {
    case 'h':		//     hold'em
    case 'H':		//     hold'em
      //ret = new UniversalHandEvaluator (2,2,3,5,0,&CardSet::evaluateHigh, NULL);
      ret.reset (new HoldemHandEvaluator);
      break;

    case 'k':		//     Kansas City lowball (2-7)
      ret.reset (new UniversalHandEvaluator (1,5,0,0,0,&CardSet::evaluateLow2to7, NULL));
      break;

    case 'l':		//     lowball (A-5)
      ret.reset (new UniversalHandEvaluator (1,5,0,0,0,&CardSet::evaluateLowA5, NULL));
      break;

    case '3':		//     three card poker
      ret.reset (new UniversalHandEvaluator (1,3,0,0,0,&CardSet::evaluate3CP, NULL));
      break;

    case 'O':		//     omaha high
      //ret.reset (new UniversalHandEvaluator (4,4,3,5,2,&CardSet::evaluateHigh, NULL));
      ret.reset (new OmahaHighHandEvaluator);
      break;

    case 'p':		//     pot limit
    case 'P':
      if (strid[2] == 'h' || strid[2] == 'H')           // plh/PLH
        ret.reset (new HoldemHandEvaluator);
      else if (strid[2] == 'o' || strid[2] == 'O')      // PLO
        ret.reset (new OmahaHighHandEvaluator);
      else
        throw std::runtime_error ("no comatible pot limit game available");
      break;

      //
      // For the draw games, we need to be able to evaluate 1 card to determine the
      // bring in on 3rd street

    case 'r':		//     razz
      //ret.reset (new UniversalHandEvaluator (1,7,0,0,0,&CardSet::evaluateLowA5, NULL));
      ret.reset (new RazzHandEvaluator);
      break;

    case 's':		//     stud
      //ret.reset (new UniversalHandEvaluator (1,7,0,0,0,&CardSet::evaluateHigh, NULL));
      ret.reset (new StudHandEvaluator);
      break;

    case 'q':		//     stud high/low no qualifier
      ret.reset (new UniversalHandEvaluator (1,7,0,0,0,
                                             &CardSet::evaluateHigh, &CardSet::evaluateLowA5));
      break;

    case 'd':		//     draw high
    case 'D':		//     draw high
      //ret.reset (new UniversalHandEvaluator (1,5,0,0,0,&CardSet::evaluateHigh, NULL));
      ret.reset (new DrawHighHandEvaluator);
      break;

    case 't':		//     triple draw lowball (2-7)
      //ret.reset (new UniversalHandEvaluator (1,5,0,0,0,&CardSet::evaluateLow2to7, NULL));
      ret.reset (new DeuceToSevenHandEvaluator);
      break;

    case 'T':		//     triple draw lowball (A-5)
      ret.reset (new UniversalHandEvaluator (1,5,0,0,0,&CardSet::evaluateLowA5, NULL));
      break;

    case 'o':		//     omaha/high low
      //ret.reset (new UniversalHandEvaluator (4,4,3,5,2, &CardSet::evaluateHigh, &CardSet::evaluate8LowA5));
      ret.reset (new OmahaEightHandEvaluator);
      break;

    case 'e':		//     stud/8
      //ret.reset (new UniversalHandEvaluator (1,7,0,0,0,&CardSet::evaluateHigh, &CardSet::evaluate8LowA5));
      ret.reset (new StudEightHandEvaluator);
      break;

    case 'b':		//     badugi
      //ret.reset (new UniversalHandEvaluator (0,52,0,0,0,&CardSet::evaluateBadugi,NULL));
      ret.reset (new BadugiHandEvaluator);
      break;

    }

  ret->_subclassID = strid;

  return ret;
}
