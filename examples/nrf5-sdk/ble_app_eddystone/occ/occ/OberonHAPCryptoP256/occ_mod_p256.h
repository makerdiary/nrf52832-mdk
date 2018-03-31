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
#ifndef OCC_MOD_P256_H
#define OCC_MOD_P256_H

#include <stdint.h>


// modulo P-256 number
typedef struct
{
  uint32_t w[8]; // little endian
}
occ_mod_p256;

// convert bytes to modulo number
void occ_mod_p256_fromBytes(occ_mod_p256 *r, const uint8_t x[32]);

// convert modulo number to bytes
void occ_mod_p256_toBytes(uint8_t r[32], const occ_mod_p256 *x);

// r = val
void occ_mod_p256_init(occ_mod_p256 *r, uint32_t val);

// conditional negate modulo P-256
// r = b ? -a : a
// condition b must be 0 or 1
// r may be same as a
void occ_mod_p256_cneg(occ_mod_p256 *r, const occ_mod_p256 *a, int b);

// get sign = lsb of fully reduced number
// returns sign bit of x
int occ_mod_p256_getsign(const occ_mod_p256 *x);

// modulo add
// r = (x + y) mod P-256
// reduced to: 0 <= r < P
// r may be same as x or y
void occ_mod_p256_add(occ_mod_p256 *r, const occ_mod_p256 *x, const occ_mod_p256 *y);

// modulo subtract
// r = (x - y) mod P-256
// reduced to: 0 <= r < P
// r may be same as x or y
void occ_mod_p256_sub(occ_mod_p256 *r, const occ_mod_p256 *x, const occ_mod_p256 *y);

// modulo multiply
// r = (x * y) mod P-256
// reduced to: 0 <= r < P
// r may be same as x or y
void occ_mod_p256_mul(occ_mod_p256 *r, const occ_mod_p256 *x, const occ_mod_p256 *y);

// modulo square
// r = x^2 mod P-256
// reduced to: 0 <= r < P
// r may be same as x
void occ_mod_p256_square(occ_mod_p256 *r, const occ_mod_p256 *x);

// modulo invert
// r = x^(p-2) = 1/x
// r may be same as x
void occ_mod_p256_invert(occ_mod_p256 *r, const occ_mod_p256 *x);

// modulo square root
// r = x^(p+1)/4 = sqrt(x)
// r may be same as x
void occ_mod_p256_sqrt(occ_mod_p256 *r, const occ_mod_p256 *x);

#endif
