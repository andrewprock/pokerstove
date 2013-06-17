#include <gtest/gtest.h>

class CardSetTest : public ::testing::Test
{
protected:
    int x;

};

TEST_F(CardSetTest, Null) {
    ASSERT_TRUE(false);
}

#if 0
TEST_F(TestStringConstructorToString)
{
    EXPECT_EQ("Ac", CardSet("Ac").str());
    EXPECT_EQ("Ac", CardSet("Ac").str());
    EXPECT_THROW(CardSet("AcAc"));
}

TEST_F(TestCanonizeRanks)
{
    CardSet AceCanon1("Ac")
    CardSet AceCanon2("AcAd")
    CardSet AceCanon3("AcAdAh")
    CardSet AceCanon4("AcAdAhAs")

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
    EXPECT_EQ(CardSet("Ac2c3c4c5c"), CardSet("Ac2d3h4d5sAh").canonizeRanks());
    EXPECT_EQ(CardSet("AcAd2c3c4c5c"), CardSet("Ac2d3h4d5sAh").canonizeRanks());
}
#endif
