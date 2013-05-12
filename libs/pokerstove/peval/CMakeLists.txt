# peval library

set(sources
        Card.cpp
        CardSet.cpp
        PokerEvaluation.cpp
        CardDeck.cpp
        PokerHand.cpp
        PokerHandEvaluator.cpp
        PokerHandEvaluator_Alloc.cpp
        Rank.cpp
        Suit.cpp
)

add_library(peval ${sources})
