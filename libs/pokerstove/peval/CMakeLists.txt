cmake_minimum_required(VERSION 2.6)
find_package (Threads)

# peval library

set(sources
        Card.cpp
        CardSet.cpp
        CardSetGenerators.cpp
        PokerEvaluation.cpp
        PokerHand.cpp
        PokerHandEvaluator.cpp
        PokerHandEvaluator_Alloc.cpp
        Rank.cpp
        Suit.cpp
)
add_library(peval ${sources})

set(test_sources
        CardSet.test.cpp
        CardSetGenerators.test.cpp
)
add_executable(peval_tests ${test_sources})

target_link_libraries(peval_tests peval gtest gtest_main)

add_test(TestPeval peval_tests )
