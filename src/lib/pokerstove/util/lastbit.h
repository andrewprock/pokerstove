#ifndef __LASTBIT_H
#define __LASTBIT_H

#include <cstdint>
#include <pokerstove/util/utypes.h>

// only support these built-ins for these two compilers
#if defined(__clang__) || defined(__GNUC__)

inline uint firstbit(uint64_t v)
{
    // __builtin_clz(0) is undefined, so don't do that.
    if (v == 0)
        return 0;
    return 63 - __builtin_clzll(v);
}

inline int lastbit(uint32_t v)
{
    if (v == 0)
        return 0;
    return __builtin_ctz(v);
}

inline int lastbit(uint16_t v)
{
    if (v == 0)
        return 0;
    return __builtin_ctz(v);
}

inline int lastbit(uint64_t v)
{
    if (v == 0)
        return 0;
    return __builtin_ctzll(v);
}

inline int lastbit64(uint64_t v)
{
    if (v == 0)
        return 0;
    return __builtin_ctzll(v);
}

#else

inline uint firstbit(uint64_t v)
{
    // unsigned int v;  // 32-bit value to find the log2 of
    const uint64_t b[] = {0x2,    0xC,        0xF0,
                          0xFF00, 0xFFFF0000, UINT64_C(0xFFFFFFFF00000000)};
    const unsigned int S[] = {1, 2, 4, 8, 16, 32};
    int i;

    unsigned int r = 0;       // result of log2(v) will go here
    for (i = 5; i >= 0; i--)  // unroll for speed...
    {
        if (v & b[i])
        {
            v >>= S[i];
            r |= S[i];
        }
    }
    return r;
}

inline int lastbit(uint32_t v)
{
    int r;
    static const int MultiplyDeBruijnBitPosition[32] = {
        0,  1,  28, 2,  29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4,  8,
        31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6,  11, 5,  10, 9};
    r = MultiplyDeBruijnBitPosition[((uint32_t)((v & -v) * 0x077CB531U)) >> 27];
    return r;
}

inline int lastbit(uint16_t v)
{
    int r;
    static const int MultiplyDeBruijnBitPosition[32] = {
        0,  1,  28, 2,  29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4,  8,
        31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6,  11, 5,  10, 9};
    r = MultiplyDeBruijnBitPosition[((uint32_t)((v & -v) * 0x077CB531U)) >> 27];
    return r;
}

inline int lastbit(uint64_t v)
{
    if (v)
    {
        uint32_t lower = static_cast<uint32_t>(v & 0xFFFFFFFF);
        if (lower)
            return lastbit(lower);
        uint32_t upper =
            static_cast<uint32_t>((v & UINT64_C(0xFFFFFFFF00000000)) >> 32);

        // uint32_t upper = static_cast<uint32_t>((v>>32)&0xFFFFFFFF);
        return lastbit(upper) + 32;
    }
    return 0;
}

// this one is slow on XP 32
inline int lastbit64(uint64_t v)
{
    int r;  // put the result in r
    static const int
        Mod67BitPosition[] =  // map a bit value mod 37 to its position
        {
            0,  0,  1,  39, 2,  15, 40, 23, 3,  12, 16, 59, 41, 19, 24, 54, 4,
            0,  13, 10, 17, 62, 60, 28, 42, 30, 20, 51, 25, 44, 55, 47, 5,  32,
            0,  38, 14, 22, 11, 58, 18, 53, 63, 9,  61, 27, 29, 50, 43, 46, 31,
            37, 21, 57, 52, 8,  26, 49, 45, 36, 56, 7,  48, 35, 6,  34, 33,
        };
    r = Mod67BitPosition[(-v & v) % 67];

    // cout << (-v&v)%37 << " " << lastbit(v) << endl;
    //  cout << (-v&v)%67 << " " << lastbit(v) << " " << r << endl;
    //  if (lastbit(v) != r)
    //  cout << "fuck\n";

    return r;
}

#endif

#endif
