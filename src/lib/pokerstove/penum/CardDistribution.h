/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 * $Id$
 */
#ifndef PENUM_CARDDISTRIBUTION_H_
#define PENUM_CARDDISTRIBUTION_H_

#include <map>
#include <string>
#include <vector>
#include <pokerstove/peval/CardSet.h>

namespace pokerstove
{
/**
 * Card distribution, a set of hands object which may have zero or more
 * cards set in each, along with associated weights.
 */
class CardDistribution
{
public:
    /**
     * Initialize it to a "random" distribution, which is defined as the empty
     * set
     */
    CardDistribution();                             //!< use 0 cards for all elements
    explicit CardDistribution(size_t n);            //!< use N cards for all elements
    explicit CardDistribution(const CardSet& cs);
    CardDistribution(const CardDistribution& cd);
    CardDistribution& operator=(const CardDistribution& other);

    virtual ~CardDistribution() {}                  //!< for the virtual methods

    void clear();

    void removeCards(const CardSet& dead);

    /**
     * return the total weight in the distribution
     */
    double weight() const;

    /**
     * this allows subclasses to downcast to the raw data interface
     */
    CardDistribution data() const;

    /**
     * Printing and parseing.
     * default implementations of these are trivial and support only raw cards
     */
    virtual bool parse(const std::string& input);  //!< parses according to distribution type

    virtual std::string display() const;           //!< prints ...

    /**
     * string of the raw hand=weight values in the distribution.  comma
     * separated
     */
    std::string str() const;

    void fill(int n);                     //!< fill with all n card permutations of deck
    void fill(const CardSet& cs, int n);  //!< fill with all n card permutations of CardSet

    /**
     * return the total number of hands in the distribution
     */
    size_t size() const;

    /**
     * return the set of cards at index
     */
    const CardSet& operator[](size_t index) const;

    /**
     * return the weight of a given set of cards
     */
    const double& operator[](const CardSet& cards) const;

    /**
     * We return a refernce to allow clients to set the weight using the
     * array syntax.
     */
    double& operator[](const CardSet& cards);

private:
    const double& weight(const CardSet& cards) const;  // operator[]

    std::vector<CardSet>      _handList;
    std::map<CardSet, double> _weights;
};

}  // namespace pokerstove

#endif  // PENUM_CARDDISTRIBUTION_H_
