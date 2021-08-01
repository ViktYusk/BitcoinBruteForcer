#include "sha256.h"

const unsigned K[64] =
{
    0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5DBA5, 0x3956C25B, 0x59F111F1, 0x923F82A4, 0xAB1C5ED5,
    0xD807AA98, 0x12835B01, 0x243185BE, 0x550C7DC3, 0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7, 0xC19BF174,
    0xE49B69C1, 0xEFBE4786, 0x0FC19DC6, 0x240CA1CC, 0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA,
    0x983E5152, 0xA831C66D, 0xB00327C8, 0xBF597FC7, 0xC6E00BF3, 0xD5A79147, 0x06CA6351, 0x14292967,
    0x27B70A85, 0x2E1B2138, 0x4D2C6DFC, 0x53380D13, 0x650A7354, 0x766A0ABB, 0x81C2C92E, 0x92722C85,
    0xA2BFE8A1, 0xA81A664B, 0xC24B8B70, 0xC76C51A3, 0xD192E819, 0xD6990624, 0xF40E3585, 0x106AA070,
    0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5, 0x391C0CB3, 0x4ED8AA4A, 0x5B9CCA4F, 0x682E6FF3,
    0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208, 0x90BEFFFA, 0xA4506CEB, 0xBEF9A3F7, 0xC67178F2
};

void sha256(unsigned input[], unsigned output[])
{
    INPUT_16(input); INPUT_17(input); INPUT_18_21(input, 18); INPUT_18_21(input, 19);
    INPUT_18_21(input, 20); INPUT_18_21(input, 21); INPUT(input, 22); INPUT(input, 23);
    INPUT_24(input); INPUT_25_29(input, 25); INPUT_25_29(input, 26); INPUT_25_29(input, 27);
    INPUT_25_29(input, 28); INPUT_25_29(input, 29); INPUT_30(input); INPUT(input, 31);
    INPUT(input, 32); INPUT(input, 33); INPUT(input, 34); INPUT(input, 35);
    INPUT(input, 36); INPUT(input, 37); INPUT(input, 38); INPUT(input, 39);
    INPUT(input, 40); INPUT(input, 41); INPUT(input, 42); INPUT(input, 43);
    INPUT(input, 44); INPUT(input, 45); INPUT(input, 46); INPUT(input, 47);
    INPUT(input, 48); INPUT(input, 49); INPUT(input, 50); INPUT(input, 51);
    INPUT(input, 52); INPUT(input, 53); INPUT(input, 54); INPUT(input, 55);
    INPUT(input, 56); INPUT(input, 57); INPUT(input, 58); INPUT(input, 59);
    INPUT(input, 60); INPUT(input, 61); INPUT(input, 62); INPUT(input, 63);
    unsigned words[8];
    output[0] = words[0] = 0x6A09E667;
    output[1] = words[1] = 0xBB67AE85;
    output[2] = words[2] = 0x3C6EF372;
    output[3] = words[3] = 0xA54FF53A;
    output[4] = words[4] = 0x510E527F;
    output[5] = words[5] = 0x9B05688C;
    output[6] = words[6] = 0x1F83D9AB;
    output[7] = words[7] = 0x5BE0CD19;
    ROUND(input, words, 0); ROUND(input, words, 1); ROUND(input, words, 2); ROUND(input, words, 3);
    ROUND(input, words, 4); ROUND(input, words, 5); ROUND(input, words, 6); ROUND(input, words, 7);
    ROUND(input, words, 8); ROUND(input, words, 9); ROUND(input, words, 10); ROUND(input, words, 11);
    ROUND(input, words, 12); ROUND(input, words, 13); ROUND(input, words, 14); ROUND(input, words, 15);
    ROUND(input, words, 16); ROUND(input, words, 17); ROUND(input, words, 18); ROUND(input, words, 19);
    ROUND(input, words, 20); ROUND(input, words, 21); ROUND(input, words, 22); ROUND(input, words, 23);
    ROUND(input, words, 24); ROUND(input, words, 25); ROUND(input, words, 26); ROUND(input, words, 27);
    ROUND(input, words, 28); ROUND(input, words, 29); ROUND(input, words, 30); ROUND(input, words, 31);
    ROUND(input, words, 32); ROUND(input, words, 33); ROUND(input, words, 34); ROUND(input, words, 35);
    ROUND(input, words, 36); ROUND(input, words, 37); ROUND(input, words, 38); ROUND(input, words, 39);
    ROUND(input, words, 40); ROUND(input, words, 41); ROUND(input, words, 42); ROUND(input, words, 43);
    ROUND(input, words, 44); ROUND(input, words, 45); ROUND(input, words, 46); ROUND(input, words, 47);
    ROUND(input, words, 48); ROUND(input, words, 49); ROUND(input, words, 50); ROUND(input, words, 51);
    ROUND(input, words, 52); ROUND(input, words, 53); ROUND(input, words, 54); ROUND(input, words, 55);
    ROUND(input, words, 56); ROUND(input, words, 57); ROUND(input, words, 58); ROUND(input, words, 59);
    ROUND(input, words, 60); ROUND(input, words, 61); ROUND(input, words, 62); ROUND(input, words, 63);
    ADD(output[0], words[0]); ADD(output[1], words[1]); ADD(output[2], words[2]); ADD(output[3], words[3]);
    ADD(output[4], words[4]); ADD(output[5], words[5]); ADD(output[6], words[6]); ADD(output[7], words[7]);
}
