#include "CardSet.h"
#include <gtest/gtest.h>

using namespace pokerstove;

TEST(CardSetTest, StringConstructorToString)
{
    EXPECT_EQ("Ac", CardSet("Ac").str());
    EXPECT_EQ("As", CardSet("As").str());
    EXPECT_EQ("Qh", CardSet("qh").str());
    EXPECT_EQ("Td", CardSet("Td").str());
    EXPECT_EQ("Td", CardSet("td").str());
    // EXPECT_THROW(CardSet("10d"));
    // EXPECT_THROW(CardSet("AcAc"));
}

TEST(CardSetTest, StringConstructorSize)
{
    EXPECT_EQ(1, CardSet("Ac").size());
    EXPECT_EQ(1, CardSet("qh").size());
    EXPECT_EQ(13, CardSet("2h3h4h5h6h7h8h9hThJhQhKhAh").size());

    // duplicate cards rolls back all parsing
    EXPECT_EQ(0, CardSet("2h2h").size());
}

TEST(CardSetTest, Canonize)
{
    EXPECT_EQ(CardSet("2c3c"), CardSet("2c3c").canonize());
    EXPECT_EQ(CardSet("2c3c"), CardSet("2s3s").canonize());
    EXPECT_EQ(CardSet("4c3d2h"), CardSet("2s3h4c").canonize());
}

TEST(CardSetTest, CanonizeRanks)
{
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

TEST(CardSetTest, fill)
{
    CardSet all;
    all.fill();
    EXPECT_EQ(STANDARD_DECK_SIZE, all.size());
}

TEST(CardSetTest, colex)
{
    // first two one-card colex indexes
    const CardSet zeroth("2c");
    const CardSet first("3c");
    EXPECT_EQ(zeroth.colex(), 0);
    EXPECT_EQ(first.colex(), 1);

    // first two two-card colex indexes
    const CardSet zeroth2("2c3c");
    const CardSet first2("2c4c");
    EXPECT_EQ(zeroth2.colex(), 0);
    EXPECT_EQ(first2.colex(), 1);

    // order doesn't matter, but suits do
    const CardSet kings1("KsKd");
    const CardSet kings2("KdKs");
    const CardSet kings3("KcKs");
    EXPECT_EQ(kings1.colex(), 1249);
    EXPECT_EQ(kings2.colex(), 1249);
    EXPECT_EQ(kings3.colex(), 1236);

    // other examples
    const CardSet aces("AcAh");
    const CardSet bigSlick("AdKh");
    const CardSet brunson("Ts2c");
    EXPECT_EQ(aces.colex(), 715);
    EXPECT_EQ(bigSlick.colex(), 691);
    EXPECT_EQ(brunson.colex(), 1081);
}

TEST(CardSetTest, fromColex)
{
    CardSet cards = CardSet::fromColex(7, 4, 22);
    CardSet target(0x01<<0 | 0x01<<3 | 0x01<<4 | 0x01<<6);
    EXPECT_EQ(cards, target);

    const CardSet aces("AcAh");
    const CardSet result = CardSet::fromColex(52, 2, aces.colex());
    EXPECT_EQ(result,  aces);

    const CardSet bigSlick("AdKh");
    EXPECT_EQ(CardSet::fromColex(52, 2, bigSlick.colex()),  bigSlick);

    const CardSet brunson("Ts2c");
    EXPECT_EQ(CardSet::fromColex(52, 2, brunson.colex()),  brunson);

    const CardSet fourCardZero("2c3c4c5c");
    const CardSet result2 = CardSet::fromColex(4, 0);
    EXPECT_EQ(result2,  fourCardZero);
}
