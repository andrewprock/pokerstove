/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 * $Id$
 */
#ifndef COMMON_ENUM_ODOMETER_H_
#define COMMON_ENUM_ODOMETER_H_

#include <algorithm>
#include <string>
#include <vector>

/** This is a simple odomter class.  It runs each index from [0..max] where
 * the max of the ith index is the ith element in the extents vector passed
 * to the init function.  For example if the extent vector is: <2,4,3> the
 * odomoeter will generate these 3-tuples in this oder:
 *
 *  0  0  0
 *  0  0  1
 *  0  0  2
 *  0  1  0
 *  0  1  1
 *  0  1  2
 *  0  2  0
 *  0  2  1
 *  0  2  2
 *  0  3  0
 *  0  3  1
 *  0  3  2
 *  1  0  0
 *  1  0  1
 *  1  0  2
 *  1  1  0
 *  1  1  1
 *  1  1  2
 *  1  2  0
 *  1  2  1
 *  1  2  2
 *  1  3  0
 *  1  3  1
 *  1  3  2
 *
 * To check if more tuples are available, call hasMore() and to get the
 * next one, call getNext ().
 *
 * usage example:
 */
class Odometer
{
public:
    explicit Odometer(const std::vector<size_t>& extents)
        : _extents(extents.size())
        , _odom(extents.size(), 0)
    {
        for (size_t i = 0; i < extents.size(); i++)
            _extents[i] = static_cast<int>(extents[i]);
    }

    size_t size() const { return _odom.size(); }

    std::string str() const
    {
        std::string ret(size(), '0');
        for (size_t i = 0; i < ret.size(); i++)
            ret[i] += _odom[i];
        return ret;
    }

    bool next() { return flip(static_cast<int>(_odom.size() - 1)); }

    unsigned int operator[](size_t i) const { return _odom[i]; }

private:
    bool flip(int n)
    {
        if (n < 0)
            return false;
        _odom[n]++;
        if (_odom[n] >= _extents[n])
        {
            _odom[n] = 0;
            return flip(n - 1);
        }
        return true;
    }

    std::vector<int> _extents;
    std::vector<int> _odom;
};

#endif  // COMMON_ENUM_ODOMETER_H_
