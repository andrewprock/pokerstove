#include <pokerstove/penum/ShowdownEnumerator.h>
#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <iostream>
#include <vector>

using namespace pokerstove;
namespace po = boost::program_options;
namespace pt = boost::property_tree;
using namespace std;

int main(int argc, char** argv) {
  po::options_description desc("ps-eval, a poker hand evaluator\n");

  // clang-format off
  desc.add_options()
      ("help,?", "produce help message")
      ("game,g", po::value<string>()->default_value("h"),
          "game to use for evaluation")
      ("board,b", po::value<string>(), "community cards for he/o/o8")
      ("hand,h", po::value<vector<string>>(), "a hand for evaluation")
      ("quiet,q", "produces no output");
  // clang-format on

  // make hand a positional argument
  po::positional_options_description p;
  p.add("hand", -1);

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv)
                .style(po::command_line_style::unix_style)
                .options(desc)
                .positional(p)
                .run(),
            vm);
  po::notify(vm);

  // check for help
  if (vm.count("help") || argc == 1) {
    cout << desc << endl;
    return 1;
  }

  // extract the options
  string game = vm["game"].as<string>();
  string board = vm.count("board") ? vm["board"].as<string>() : "";
  vector<string> hands = vm["hand"].as<vector<string>>();

  bool quiet = vm.count("quiet") > 0;

  // allocate evaluator and create card distributions
  boost::shared_ptr<PokerHandEvaluator> evaluator =
      PokerHandEvaluator::alloc(game);
  vector<CardDistribution> handDists;
  for (const string& hand : hands) {
    handDists.emplace_back();
    handDists.back().parse(hand);
  }

  // fill with random if necessary
  if (handDists.size() == 1) {
    handDists.emplace_back();
    handDists.back().fill(evaluator->handSize());
  }

  // calcuate the results and print them
  ShowdownEnumerator showdown;
  vector<EquityResult> results =
      showdown.calculateEquity(handDists, CardSet(board), evaluator);

  double total = 0.0;
  for (const EquityResult& result : results) {
    total += result.winShares + result.tieShares;
  }

  pt::ptree root;
  pt::ptree hands_node;
  
    for (size_t i = 0; i < results.size(); ++i) {
    string handDesc = (hands[i] == ".") ? "random" : hands[i];

    pt::ptree hand_node;
    hand_node.put("hand", handDesc);
    hand_node.put("win", results[i].winShares / total * 100);
    hand_node.put("tie", results[i].tieShares / total * 100);

    hands_node.push_back(make_pair("", hand_node));
  }
  root.add_child("hands", hands_node);
  
  write_json(cout, root);
}
