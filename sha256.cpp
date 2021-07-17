/*
 * FIPS 180-2 SHA-224/256/384/512 implementation
 * Last update: 02/02/2007
 * Issue date:  04/30/2005
 *
 * Copyright (C) 2013, Con Kolivas <kernel@kolivas.org>
 * Copyright (C) 2005, 2007 Olivier Gay <olivier.gay@a3.epfl.ch>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the project nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE PROJECT AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <string.h>

#include "sha256.h"


unsigned sha256_k[64] =
            {0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
             0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
             0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
             0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
             0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
             0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
             0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
             0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
             0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
             0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
             0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
             0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
             0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
             0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
             0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
             0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};
#ifdef COUNT_TEST
unsigned long long sha256Counter = 0;
#endif

/* SHA-256 functions */


// TODO: оптимизировать с учётом особенностей входных данных
void sha256(unsigned* input, unsigned* output)
{
#ifdef COUNT_TEST
    sha256Counter++;
#endif
    output[0] = 0x6A09E667;
    output[1] = 0xBB67AE85;
    output[2] = 0x3C6EF372;
    output[3] = 0xA54FF53A;
    output[4] = 0x510E527F;
    output[5] = 0x9B05688C;
    output[6] = 0x1F83D9AB;
    output[7] = 0x5BE0CD19;

    unsigned wv[8];
    unsigned t1, t2;

    int j;

    for (j = 16; j < 64; j++)
        SHA256_SCR(input, j);

    //memcpy(wv, state, 32);
    wv[0] = output[0];
    wv[1] = output[1];
    wv[2] = output[2];
    wv[3] = output[3];
    wv[4] = output[4];
    wv[5] = output[5];
    wv[6] = output[6];
    wv[7] = output[7];

    for (j = 0; j < 64; j++) {
        t1 = wv[7] + SHA256_F2(wv[4]) + CH(wv[4], wv[5], wv[6])
             + sha256_k[j] + input[j];
        t2 = SHA256_F1(wv[0]) + MAJ(wv[0], wv[1], wv[2]);
        wv[7] = wv[6];
        wv[6] = wv[5];
        wv[5] = wv[4];
        wv[4] = wv[3] + t1;
        wv[3] = wv[2];
        wv[2] = wv[1];
        wv[1] = wv[0];
        wv[0] = t1 + t2;
    }

    output[0] += wv[0];
    output[1] += wv[1];
    output[2] += wv[2];
    output[3] += wv[3];
    output[4] += wv[4];
    output[5] += wv[5];
    output[6] += wv[6];
    output[7] += wv[7];
    /*
    UNPACK32(states[0], &output[0]);
    UNPACK32(states[1], &output[4]);
    UNPACK32(states[2], &output[8]);
    UNPACK32(states[3], &output[12]);
    UNPACK32(states[4], &output[16]);
    UNPACK32(states[5], &output[20]);
    UNPACK32(states[6], &output[24]);
    UNPACK32(states[7], &output[28]);
    */
}
