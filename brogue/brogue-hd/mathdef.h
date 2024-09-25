#pragma once

#include "broguedef.h"

namespace brogueHd::backend::math
{
    typedef struct randomRange {
        short lowerBound;
        short upperBound;
        short clumpFactor;
    } randomRange;

    // ------------
    // SQRT.c
    // ------------

    ///* Computing the number of leading zeros in a word. */
    //static int32_t clz(uint32_t x)
    //{
    //    int n;

    //    /* See "Hacker's Delight" book for more details */
    //    if (x == 0) return 32;
    //    n = 0;
    //    if (x <= 0x0000FFFF) { n = n + 16; x = x << 16; }
    //    if (x <= 0x00FFFFFF) { n = n + 8; x = x << 8; }
    //    if (x <= 0x0FFFFFFF) { n = n + 4; x = x << 4; }
    //    if (x <= 0x3FFFFFFF) { n = n + 2; x = x << 2; }
    //    if (x <= 0x7FFFFFFF) { n = n + 1; }

    //    return n;
    //}

    //unsigned fp_sqrt(unsigned val)
    //{
    //    unsigned x;
    //    int bitpos;
    //    unsigned long long v;

    //    if (!val)
    //        return val;

    //    /* clz = count-leading-zeros. bitpos is the position of the most significant bit,
    //        relative to "1" or 1 << FP_BASE */
    //    bitpos = FP_BASE - clz(val);

    //    /* Calculate our first estimate.
    //        We use the identity 2^a * 2^a = 2^(2*a) or:
    //         sqrt(2^a) = 2^(a/2)
    //    */
    //    if (bitpos > 0u) /* val > 1 */
    //        x = (1u << FP_BASE) << (bitpos >> 1u);
    //    else if (bitpos < 0) /* 0 < val < 1 */
    //        x = (1u << FP_BASE) << ((unsigned)(-bitpos) << 1u);
    //    else /* val == 1 */
    //        x = (1u << FP_BASE);

    //    /* We need to scale val with FP_BASE due to the division.
    //       Also val /= 2, hence the subtraction of one*/
    //    v = (unsigned long long)val << (FP_BASE - 1u);

    //    /* The actual iteration */
    //    x = (x >> 1u) + v / x;
    //    x = (x >> 1u) + v / x;
    //    x = (x >> 1u) + v / x;
    //    x = (x >> 1u) + v / x;
    //    return x;
    //}

    
}
