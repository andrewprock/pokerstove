/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 * $Id$
 */
#ifndef COMMON_ENUM_PARTITIONENUMERATOR_H_
#define COMMON_ENUM_PARTITIONENUMERATOR_H_

#include <cstdint>
#include <vector>
#include <boost/lexical_cast.hpp>
#include <pokerstove/util/combinations.h>

namespace pokerstove
{
// see http://cplusplus.com/reference/algorithm/set_difference/
// for original version.
// this version is shifted a bit as we use first1+first2 instead of first1
template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator myset_difference(InputIterator1 first1, InputIterator1 last1,
                                InputIterator2 first2, InputIterator2 last2,
                                OutputIterator result)
{
    InputIterator1 first = first1;
    while (first1 != last1 && first2 != last2)
    {
        if (*first1 < *(first + *first2))
            *result++ = *first1++;
        else if (*(first + *first2) < *first1)
            first2++;
        else
        {
            first1++;
            first2++;
        }
    }
    return copy(first1, last1, result);
}

typedef combinations Combos;

/**
 * this class enumerates over all partistions of a set of data
 * *given* the size of the partitions.
 */
class PartitionEnumerator2
{
public:
    /**
     * create a partition enumerator
     *
     * @setsize the size of the index set to enumerate over
     * @partitions the list of partition sizes to use for enumeration
     */
    PartitionEnumerator2(size_t setSize, const std::vector<size_t> partitions)
        : _setSize(setSize)
        , _parts(partitions)
        , _pcombos()
        , _subsets()
        , _masks(partitions.size())
    {
        int used = 0;
        for (size_t i = 0; i < _parts.size(); i++)
        {
            _pcombos.push_back(Combos(_setSize - used, _parts[i]));
            _subsets.push_back(std::vector<size_t>(_setSize - used));
            used += _parts[i];
            setup(static_cast<int>(i));
        }
    }

    /**
     * create a string of all indices
     */
    std::string index_str() const
    {
        std::string ret;
        for (size_t p = 0; p < numParts(); p++)
        {
            ret += (p ? " {" : "{");
            for (size_t i = 0; i < partSize(p); i++)
            {
                int el = boost::lexical_cast<int>(getIndex(p, i));
                ret += (i ? " " : "") + boost::lexical_cast<std::string>(el);
            }
            ret += "}";
        }
        return ret;
    }

    /**
     * string representation
     */
    std::string str() const
    {
        std::string ret;
        for (size_t p = 0; p < numParts(); p++)
        {
            ret += (p ? " {" : "{");
            for (size_t i = 0; i < partSize(p); i++)
            {
                int el = boost::lexical_cast<int>(get(p, i));
                ret += (i ? " " : "") + boost::lexical_cast<std::string>(el);
            }
            if (partSize(p) == 0)
                ret += "e";
            ret += "}";
        }
        return ret;
    }

    size_t numParts() const { return _parts.size(); }

    /**
     * size of a given partition
     */
    size_t partSize(size_t p) const { return _parts[p]; }

    /**
     * the the contents of specific part
     */
    size_t getIndex(size_t partnum, size_t index) const
    {
        return _pcombos[partnum][index];
    }

    size_t get(size_t partnum, size_t index) const
    {
        return _subsets[partnum][_pcombos[partnum][index]];
    }

    uint64_t getMask(size_t partnum) const { return _masks[partnum]; }

    std::vector<size_t> get(size_t partnum) const
    {
        std::vector<size_t> ret(_parts[partnum]);
        for (size_t i = 0; i < _parts[partnum]; i++)
            ret[i] = _subsets[partnum][_pcombos[partnum][i]];
        return ret;
    }

    bool next() { return incr(); }

private:
    size_t _setSize;
    std::vector<size_t> _parts;
    std::vector<Combos> _pcombos;
    std::vector<std::vector<size_t>> _subsets;
    mutable std::vector<uint64_t> _masks;

    bool incr() { return incr(numParts() - 1); }

    void makeMask(size_t partnum) const
    {
        if (partnum == 0)
        {
            _masks[0] = _pcombos[0].getMask();
        }
        else
        {
            const size_t* c = _pcombos[partnum].begin();
            std::vector<size_t>::const_iterator s = _subsets[partnum].begin();
            uint64_t ret = 0;

            // this loop is very expensive, it accounts for about *half*
            // of the total enumeration time
            for (size_t i = 0; i < _parts[partnum]; i++)
                ret |= UINT64_C(0x01) << s[*c++];
            _masks[partnum] = ret;
        }
    }

    // increment to the nth subset
    // this is meant to simulate an odomoeter running when a slot flips,
    // all slots further along "reset".  If a slot runs out of values,
    // then the slot before it flips
    bool incr(int n)
    {
        // workaround warning in whiel (always true);
        // TODO: verify not buggies!
        // int n = num;
        while (n >= 0)
        {
            if (_pcombos[n].next())
            {
                makeMask(n);
                while (++n < _parts.size())
                    setup(static_cast<int>(n));
                return true;
            }
            if (n == 0)
            {
                return false;
            }
            n--;
        }
        return false;
    }

    void setup(int n)  // setup the nth subset for enumerating
    {
        if (n >= static_cast<int>(_parts.size()))
            return;

        // set up the list for this combinator
        if (n - 1 >= 0)
        {
            myset_difference(_subsets[n - 1].begin(), _subsets[n - 1].end(),
                             _pcombos[n - 1].begin(), _pcombos[n - 1].end(),
                             _subsets[n].begin());
        }
        else if (n == 0)
        {
            size_t i = 0;
            for (auto it = _subsets[0].begin(); it != _subsets[0].end(); it++)
                *it = i++;
        }
        _pcombos[n].reset();
        makeMask(n);
    }
};
}  // namespace pokerstove

#endif  // COMMON_ENUM_PARTITIONENUMERATOR_H_
