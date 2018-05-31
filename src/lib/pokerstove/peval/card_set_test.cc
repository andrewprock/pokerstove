#include "card_set.h"

#include <gtest/gtest.h>

TEST(CardSetTest, StringConstructorToString) {
  using namespace pokerstove;

  EXPECT_EQ("Ac", CardSet("Ac").str());
  EXPECT_EQ("As", CardSet("As").str());
  EXPECT_EQ("Qh", CardSet("qh").str());
  EXPECT_EQ("Td", CardSet("Td").str());
  EXPECT_EQ("Td", CardSet("td").str());
  // EXPECT_THROW(CardSet("10d"));
  // EXPECT_THROW(CardSet("AcAc"));
}

TEST(CardSetTest, StringConstructorSize) {
  using namespace pokerstove;

  EXPECT_EQ(1, CardSet("Ac").size());
  EXPECT_EQ(1, CardSet("qh").size());
  EXPECT_EQ(13, CardSet("2h3h4h5h6h7h8h9hThJhQhKhAh").size());

  // duplicate cards rolls back all parsing
  EXPECT_EQ(0, CardSet("2h2h").size());
}

TEST(CardSetTest, Canonize) {
  using namespace pokerstove;

  EXPECT_EQ(CardSet("2c3c"), CardSet("2c3c").canonize());
  EXPECT_EQ(CardSet("2c3c"), CardSet("2s3s").canonize());
  EXPECT_EQ(CardSet("4c3d2h"), CardSet("2s3h4c").canonize());
}

TEST(CardSetTest, CanonizeRanks) {
  using namespace pokerstove;

  const CardSet AceCanon1("Ac");
  const CardSet AceCanon2("AcAd");
  const CardSet AceCanon3("AcAdAh");
  const CardSet AceCanon4("AcAdAhAs");

  // all combinations of one
  EXPECT_EQ(AceCanon1, CardSet("Ac").canonizeRanks());
  EXPECT_EQ(AceCanon1, CardSet("Ad").canonizeRanks());
  EXPECT_EQ(AceCanon1, CardSet("Ah").canonizeRanks());
  EXPECT_EQ(AceCanon1, CardSet("As").canonizeRanks());

  // all combinations of two
  EXPECT_EQ(AceCanon2, CardSet("AcAd").canonizeRanks());
  EXPECT_EQ(AceCanon2, CardSet("AcAh").canonizeRanks());
  EXPECT_EQ(AceCanon2, CardSet("AcAs").canonizeRanks());
  EXPECT_EQ(AceCanon2, CardSet("AdAh").canonizeRanks());
  EXPECT_EQ(AceCanon2, CardSet("AdAs").canonizeRanks());
  EXPECT_EQ(AceCanon2, CardSet("AhAs").canonizeRanks());

  // all combinations of three
  EXPECT_EQ(AceCanon3, CardSet("AcAdAh").canonizeRanks());
  EXPECT_EQ(AceCanon3, CardSet("AcAdAs").canonizeRanks());
  EXPECT_EQ(AceCanon3, CardSet("AcAhAs").canonizeRanks());
  EXPECT_EQ(AceCanon3, CardSet("AdAhAs").canonizeRanks());

  // the one combination of four
  EXPECT_EQ(AceCanon4, CardSet("AcAdAhAs").canonizeRanks());

  // some multi card exampes
  EXPECT_EQ(CardSet("Ac2c3c4c5c"), CardSet("Ac2d3h4d5s").canonizeRanks());
  EXPECT_EQ(CardSet("AcAd2c3c4c5c"), CardSet("Ac2d3h4d5sAh").canonizeRanks());
  EXPECT_EQ(CardSet("AcAd2c3c4c5c"), CardSet("Ac2d3h4d5sAh").canonizeRanks());
}

TEST(CardSetTest, fill) {
  using namespace pokerstove;
  CardSet all;
  all.fill();
  EXPECT_EQ(STANDARD_DECK_SIZE, all.size());
}
