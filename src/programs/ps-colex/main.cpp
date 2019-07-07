#include <boost/format.hpp>
#include <boost/program_options.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <pokerstove/peval/Card.h>
#include <pokerstove/peval/CardSet.h>
#include <pokerstove/util/combinations.h>
#include <string>
#include <vector>

using namespace std;
namespace po = boost::program_options;
using namespace pokerstove;

#if 0
std::set<CardSet>
expandRankSet(size_t numCards)
{
    combinations cards(52,numCards);
    do
    {
        CardSet hand;
        for (size_t i=0; i<num_cards; i++)
        {
            hand.insert (Card(cards[i]));
        }
        collection.insert(hand.canonize());
        rankHands[hand.rankstr()] = hand.rankColex();
    }
    while (cards.next());
}
#endif

int main(int argc, char** argv)
{
    try
    {
        // set up the program options, handle the help case, and extract the
        // values
        po::options_description desc(
            "pscolex, a utility which prints all combinations\n"
            "of poker hands, using canonical suits, or only ranks\n");

        desc.add_options()
            ("help,?",      "produce help message")
            ("num-cards,n", po::value<size_t>()->default_value(2), "number of cards in hands")
            ("ranks",       "print the set of rank values");

        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv)
                      .style(po::command_line_style::unix_style)
                      .options(desc)
                      .run(),
                  vm);
        po::notify(vm);

        // check for help
        if (vm.count("help") || argc == 1)
        {
            cout << desc << endl;
            return 1;
        }

        // extract the options
        size_t num_cards = vm["num-cards"].as<size_t>();

        set<CardSet> canonicalHands;
        map<string, size_t> rankHands;
        combinations cards(52, num_cards);
        do
        {
            CardSet hand;
            for (size_t i = 0; i < num_cards; i++)
            {
                hand.insert(Card(cards[i]));
            }
            canonicalHands.insert(hand.canonize());
            rankHands[hand.rankstr()] = hand.rankColex();
        } while (cards.next());

        if (vm.count("ranks") > 0)
        {
            for (auto it = rankHands.begin(); it != rankHands.end(); it++)
                cout << boost::format("%s: %d\n") % it->first % it->second;
        }
        else
        {
            for (auto it = canonicalHands.begin(); it != canonicalHands.end();
                 it++)
                cout << boost::format("%s: %d\n") % it->str() % it->colex();
        }
    }
    catch (std::exception& e)
    {
        cerr << "-- caught exception--\n" << e.what() << "\n";
        return 1;
    }
    catch (...)
    {
        cerr << "Exception of unknown type!\n";
        return 1;
    }
    return 0;
}
