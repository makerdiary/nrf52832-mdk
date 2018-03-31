/**
 * Copyright (c) 2016 - 2017, Nordic Semiconductor ASA
 * 
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 * 
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 * 
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 * 
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 * 
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */
#ifndef OCC_MOD25519_H
#define OCC_MOD25519_H

#include <stdint.h>


/**
 * modulo 2^255-19 number
 */
typedef struct
{
  uint32_t w[8]; /**< little endian */
}
occ_mod25519;


/**
 * final reduction modulo 2^255-19
 * @param[in,out] r Number to reduce.
 */
void occ_mod25519_freeze(occ_mod25519 *r);

/**
 * convert bytes to modulo number
 * @param[out] r Conversion result.
 * @param[in]  x Bytes to convert to number.
 */
void occ_mod25519_fromBytes(occ_mod25519 *r, const uint8_t x[32]);

/**
 * convert modulo number to bytes
 * @param[out] r Conversion result.
 * @param[in]  x Number to convert to bytes.
 */
void occ_mod25519_toBytes(uint8_t r[32], occ_mod25519 *x);

/**
 * `r = val`
 * @param[out] r   Result.
 * @param      val Value to assign.
 */
void occ_mod25519_init(occ_mod25519 *r, uint32_t val);

/**
 * negate modulo 2^255-19
 *
 * `r = -x mod 2^255-19`
 *
 * @param r Result.
 * @param x Value to negate.
 *
 * @remark @p r may be same as @p x.
 */
void occ_mod25519_neg(occ_mod25519 *r, const occ_mod25519 *x);

/**
 * get sign = lsb of fully reduced number
 *
 * @param[in,out] x Number to get sign of.
 *
 * @returns sign bit of @p x (0 or 1)
 *
 * @remark @p x will be fully reduced.
 */
int occ_mod25519_getsign(occ_mod25519 *x);

/**
 * compare two modulo numbers
 *
 * @param[in,out] x Number to compare with @p y.
 * @param[in,out] y Number to compare with @p x.
 *
 * @returns 1 If the two numbers are equal modulo p.
 * @returns 0 Otherwise.
 *
 * @remark @p x and @p y will be fully reduced.
 */
int occ_mod25519_equal(occ_mod25519 *x, occ_mod25519 *y);

/**
 * modulo add
 *
 * `r = (x + y) mod 2^255-19` @n
 * reduced to: 0 <= @p r < 2^256
 *
 * @param[out] r Sum of @p x and @p y modulo 2^255-19, reduced to 0 <= @p r < 2^256.
 * @param      x First summand.
 * @param      y Second summand.
 *
 * @remark @p r may be same as @p x or @p y.
 */
void occ_mod25519_add(occ_mod25519 *r, const occ_mod25519 *x, const occ_mod25519 *y);

/**
 * modulo subtract
 *
 * `r = (x - y) mod 2^255-19` @n
 * reduced to: 0 <= @p r < 2^256
 *
 * @param[out] r Difference of @p x and @p y modulo 2^255-19, reduced to 0 <= @p r < 2^256.
 * @param      x Minuend.
 * @param      y Subtrahend.
 *
 * @remark @p r may be same as @p x or @p y.
 */
void occ_mod25519_sub(occ_mod25519 *r, const occ_mod25519 *x, const occ_mod25519 *y);

/**
 * modulo multiply
 *
 * `r = (x * y) mod 2^255-19` @n
 * reduced to: 0 <= @p r < 2^256
 *
 * @param[out] r Product of @p x and @p y modulo 2^255-19, reduced to 0 <= @p r < 2^256.
 * @param      x First factor.
 * @param      y Second factor.
 *
 * @remark @p r may be same as @p x or @p y.
 */
void occ_mod25519_mul(occ_mod25519 *r, const occ_mod25519 *x, const occ_mod25519 *y);

/**
 * modulo square
 *
 * `r = x^2 mod 2^255-19` @n
 * reduced to: 0 <= @p r < 2^256
 *
 * @param[out] r Square of @p x modulo 2^255-19, reduced to 0 <= @p r < 2^256.
 * @param      x Base.
 *
 * @remark @p r may be same as @p x.
 */
void occ_mod25519_square(occ_mod25519 *r, const occ_mod25519 *x);

/**
 * modulo multiply by 121666
 *
 * `r = (x * 121666) mod 2^255-19` @n
 * reduced to: 0 <= @p r < 2^256
 *
 * @param[out] r Product of @p x and 121666 modulo 2^255-19, reduced to 0 <= @p r < 2^256.
 * @param      x Factor to multiply with 121666.
 *
 * @remark @p r may be same as @p x.
 */
void occ_mod25519_mul121666(occ_mod25519 *r, const occ_mod25519 *x);

/**
 * modulo power
 *
 * @param[out] r
 * @parblock
 * - invert == 0: <pre>r = x^(2^252-3)  (sqrt(x)/x)</pre>
 * - invert == 1: <pre>r = x^(2^255-21) (r = 1/x)</pre>
 * @endparblock
 * @param      x Base
 * @param      invert Function to apply. See @p r.
 *
 * @remark @p r may be same as @p x
 */
void occ_mod25519_power(occ_mod25519 *r, const occ_mod25519 *x, int invert);

#endif
