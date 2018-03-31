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
#ifndef OCC_SC25519_H
#define OCC_SC25519_H

#include <stdint.h>


/**
 * ed25519 scalar modulo group order
 */
typedef struct
{
    uint32_t w[8]; /**< little endian */
}
occ_sc25519;


/**
 * 32 byte value to scalar
 *
 * @param[out] r Conversion result.
 * @param      x Bytes to convert to scalar.
 */
void occ_sc25519_from32bytes(occ_sc25519 *r, const uint8_t x[32]);

/**
 * 64 byte value to scalar
 *
 * @param[out] r Conversion result.
 * @param      x Bytes to convert to scalar.
 */
void occ_sc25519_from64bytes(occ_sc25519 *r, const uint8_t x[64]);

/**
 * scalar to 32 byte value
 *
 * @param[out] r Conversion result.
 * @param      x Scalar to convert to bytes.
 */
void occ_sc25519_to32bytes(uint8_t r[32], const occ_sc25519 *x);

/**
 * add scalars
 *
 * `r = x + y mod m` (the ed25519 group order)
 *
 * @param[out] r Sum of @p x and @p y modulo the ed25519 group order.
 * @param      x First summand.
 * @param      y Second summand.
 */
void occ_sc25519_add(occ_sc25519 *r, const occ_sc25519 *x, const occ_sc25519 *y);

/**
 * multiply scalars
 *
 * `r = x * y mod m` (the ed25519 group order)
 *
 * @param[out] r Product of @p x and @p y modulo the ed25519 group order.
 * @param      x First factor.
 * @param      y Second factor.
 */
void occ_sc25519_mul(occ_sc25519 *r, const occ_sc25519 *x, const occ_sc25519 *y);

/**
 * setup fixed 4 bit windows
 */
void occ_sc25519_window4(int8_t r[64], const occ_sc25519 *s);

/**
 * setup sliding 4 bit windows
 */
void occ_sc25519_sliding_window4(int8_t b[253], const occ_sc25519 *s);

#endif
