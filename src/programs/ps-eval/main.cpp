#include <iostream>
#include <vector>
#include <pokerstove/penum/ShowdownEnumerator.h>

int main() {

using namespace pokerstove;
using namespace std;

CardSet completeDeck;
completeDeck.fill();
cout << "The whole deck has " << completeDeck.size() << " cards" << endl;

CardDistribution anyTwo;
anyTwo.fill(completeDeck, 2);
cout << "There are " << anyTwo.size() << " two card combinations"  << endl;

CardDistribution holeCards;
holeCards.parse("As6s");
cout << "There are " << holeCards.size() << " two card combinations"  << endl;

ShowdownEnumerator showdown;
vector<EquityResult> result = showdown.calculateEquity(
    vector<CardDistribution>{anyTwo, holeCards},
    CardSet(""),
    PokerHandEvaluator::alloc("h")
);

double shareRandom = result.at(0).winShares + result.at(0).tieShares;
double shareHand   = result.at(1).winShares + result.at(1).tieShares;
double total       = shareRandom + shareHand;

cout << "A random hand has "  << shareRandom / total * 100  << " % equity (" << result.at(0).str() << ")" << endl;
cout << "The hand As6s has "  << shareHand   / total * 100  << " % equity (" << result.at(1).str() << ")" << endl;

}
