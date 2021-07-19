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

#ifdef COUNT_TEST
unsigned long long sha256Counter = 0;
#endif

void sha256(unsigned input[], unsigned output[])
{
#ifdef COUNT_TEST
    sha256Counter++;
#endif
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


/*
void sha256(unsigned inputs[][64], unsigned outputs[][8])
{
#ifdef COUNT_TEST
sha256Counter++;
#endif
    INPUT_16_SIMD(); INPUT_17_SIMD(); INPUT_18_21_SIMD(18); INPUT_18_21_SIMD(19);
    INPUT_18_21_SIMD(20); INPUT_18_21_SIMD(21); INPUT_SIMD(22); INPUT_SIMD(23);
    INPUT_24_SIMD(); INPUT_25_29_SIMD(25); INPUT_25_29_SIMD(26); INPUT_25_29_SIMD(27);
    INPUT_25_29_SIMD(28); INPUT_25_29_SIMD(29); INPUT_30_SIMD(); INPUT_SIMD(31);
    INPUT_SIMD(32); INPUT_SIMD(33); INPUT_SIMD(34); INPUT_SIMD(35);
    INPUT_SIMD(36); INPUT_SIMD(37); INPUT_SIMD(38); INPUT_SIMD(39);
    INPUT_SIMD(40); INPUT_SIMD(41); INPUT_SIMD(42); INPUT_SIMD(43);
    INPUT_SIMD(44); INPUT_SIMD(45); INPUT_SIMD(46); INPUT_SIMD(47);
    INPUT_SIMD(48); INPUT_SIMD(49); INPUT_SIMD(50); INPUT_SIMD(51);
    INPUT_SIMD(52); INPUT_SIMD(53); INPUT_SIMD(54); INPUT_SIMD(55);
    INPUT_SIMD(56); INPUT_SIMD(57); INPUT_SIMD(58); INPUT_SIMD(59);
    INPUT_SIMD(60); INPUT_SIMD(61); INPUT_SIMD(62); INPUT_SIMD(63);
    unsigned words[4][8];
    outputs[0][0] = outputs[1][0] = outputs[2][0] = outputs[3][0] = words[0][0] = words[1][0] = words[2][0] = words[3][0] = 0x6A09E667;
    outputs[0][1] = outputs[1][1] = outputs[2][1] = outputs[3][1] = words[0][1] = words[1][1] = words[2][1] = words[3][1] = 0xBB67AE85;
    outputs[0][2] = outputs[1][2] = outputs[2][2] = outputs[3][2] = words[0][2] = words[1][2] = words[2][2] = words[3][2] = 0x3C6EF372;
    outputs[0][3] = outputs[1][3] = outputs[2][3] = outputs[3][3] = words[0][3] = words[1][3] = words[2][3] = words[3][3] = 0xA54FF53A;
    outputs[0][4] = outputs[1][4] = outputs[2][4] = outputs[3][4] = words[0][4] = words[1][4] = words[2][4] = words[3][4] = 0x510E527F;
    outputs[0][5] = outputs[1][5] = outputs[2][5] = outputs[3][5] = words[0][5] = words[1][5] = words[2][5] = words[3][5] = 0x9B05688C;
    outputs[0][6] = outputs[1][6] = outputs[2][6] = outputs[3][6] = words[0][6] = words[1][6] = words[2][6] = words[3][6] = 0x1F83D9AB;
    outputs[0][7] = outputs[1][7] = outputs[2][7] = outputs[3][7] = words[0][7] = words[1][7] = words[2][7] = words[3][7] = 0x5BE0CD19;
    ROUND_SIMD(0); ROUND_SIMD(1); ROUND_SIMD(2); ROUND_SIMD(3);
    ROUND_SIMD(4); ROUND_SIMD(5); ROUND_SIMD(6); ROUND_SIMD(7);
    ROUND_SIMD(8); ROUND_SIMD(9); ROUND_SIMD(10); ROUND_SIMD(11);
    ROUND_SIMD(12); ROUND_SIMD(13); ROUND_SIMD(14); ROUND_SIMD(15);
    ROUND_SIMD(16); ROUND_SIMD(17); ROUND_SIMD(18); ROUND_SIMD(19);
    ROUND_SIMD(20); ROUND_SIMD(21); ROUND_SIMD(22); ROUND_SIMD(23);
    ROUND_SIMD(24); ROUND_SIMD(25); ROUND_SIMD(26); ROUND_SIMD(27);
    ROUND_SIMD(28); ROUND_SIMD(29); ROUND_SIMD(30); ROUND_SIMD(31);
    ROUND_SIMD(32); ROUND_SIMD(33); ROUND_SIMD(34); ROUND_SIMD(35);
    ROUND_SIMD(36); ROUND_SIMD(37); ROUND_SIMD(38); ROUND_SIMD(39);
    ROUND_SIMD(40); ROUND_SIMD(41); ROUND_SIMD(42); ROUND_SIMD(43);
    ROUND_SIMD(44); ROUND_SIMD(45); ROUND_SIMD(46); ROUND_SIMD(47);
    ROUND_SIMD(48); ROUND_SIMD(49); ROUND_SIMD(50); ROUND_SIMD(51);
    ROUND_SIMD(52); ROUND_SIMD(53); ROUND_SIMD(54); ROUND_SIMD(55);
    ROUND_SIMD(56); ROUND_SIMD(57); ROUND_SIMD(58); ROUND_SIMD(59);
    ROUND_SIMD(60); ROUND_SIMD(61); ROUND_SIMD(62); ROUND_SIMD(63);
    ADD_SIMD(outputs[0][0], outputs[0][1], outputs[0][2], outputs[0][3], words[0][0], words[0][1], words[0][2], words[0][3]); ADD_SIMD(outputs[0][4], outputs[0][5], outputs[0][6], outputs[0][7], words[0][4], words[0][5], words[0][6], words[0][7]);
    ADD_SIMD(outputs[1][0], outputs[1][1], outputs[1][2], outputs[1][3], words[1][0], words[1][1], words[1][2], words[1][3]); ADD_SIMD(outputs[1][4], outputs[1][5], outputs[1][6], outputs[1][7], words[1][4], words[1][5], words[1][6], words[1][7]);
    ADD_SIMD(outputs[2][0], outputs[2][1], outputs[2][2], outputs[2][3], words[2][0], words[2][1], words[2][2], words[2][3]); ADD_SIMD(outputs[2][4], outputs[2][5], outputs[2][6], outputs[2][7], words[2][4], words[2][5], words[2][6], words[2][7]);
    ADD_SIMD(outputs[3][0], outputs[3][1], outputs[3][2], outputs[3][3], words[3][0], words[3][1], words[3][2], words[3][3]); ADD_SIMD(outputs[3][4], outputs[3][5], outputs[3][6], outputs[3][7], words[3][4], words[3][5], words[3][6], words[3][7]);
}
*/
