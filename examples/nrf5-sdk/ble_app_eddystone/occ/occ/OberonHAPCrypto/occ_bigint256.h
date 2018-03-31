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
#ifndef OCC_BIGINT256_H
#define OCC_BIGINT256_H

#include <stdint.h>
#include "occ_cpu_dep.h"


#ifndef occ_load_littleendian

/**
 * load a word from 4 bytes in memory, unaligned little endian
 */
uint32_t occ_load_littleendian(const uint8_t x[4]);

/**
 * store a word to 4 bytes in memory, unaligned little endian
 */
void occ_store_littleendian(uint8_t x[4], uint32_t u);

/**
 * load a word from 4 bytes in memory, unaligned big endian
 */
uint32_t occ_load_bigendian(const uint8_t x[4]);

/**
 * store a word to 4 bytes in memory, unaligned big endian
 */
void occ_store_bigendian(uint8_t x[4], uint32_t u);

#endif


/**
 * 128 bit add
 *
 * `r = a + b`
 *
 * @param[out] r Sum of @p a and @p b.
 * @param      a First summand.
 * @param      b Second summand.
 *
 * @returns carry out (0 or 1).
 *
 * @remark @p r may be same as @p a or @p b.
 */
int occ_bigint_add128(uint32_t r[4], const uint32_t a[4], const uint32_t b[4]);

/**
 * 256 bit add
 *
 * `r = a + b`
 *
 * @param[out] r Sum of @p a and @p b.
 * @param      a First summand.
 * @param      b Second summand.
 *
 * @returns carry out (0 or 1).
 *
 * @remark @p r may be same as @p a or @p b.
 */
int occ_bigint_add256(uint32_t r[8], const uint32_t a[8], const uint32_t b[8]);

/**
 * 256 bit subtract
 *
 * `r = a - b`
 *
 * @param[out] r Difference of @p a and @p b.
 * @param      a Minuend.
 * @param      b Subtrahend.
 *
 * @returns carry out (0 or 1).
 *
 * @remark @p r may be same as @p a or @p b.
 */
int occ_bigint_sub256(uint32_t r[8], const uint32_t a[8], const uint32_t b[8]);

/**
 * 128 bit multiply, 256 bit result
 *
 * `r = a * b`
 *
 * @param[out] r Product of @p a and @p b.
 * @param      a First factor.
 * @param      b Second factor.
 */
void occ_bigint_mul128(uint32_t r[8], const uint32_t a[4], const uint32_t b[4]);

/**
 * 256 bit multiply, 512 bit result
 *
 * `r = a * b`
 *
 * @param[out] r Product of @p a and @p b.
 * @param      a First factor.
 * @param      b Second factor.
 */
void occ_bigint_mul256(uint32_t r[16], const uint32_t a[8], const uint32_t b[8]);

/**
 * 256 bit square, 512 bit result
 *
 * `r = a ^ 2`
 *
 * @param[out] r Square of @p a.
 * @param      a Base.
 */
void occ_bigint_sqr256(uint32_t r[16], const uint32_t a[8]);

/**
 * conditional 256 bit add
 *
 * `r += b ? a : 0`
 *
 * @returns carry out (0 or 1).
 *
 * @param[in,out] r @p a is added if @p b = 1.
 * @param         a Summand to add to @p r if condition @p b = 1.
 * @param         b Condition. Must be 0 or 1.
 */
int occ_bigint_cadd256(uint32_t r[8], const uint32_t a[8], int b);

/**
 * conditional 256 bit subtract
 *
 * `r -= b ? a : 0`
 *
 * @returns carry out (0 or 1).
 *
 * @param[in,out] r @p a is added if @p b = 1.
 * @param         a Summand to add to @p r if condition @p b = 1.
 * @param         b Condition. Must be 0 or 1.
 */
int occ_bigint_csub256(uint32_t r[8], const uint32_t a[8], int b);

/**
 * conditional 256 bit swap
 *
 * `(x,y) = b ? (y,x) : (x,y)`
 *
 * @param[in,out] x Is swapped with @p y if @p b = 1.
 * @param[in,out] y Is swapped with @p x if @p b = 1.
 * @param         b Condition. Must be 0 or 1.
 */
void occ_bigint_cswap256(uint32_t x[8], uint32_t y[8], int b);

#endif
