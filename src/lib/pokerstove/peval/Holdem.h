/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 * $Id: Holdem.h 2649 2012-06-30 04:53:24Z prock $
 */
#ifndef PEVAL_HOLDEM_H_
#define PEVAL_HOLDEM_H_

namespace pokerstove
{

// Facts about holdem.
const unsigned int PREFLOP = 0;
const unsigned int FLOP = 1;
const unsigned int TURN = 2;
const unsigned int RIVER = 3;
const unsigned int BOARD_SIZE = 5;
const unsigned int NUM_FLOP_CARDS = 3;
const unsigned int NUM_TURN_CARDS = 4;
const unsigned int NUM_RIVER_CARDS = 5;
const unsigned int NUM_HOLDEM_POCKET = 2;
const unsigned int NUM_HOLDEM_ROUNDS = 4;
}  // namespace pokerstove

#endif  // PEVAL_HOLDEM_H_
