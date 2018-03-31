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
#ifndef OCC_SC_P256_H
#define OCC_SC_P256_H

#include <stdint.h>
#include "occ_mod_p256.h"


// P-256 scalar modulo group order
typedef struct
{
    uint32_t w[8]; // little endian
}
occ_sc_p256;


// 32 byte value to scalar
void occ_sc_p256_from32bytes(occ_sc_p256 *r, const uint8_t x[32]);

// modulo p256 value to scalar
void occ_sc_p256_from_mod256(occ_sc_p256 *r, const occ_mod_p256 *x);

// scalar to 32 byte value
void occ_sc_p256_to32bytes(uint8_t r[32], const occ_sc_p256 *x);

// add scalars
void occ_sc_p256_add(occ_sc_p256 *r, const occ_sc_p256 *x, const occ_sc_p256 *y);

// multiply scalars
void occ_sc_p256_mul(occ_sc_p256 *r, const occ_sc_p256 *x, const occ_sc_p256 *y);

// modulo inversion
// r = x^(q-2) = 1/x
void occ_sc_p256_invert(occ_sc_p256 *r, const occ_sc_p256 *x);


#endif
