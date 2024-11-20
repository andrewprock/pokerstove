#include "combinations.h"
#include <gtest/gtest.h>

using namespace pokerstove;

TEST(Combinations, holdem)
{
    // if we run though all the 2 card combos we get 52c2 hands
    int count = 0;
    combinations cards(52, 2);
    do {
        count += 1;
    } while (cards.next());

    EXPECT_EQ(count, 1326);
}

TEST(Combinations, time_boost)
{
    for (int i=0; i<6000; i++) {
        for (int k=0; k<10; k++) {
            for (int n=k; n<10; n++) {
                (void)static_cast<size_t>(boost::math::binomial_coefficient<double>(n, k));
            }
        }
    }
}

TEST(Combinations, time_clamped_boost)
{
    for (int i=0; i<6000; i++) {
        for (int k=0; k<10; k++) {
            for (int n=k; n<10; n++) {
                choose(n, k);
            }
        }
    }
}