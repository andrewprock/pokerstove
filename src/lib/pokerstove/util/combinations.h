/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 * $Id: combinations.h 2655 2012-06-30 04:58:10Z prock $
 */
#ifndef COMMON_ENUM_COMBINATIONS_H_
#define COMMON_ENUM_COMBINATIONS_H_

#include <algorithm>
#include <boost/lexical_cast.hpp>
#include <pokerstove/util/lastbit.h>
#include <pokerstove/util/utypes.h>

namespace pokerstove
{
/**
 * Generates the set of all N choose K combinations of K
 * indices less than N.
 */
class combinations
{
public:
    combinations(size_t n, size_t k)
        : n_(n)
        , k_(k)
        , didnull_(true)
    {
        if (k_ > 0)
            comb_ = new size_t[k_];
        else
            comb_ = NULL;
        for (size_t i = 0; i < k_; ++i)
            comb_[i] = i;
    }

    void reset(size_t n, size_t k)
    {
        if (k > k_)
        {
            delete[] comb_;
            comb_ = new size_t[k];
        }
        k_ = k;
        n_ = n;
        for (size_t i = 0; i < k_; ++i)
            comb_[i] = i;
    }

    combinations(const combinations& c)
        : n_(c.n_)
        , k_(c.k_)
        , didnull_(c.didnull_)
    {
        if (k_ > 0)
            comb_ = new size_t[k_];
        else
            comb_ = NULL;
        for (size_t i = 0; i < k_; ++i)
            comb_[i] = c.comb_[i];
    }

    void reset()
    {
        for (size_t i = 0; i < k_; ++i)
            comb_[i] = i;
        didnull_ = true;
    }

    std::string str() const
    {
        std::string ret;
        for (size_t i = 0; i < size(); i++)
            ret += boost::lexical_cast<std::string>((*this)[i]) + " ";
        return ret;
    }

    uint64_t getMask() const
    {
        uint64_t mask = 0;
        size_t* pcom = comb_;
        for (size_t i = 0; i < size(); i++)
            mask |= static_cast<uint64_t>(0x01) << *pcom++;
        return mask;
    }

    ~combinations() { delete[] comb_; }

    bool next()
    {
        // this is to properly hand the case where of Nc0
        if (k_ == 0)
            return (didnull_ = !didnull_);

        int ii = static_cast<int>(k_) - 1;
        while (ii >= 0 && comb_[ii] + k_ + 1 > n_ + ii)
            ii--;
        if (ii < 0)
            return false;
        comb_[ii]++;
        while (static_cast<size_t>(++ii) < k_)
            comb_[ii] = comb_[ii - 1] + 1;
        return true;
    }

    bool nextcomb() { return next(); }

    size_t operator[](size_t i) const { return comb_[i]; }

    size_t size() const { return k_; }

    size_t* begin() { return comb_; }

    const size_t* begin() const { return comb_; }

    size_t* end() { return comb_ + k_; }

    const size_t* end() const { return comb_ + k_; }

private:
    size_t* comb_;
    size_t n_;
    size_t k_;
    bool didnull_;
};

/**
 * Generates the set of all N choose K combinations of K indices less
 * than N.  Templated version, roughly 8% faster.
 */
template <unsigned int N>
class Combinations
{
public:
    Combinations()
        : K(1)
    {
        for (unsigned int i = 0; i < K; ++i)
            comb_[i] = i;
    }

    Combinations(unsigned int k)
        : K(k)
    {
        for (unsigned int i = 0; i < K; ++i)
            comb_[i] = i;
    }

    void reset()
    {
        for (unsigned int i = 0; i < K; ++i)
            comb_[i] = i;
    }

    void setK(int k) { K = k; }

    bool next()
    {
        int i = K - 1;
        while (i >= 0 && comb_[i] + K + 1 > N + i)
            i--;
        if (i < 0)
            return false;
        comb_[i]++;
        while (static_cast<unsigned int>(++i) < K)
            comb_[i] = comb_[i - 1] + 1;
        return true;
    }

    unsigned int operator[](size_t i) const { return comb_[i]; }

    size_t size() const { return K; }

private:
    unsigned int K;
    unsigned int comb_[N];
};

}  // namespace pokerstove
#endif  // COMMON_ENUM_COMBINATIONS_H_
