//
// Created by jhnos on 25-05-2025.
//

#ifndef HETA_MATH_H
#define HETA_MATH_H

#include <math.h>
#include <stdint.h>

typedef union {
    uint64_t bits64;
    uint32_t bits32[2];
    double num;
} HetaDoubleBits;


#define HETA_DOUBLE_QNAN_POS_MIN_BITS (UINT64_C(0x7FF8000000000000))
#define HETA_DOUBLE_QNAN_POS_MAX_BITS (UINT64_C(0x7FFFFFFFFFFFFFFF))
#define HETA_DOUBLE_NAN (heta_double_from_bits(HETA_DOUBLE_QNAN_POS_MIN_BITS))

static inline double heta_double_from_bits(uint64_t bits)
{
    HetaDoubleBits data;
    data.bits64 = bits;
    return data.num;
}

static inline uint64_t heta_double_to_bits(double num)
{
    HetaDoubleBits data;
    data.num = num;
    return data.bits64;
}

#endif //HETA_MATH_H
