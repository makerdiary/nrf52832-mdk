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
#ifndef OCC_GE25519_H
#define OCC_GE25519_H

#include "occ_sc25519.h"
#include "occ_mod25519.h"


/**
 * projective coordinates
 */
typedef struct {
    /**@{
     * point = `(x/z, y/z)`
     */
    occ_mod25519 x;
    occ_mod25519 y;
    occ_mod25519 z;
    /**@}*/
} occ_ge25519_p2;

/**
 * extened coordinates
 */
typedef struct {
    /**@{
     * point = `(x/z, y/z)`
     * @remark `x*y = t*z`
     */
    occ_mod25519 x;
    occ_mod25519 y;
    occ_mod25519 z;
    occ_mod25519 t;
    /**@}*/
} occ_ge25519_p3;

/**
 * completed coordinates
 */
typedef struct {
    /**@{
     * point = `(e/g, h/f)`
     */
    occ_mod25519 e;
    occ_mod25519 f;
    occ_mod25519 g;
    occ_mod25519 h;
    /**@}*/
} occ_ge25519_p1p1;

/**
 * affine coordinates
 */
typedef struct {
    /**@{
     * point = `(x, y)`
     */
    occ_mod25519 x;
    occ_mod25519 y;
    /**@}*/
} occ_ge25519_aff;


/**
 * `r = -unpack(p)`
 *
 * @param[out] r `-unpack(p)`.
 * @param      p Packed curve point.
 *
 * @returns 0  If p is a legal curve point.
 * @returns -1 Otherwise
 */
int occ_ge25519_unpackneg(occ_ge25519_p3 *r, const uint8_t p[32]);

/**
 * `r = pack(p)`
 *
 * @param[out] r `pack(p)`.
 * @param      p Curve point.
 */
void occ_ge25519_pack(uint8_t r[32], const occ_ge25519_p2 *p);

/**
 * `r = s1 * p1 + s2 * basePoint`
 */
void occ_ge25519_double_scalarmult_vartime(occ_ge25519_p2 *r,
                                           const occ_ge25519_p3 *p1,
                                           const occ_sc25519 *s1,
                                           const occ_sc25519 *s2);

/**
 * `r = s * basePoint`
 */
void occ_ge25519_scalarmult_base(occ_ge25519_p2 *r, const occ_sc25519 *s);

#endif
