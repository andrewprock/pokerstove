/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 */
#include "ShowdownEnumerator.h"

#include <string>
#include <vector>

#include "Odometer.h"
#include "PartitionEnumerator.h"
#include "SimpleDeck.hpp"

using std::runtime_error;
using std::string;
using std::vector;

namespace pokerstove
{

ShowdownEnumerator::ShowdownEnumerator() {}

vector<EquityResult> ShowdownEnumerator::calculateEquity(const vector<CardDistribution>& dists,
                                                         const CardSet& board,
                                                         boost::shared_ptr<PokerHandEvaluator> peval) const
{
    if (peval.get() == NULL)
        throw runtime_error("ShowdownEnumerator, null evaluator");
    assert(dists.size() > 1);
    const size_t ndists = dists.size();
    vector<EquityResult> results(ndists, EquityResult());
    size_t handsize = peval->handSize();

    // the dsizes vector is a list of the sizes of the player hand
    // distributions
    vector<size_t> dsizes;
    for (size_t i = 0; i < ndists; i++)
    {
        assert(dists[i].size() > 0);
        dsizes.push_back(dists[i].size());
    }

    // need to figure out the board stuff, we'll be rolling the board into
    // the partitions to make enumeration easier down the line.
    size_t nboards = 0;
    size_t boardsize = peval->boardSize();
    if (boardsize > 0)
        nboards++;

    // for the most part, these are allocated here to avoid contant stack
    // reallocation as we cycle through the inner loops
    SimpleDeck deck;
    CardSet dead;
    double weight;
    vector<CardSet>             ehands         (ndists + nboards);
    vector<size_t>              parts          (ndists + nboards);
    vector<CardSet>             cardPartitions (ndists + nboards);
    vector<PokerHandEvaluation> evals          (ndists);  // NO BOARD

    // copy quickness
    CardSet* copydest = &ehands[0];
    CardSet* copysrc = &cardPartitions[0];
    size_t ncopy = (ndists + nboards) * sizeof(CardSet);
    Odometer o(dsizes);
    do
    {
        // colect all the cards being used by the players, skip out in the
        // case of card duplication
        bool disjoint = true;
        dead.clear();
        weight = 1.0;
        for (size_t i = 0; i < ndists + nboards; i++)
        {
            if (i < ndists)
            {
                cardPartitions[i] = dists[i][o[i]];
                parts[i]          = handsize - cardPartitions[i].size();
                weight           *= dists[i][cardPartitions[i]];
            }
            else
            {
                // this allows us to have board distributions in the future
                cardPartitions[i] = board;
                parts[i]          = boardsize - cardPartitions[i].size();
            }
            disjoint = disjoint && dead.disjoint(cardPartitions[i]);
            dead |= cardPartitions[i];
        }

        if (disjoint)
        {
            deck.reset();
            deck.remove(dead);
            PartitionEnumerator2 pe(deck.size(), parts);
            do
            {
                // we use memcpy here for a little speed bonus
                memcpy(copydest, copysrc, ncopy);
                for (size_t p = 0; p < ndists + nboards; p++)
                    ehands[p] |= deck.peek(pe.getMask(p));

                // TODO: do we need this if/else, or can we just use the if
                // clause? A: need to rework tracking of whether a board is
                // needed
                if (nboards > 0)
                    peval->evaluateShowdown(ehands, ehands[ndists], evals, results, weight);
                else
                    peval->evaluateShowdown(ehands, board, evals, results, weight);
            } while (pe.next());
        }
    } while (o.next());

    return results;
}

}  // namespace pokerstove
