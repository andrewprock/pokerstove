#include <boost/format.hpp>
#include <boost/program_options.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <pokerstove/peval/Card.h>
#include <pokerstove/peval/CardSet.h>
#include <pokerstove/peval/CardSetGenerators.h>
#include <pokerstove/peval/PokerHandEvaluator.h>
#include <pokerstove/util/combinations.h>
#include <string>
#include <vector>

using namespace std;
namespace po = boost::program_options;
using namespace pokerstove;

int main(int argc, char** argv)
{
    try
    {
        // set up the program options, handle the help case, and extract the
        // values
        po::options_description desc("create a lookup table for poker hands\n");
        desc.add_options()
            ("help,?", "produce help message")
            ("pocket-count,p",  po::value<size_t>()->default_value(2),  "number of pocket cards to use")
            ("board-count,b",   po::value<size_t>()->default_value(3),  "number of board cards to use")
            ("game,g",          po::value<string>()->default_value("O"), "game to use for evaluation")
            ("ranks",           "print the set of rank values");

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
        size_t pocketCount = vm["pocket-count"].as<size_t>();
        size_t boardCount = vm["board-count"].as<size_t>();
        string game = vm["game"].as<string>();

        bool ranks = vm.count("ranks") > 0;
        // make the sets
        Card::Grouping grouping = Card::SUIT_CANONICAL;
        if (ranks)
            grouping = Card::RANK;
        set<CardSet> pockets = createCardSet(pocketCount, grouping);
        set<CardSet> boards = createCardSet(boardCount, grouping);

        auto evaluator = PokerHandEvaluator::alloc(game);
        for (auto pit = pockets.begin(); pit != pockets.end(); pit++)
            for (auto bit = boards.begin(); bit != boards.end(); bit++)
            {
                if (ranks)
                {
                    PokerEvaluation eval = evaluator->evaluateRanks(*pit, *bit);
                    // cout << boost::format("%s, %s\n") % pit->rankstr() %
                    // bit->rankstr();

                    cout << boost::format("%s, %s: [%4d,%4d] -> %s [%9d]\n") %
                                pit->str() %
                                bit->str() %
                                pit->rankColex() %
                                bit->rankColex() %
                                eval.str() %
                                eval.code();
                }
                else
                {
                    bit->canonize(*pit);
                    if (pit->intersects(*bit))
                        continue;
                    PokerHandEvaluation eval = evaluator->evaluate(*pit, *bit);
                    cout << boost::format("%s, %s: [%4d,%4d] -> %s [%9d]\n") %
                                pit->str() %
                                bit->str() %
                                pit->colex() %
                                bit->colex() %
                                eval.str() %
                                eval.high().code();
                }
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
