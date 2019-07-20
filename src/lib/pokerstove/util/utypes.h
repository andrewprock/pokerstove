#ifndef __UTYPES_H
#define __UTYPES_H

#include <cstdint>
#include <string>

typedef unsigned int uint;
// typedef unsigned char ubyte;

const uint64_t ONE64 = 1;

/**
 * Produce a string corresponding to the bits in the data.
 * A space separates every 8 bits.
 */
template <typename T>
std::string toBitString(T t)
{
    std::string ret;
    const int BYTE_SZ = 8;
    for (unsigned int i = 0; i < sizeof(T) * BYTE_SZ; i++)
    {
        if ((i % BYTE_SZ) == 0)
            ret += " ";
        if (t >> i & 0x01)
            ret += "1";
        else
            ret += "0";
    }
    return ret;
}

/**
 * Count the number of bits that are on in the input.  This may need
 * to be fixed to work with unsigned input
 */
#if 1

template <typename T>
int countbits(T v)
{
    size_t c;
    for (c = 0; v; c++)
    {
        v &= v - 1;  // clear the least significant bit set
    }
    return c;
}

#else
template <typename T>
int countbits(T x)
{
    int rval = 0;
    while (x != 0)
    {
        if (x & 0x01)
            rval++;
        x >>= 1;
    }
    return rval;
}
#endif

std::string makeCommaNumStr(int64_t n);

#endif
