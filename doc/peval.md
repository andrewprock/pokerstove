# `pokerstove::peval`

The peval library is a simple flexible library for evaluating poker hands.
There are two main components to the library, the CardSet class and the
PokerHandEvaluator class.

## CardSet

The CardSet class is available by including `<pokerstove/peval/CardSet.h>`.

The CardSet class represents a bitset of all cards encoded as a bitset, as well
as various utility functions for manipulating and querying the contents of the
bitset.

Each card is represented as a bit in a 64-bit integer.  Because a standard deck
has 52 cards, this means that there are another 12 bits that are available for
other processing.  In the future some of these bits will certainly be used for
encoding other special cards, like the joker and the bug.

Because the representation is openly advertised as a bitset, the bit
manipulation operators are overloaded so that two CardSet objects can be
operated on directly at the bit level.

Creating a CardSet object is meant to be straightforward, using the various low
level object representations of Card, Rank, and Suit objects.  Alternatively,
string representations can be used to create them.

Here is a sample program that constructs two hands and prints them to the console.

    #include <iostream>
    #include <pokerstove/peval/CardSet.h>

    using pokerstove::CardSet;
    using pokerstove::Card;
    using pokerstove::Rank;
    using pokerstove::Suit;

    int main(void)
    {
        CardSet big_slick(Card(Rank('A'),Suit('s')),
                          Card(Rank('K'),Suit('s')));
        std::cout << big_slick.str() << std::endl;

        CardSet dead_mans_hand("AcAs8d8hJs");
        std::cout << dead_mans_hand.str() << std::endl;
    }

The expected output is:

    AsKs
    AcAs8d8hJs

### Low level CardSet evaluation

Currently the low level evaluation code is bound directly to the CardSet class.
There is a remarkable number of evaluators that exist, yet none of them
encompass any specific game structure.  Direct evaluators exist for:

* high poker
* lowball poker (A-5)
* lowball with an 8 qualifier (A-5)
* lowball poker (2-7)
* badugi
* rank evaluations
* suit evaluations

Plus a couple of others.  These low level evaluators exist for the purposes of
building evaluations for more complex games, as well as producing partial
evaluations during optimization.

### Colex evaluators

There are two colex evaluators which are designed to map card sets (and rank
sets) to a unique index.  These again are used for managing complex evaluatins,
and creating static and dynamic evaluation lookup tables.

### Printing utilities

There is a small handful of printing utilities, which print out the hand as a
string.  The rankstr() method prints the hand without suits, whereas
toRankBitString() prints out thirteen 0/1 values representing whether or not the
given rank exists in the hand.

Of special note is the Suit::setSuitStringType(Suit::display s) method which
allows suits to be printed in a variety of formats, including html, ansi,
extended ascii (ibm) with four color options.

## Game evaluation

Specific games use the low level evaluators to contruct evaluations according to
the rules of the given game.  For example, the HoldemHandEvaluator melds a
players pocket cards with a community board.  Likewise, the
StudEightHandEvaluator uses both evaluateHigh() and evaluate8LowA5() as it is a
split pot game.

The UniversalHandEvaluator is a utility class that that supports parameterized
game evaluation.  Most of the classic games can be constructed with this
evaluator, though optimized versions are created to speed up specific games.

The factory PokerHandEvaluator::alloc() can be used to construct the appropriate
game evaluator based on the input string.
