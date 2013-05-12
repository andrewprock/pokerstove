#include <iostream>
#include <vector>
#include <string>
#include <boost/program_options.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/format.hpp>
#include <pokerstove/peval/PokerHandEvaluator.h>

using namespace std;
namespace po = boost::program_options;
using namespace pokerstove;

class EvalDriver
{
public:
    EvalDriver(const string& game, 
               const vector<string>& hands, 
               const string& board)
        : _peval(PokerHandEvaluator::alloc (game))
        , _hands(hands)
        , _board(board)
    {
    }

    void evaluate()
    {
        for (string& hand: _hands)
        {
            _results[hand] = _peval->evaluate(hand, _board);
        }
    }

    string str() const
    {
        string ret;
        for (const string& hand: _hands)
        {
            ret += (boost::format("%10s: %s\n") % hand % _results.at(hand).str()).str();
        }
        return ret;
    }

private:
    boost::shared_ptr<PokerHandEvaluator> _peval;
    vector<string> _hands;
    string _board;
    map<string,PokerHandEvaluation> _results;
};

int main (int argc, char ** argv)
{
    string extendedHelp = "\n"
        "   For the --game option, one of the follwing games may be specified.\n"
        "     h     hold'em\n"
        "     o     omaha/8\n"
        "     O     omaha high\n"
        "     r     razz\n"
        "     s     stud\n"
        "     e     stud/8\n"
        "     q     stud high/low no qualifier\n"
        "     d     draw high\n"
        "     l     lowball (A-5)\n"
        "     k     Kansas City lowball (2-7)\n"
        "     t     triple draw lowball (2-7)\n"
        "     T     triple draw lowball (A-5)\n"
        "     b     badugi\n"
        "     3     three-card poker\n"
        "\n"
        "   examples:\n"
        "       pseval acas\n"
        "       pseval AcAs Kh4d --board 5c8s9h\n"
        "       pseval AcAs Kh4d --board 5c8s9h\n"
        "       pseval --game l 7c5c4c3c2c\n"
        "       pseval --game k 7c5c4c3c2c\n"
        "       pseval --game kansas-city-lowball 7c5c4c3c2c\n"
        "\n"
        ;

    try 
    {
        // set up the program options, handle the help case, and extract the values
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help,?",	  "produce help message")
            ("game,g",    po::value<string>()->default_value("h"), "game to use for evaluation")
            ("board,b",   po::value<string>()->default_value(""),  "community cards for he/o/o8")
            ("hand,h",    po::value< vector<string> >(),           "a hand for evaluation")
			("quiet,q",	  "produce no output")
            ;
      
        po::positional_options_description p;
        p.add("hand", -1);
        po::variables_map vm;
        po::store (po::command_line_parser(argc, argv)
                   .style(po::command_line_style::unix_style)
                   .options(desc)
                   .positional(p)
                   .run(), vm);
        po::notify (vm);

        // check for help
        if (vm.count("help") || argc == 1)
        {
            cout << desc << extendedHelp << endl;
            return 1;
        }

        // extract the options
        EvalDriver driver(vm["game"].as<string>(),
                          vm["hand"].as< vector<string> >(),
                          vm["board"].as<string>());
		driver.evaluate();
		if (vm.count("quiet") == 0)
			cout << driver.str();
    }
    catch(std::exception& e) 
    {
        cerr << "-- caught exception--\n" << e.what() << "\n";
        return 1;
    }
    catch(...) 
    {
        cerr << "Exception of unknown type!\n";
        return 1;
    }
    return 0;
}
