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
#ifndef OCC_SRP_MATH_H
#define OCC_SRP_MATH_H

#include <stdint.h>


/**
 * scaled numbers for Montgomery multiplication
 */
typedef struct
{
    uint32_t w[96]; /**< `x * 2^1536 modulo p3072`, little endian */
}
occ_srp_mg;


/**
 * convert bytes to Montgomery scaled number
 *
 * @param[out] y Conversion result.
 * @param      x Bytes to convert.
 *
 * @remark @p y may be same as @p x.
 */
void occ_srp_fromBytes(occ_srp_mg* y, const uint8_t x[384]);

/**
 * convert Montgomery scaled number to bytes
 *
 * @param[out] y Conversion result.
 * @param      x Montgomery scaled number to convert.
 */
void occ_srp_toBytes(uint8_t y[384], const occ_srp_mg* x);

/**
 * `r = a+b mod p3072`
 *
 * @remark @p r may be same as @p a or @p b.
 */
void occ_srp_addmod3072(occ_srp_mg* r, const occ_srp_mg* a, const occ_srp_mg* b);

/**
 * `r = a-b mod p3072`
 *
 * @remark @p r may be same as @p a or @p b.
 */
void occ_srp_submod3072(occ_srp_mg* r, const occ_srp_mg* a, const occ_srp_mg* b);

/**
 * `r = r+inc mod p3072`
 *
 * @returns 1 If r == 0.
 * @returns 0 Otherwise.
 */
int occ_srp_incmod3072(occ_srp_mg* r, int inc);

/**
 * `r = a*b/2^1536 mod p3072`
 *
 * @remark @p r may be same as @p a or @p b.
 */
void occ_srp_mulmod3072(occ_srp_mg* r, const occ_srp_mg* a, const occ_srp_mg* b);

/**
 * `r = a^b mod p3072`
 */
void occ_srp_expmod3072(occ_srp_mg* r, const occ_srp_mg* a, const uint8_t* b, unsigned int blen);

/**
 * `r = 5^b mod p3072`
 */
void occ_srp_base_expmod3072(occ_srp_mg* r, const uint8_t* b, unsigned int blen);


#endif
