#include "suit.h"

#include <gtest/gtest.h>

using namespace pokerstove;

const char* suits = "cdhsCDHS";

TEST(Suit, ConstructorChars) {
  char suitstr[] = "?";

  // test the valid char range
  for (int i = 0; i < strlen(suits); i++) {
    Suit s(suits[i]);
    suitstr[0] = suits[i];
    EXPECT_STRCASEEQ(suitstr, s.str().c_str());
  }
}

TEST(Suit, ConstructorInts) {
  char suitstr[] = "?";

  // test the valid int range
  for (int i = 0; i <= 3; i++) {
    Suit s(i);
    suitstr[0] = suits[i];
    EXPECT_STREQ(suitstr, s.str().c_str());
  }
}

TEST(Suit, ConstructorErrs) {
  char suitstr[] = "?";

  // test the invalid int range
  for (int i = 4; i < 'C'; i++) {
    Suit s(i);
    EXPECT_STREQ(suitstr, s.str().c_str());
  }
}
