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
#ifndef OCC_BIGINT3072_H
#define OCC_BIGINT3072_H

#include <stdint.h>


/**@{
 * add
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
int occ_bigint_add768(uint32_t r[24], const uint32_t a[24], const uint32_t b[24]);
int occ_bigint_add1536(uint32_t r[48], const uint32_t a[48], const uint32_t b[48]);
int occ_bigint_add3072(uint32_t r[96], const uint32_t a[96], const uint32_t b[96]);
/**@}*/

/**@{
 * subtract
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
int occ_bigint_sub768(uint32_t r[24], const uint32_t a[24], const uint32_t b[24]);
int occ_bigint_sub1536(uint32_t r[48], const uint32_t a[48], const uint32_t b[48]);
int occ_bigint_sub3072(uint32_t r[96], const uint32_t a[96], const uint32_t b[96]);
/**@}*/

/**
 * 3072 bit conditional add
 *
 * `r += b ? a : 0`
 *
 * @param[in,out] r @p a is added if @p b = 1.
 * @param         a Summand to add to @p r if condition @p b = 1.
 * @param         b Condition. Must be 0 or 1.
 *
 * @returns carry out (0 or 1).
 */
int occ_bigint_cadd3072(uint32_t r[96], const uint32_t a[96], int b);

/**
 * 3072 bit conditional subtract
 *
 * `r -= b ? a : 0`
 *
 * @param[in,out] r @p a is subtracted if @p b = 1.
 * @param         a Subtrahend to subtract from @p r if condition @p b = 1.
 * @param         b Condition. Must be 0 or 1.
 *
 * @returns carry out (0 or 1).
 */
int occ_bigint_csub3072(uint32_t r[96], const uint32_t a[96], int b);

/**@{
 * add signed word
 *
 * `r = a + w`
 *
 * @param[out] r Sum of @p a and @p w.
 * @param      a First summand.
 * @param      w Second summand.
 *
 * @returns signed carry out (-1,0,+1).
 *
 * @remark @p r may be same as @p a.
 */
int occ_bigint_addw256(uint32_t r[8], const uint32_t a[8], int w);
int occ_bigint_addw768(uint32_t r[24], const uint32_t a[24], int w);
int occ_bigint_addw1536(uint32_t r[48], const uint32_t a[48], int w);
/**@}*/

/**@{
 * positive difference and sign
 *
 * `r = |a - b|`
 *
 * @param[out] r Absolute difference of @p a and @p b.
 * @param      a Minuend.
 * @param      b Subtrahend.

 * @returns sign of `a - b`.
 *
 * @remark @p r may be same as @p a.
 */
int occ_bigint_diff256(uint32_t r[8], const uint32_t a[8], const uint32_t b[8]);
int occ_bigint_diff768(uint32_t r[24], const uint32_t a[24], const uint32_t b[24]);
int occ_bigint_diff1536(uint32_t r[48], const uint32_t a[48], const uint32_t b[48]);
/**@}*/

/**@{
 * signed addition
 *
 * `r = sign ? r - a : r + a`
 *
 * @param[in,out] r    `sign ? r - a : r + a`
 * @param         a    Summand to add to @p r with sign @p sign.
 * @param         sign Sign.
 *
 * @returns signed carry out (-1,0,+1).
 */
int occ_bigint_sadd512(uint32_t r[16], const uint32_t a[16], int sign);
int occ_bigint_sadd1536(uint32_t r[48], const uint32_t a[48], int sign);
int occ_bigint_sadd3072(uint32_t r[96], const uint32_t a[96], int sign);
/**@}*/

#endif
