#include "PartitionEnumerator.h"
#include <gtest/gtest.h>
#include <iostream>

using namespace pokerstove;
using namespace std;

TEST(PartitionEnumerator, tautology) { EXPECT_EQ(1, 1); }

TEST(PartitionEnumerator, chinese_settings)
{
    // compute the number of ways to set a chinese poker hand
    std::vector<size_t> partitions(3);
    partitions[0] = 3;
    partitions[1] = 5;
    partitions[2] = 5;

    int visits = 0;
    PartitionEnumerator2 walker(13, partitions);
    do
    {
        visits += 1;
    } while (walker.next());
    EXPECT_EQ(72072, visits);
}

TEST(PartitionEnumerator, DISABLED_slow_all_ofcp_draws)
{
    // ofcp = open face chinese poker
    // compute the number of possible draw sets in open face chinese poker
    // This amounts to enumerating over all 47c8 sets.  This is set to
    // disabled because it takes a long time to run.
    std::vector<size_t> partitions;
    partitions.push_back(8);

    uint64_t visits = 0;
    PartitionEnumerator2 walker(47, partitions);
    do
    {
        if (visits % 10000000 == 0)
            cout << visits << endl;
        visits += 1;
    } while (walker.next());
    EXPECT_EQ(314457495, visits);  // 314,457,495
}

TEST(PartitionEnumerator, some_ofcp_draws)
{
    // ofcp = open face chinese poker
    // Iterate over all possible settings with 36 cards left, player has
    // three hands with open cards, 1, 2, 2 slots respectively.
    std::vector<size_t> partitions;
    partitions.push_back(1);  // one card left
    partitions.push_back(2);
    partitions.push_back(2);

    uint64_t visits = 0;
    PartitionEnumerator2 walker(36, partitions);
    do
    {
        visits += 1;
    } while (walker.next());
    EXPECT_EQ(11309760, visits);  // 11,309,760
}

TEST(PartitionEnumerator, end_game_ofcp_draws_A)
{
    // ofcp = open face chinese poker
    // Iterate over all end game scenarios.
    // Player A: one hand with two open cards.
    // Player B: two hands with one open card each.
    // At this point in the game there are (13-2)*2 cards used, or 30 cards
    // remaining in the deck.
    std::vector<size_t> partitions;
    partitions.push_back(2);
    partitions.push_back(1);
    partitions.push_back(1);

    uint64_t visits = 0;
    PartitionEnumerator2 walker(30, partitions);
    do
    {
        visits += 1;
    } while (walker.next());
    EXPECT_EQ(328860, visits);  // 328,860
}
