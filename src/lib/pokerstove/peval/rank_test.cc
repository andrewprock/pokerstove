#include "rank.h"

#include <gtest/gtest.h>

using namespace pokerstove;

const char* ranks = "23456789TJQKA";
const char* low_ranks = "23456789tjqka";

TEST(Rank, ConstructorChars) {
  char rankstr[] = "?";

  // test the valid char range
  for (int i = 0; i < strlen(ranks); i++) {
    Rank r(ranks[i]);
    rankstr[0] = ranks[i];
    EXPECT_STREQ(rankstr, r.str().c_str());
  }
}

TEST(Rank, ConstructorInts) {
  char rankstr[] = "?";

  // test the valid int range
  for (int i = 0; i <= 12; i++) {
    Rank r(i);
    rankstr[0] = ranks[i];
    EXPECT_STREQ(rankstr, r.str().c_str());
  }
}

TEST(Rank, ConstructorErrs) {
  char rankstr[] = "?";

  // test the valid int range
  for (int i = 13; i < '2'; i++) {
    Rank r(i);
    EXPECT_STREQ(rankstr, r.str().c_str());
  }
}
