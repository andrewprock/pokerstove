/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 */
#ifndef PENUM_SHOWDOWNENUMERATOR_H_
#define PENUM_SHOWDOWNENUMERATOR_H_

#include "CardDistribution.h"
#include <boost/shared_ptr.hpp>
#include <pokerstove/peval/PokerHandEvaluator.h>
#include <vector>

namespace pokerstove
{
class ShowdownEnumerator
{
public:
    ShowdownEnumerator();

    /**
     * enumerate a poker scenario, with board support
     */
    std::vector<EquityResult>
    calculateEquity(const std::vector<CardDistribution>& dists,
                    const CardSet& board,
                    boost::shared_ptr<PokerHandEvaluator> peval) const;
};
}  // namespace pokerstove

#endif  // PENUM_SHOWDOWNENUMERATOR_H_
