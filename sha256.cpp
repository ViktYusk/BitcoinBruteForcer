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

// TODO: изучить инструкции AdvSIMD (NEON не поддерживается процессором)
void sha256(unsigned* input, unsigned* output)
{
#ifdef COUNT_TEST
    sha256Counter++;
#endif
    unsigned temp;
    F3(input[1], input[16]); ADD2(input[16], input[0]);
    F3(input[2], input[17]); ADD3(input[17], input[1], 0x00A50000);
    F3(input[3], input[18]); F4(input[16], temp); ADD3(input[18], temp, input[2]);
    F3(input[4], input[19]); F4(input[17], temp); ADD3(input[19], temp, input[3]);
    F3(input[5], input[20]); F4(input[18], temp); ADD3(input[20], temp, input[4]);
    F3(input[6], input[21]); F4(input[19], temp); ADD3(input[21], temp, input[5]);
    F3(input[7], input[22]); F4(input[20], temp); ADD4(input[22], temp, input[6], 0x00000108);
    INPUT(input, 23, temp)
    F4(input[22], input[24]); ADD3(input[24], input[8], input[17]);
    F4(input[23], input[25]); ADD2(input[25], input[18]);
    F4(input[24], input[26]); ADD2(input[26], input[19]);
    F4(input[25], input[27]); ADD2(input[27], input[20]);
    F4(input[26], input[28]); ADD2(input[28], input[21]);
    F4(input[27], input[29]); ADD2(input[29], input[22]);
    F4(input[28], input[30]); ADD3(input[30], input[23], 0x10420023);
    F3(input[16], input[31]); F4(input[29], temp); ADD4(input[31], temp, input[24], 0x00000108);
    INPUT(input, 32, temp) INPUT(input, 33, temp) INPUT(input, 34, temp) INPUT(input, 35, temp)
    INPUT(input, 36, temp) INPUT(input, 37, temp) INPUT(input, 38, temp) INPUT(input, 39, temp)
    INPUT(input, 40, temp) INPUT(input, 41, temp) INPUT(input, 42, temp) INPUT(input, 43, temp)
    INPUT(input, 44, temp) INPUT(input, 45, temp) INPUT(input, 46, temp) INPUT(input, 47, temp)
    INPUT(input, 48, temp) INPUT(input, 49, temp) INPUT(input, 50, temp) INPUT(input, 51, temp)
    INPUT(input, 52, temp) INPUT(input, 53, temp) INPUT(input, 54, temp) INPUT(input, 55, temp)
    INPUT(input, 56, temp) INPUT(input, 57, temp) INPUT(input, 58, temp) INPUT(input, 59, temp)
    INPUT(input, 60, temp) INPUT(input, 61, temp) INPUT(input, 62, temp) INPUT(input, 63, temp)
    unsigned states[8];
    output[0] = states[0] = 0x6A09E667;
    output[1] = states[1] = 0xBB67AE85;
    output[2] = states[2] = 0x3C6EF372;
    output[3] = states[3] = 0xA54FF53A;
    output[4] = states[4] = 0x510E527F;
    output[5] = states[5] = 0x9B05688C;
    output[6] = states[6] = 0x1F83D9AB;
    output[7] = states[7] = 0x5BE0CD19;
    unsigned temp1, temp2;
    ROUND(states, input, 0, temp1, temp2, temp) ROUND(states, input, 1, temp1, temp2, temp)
    ROUND(states, input, 2, temp1, temp2, temp) ROUND(states, input, 3, temp1, temp2, temp)
    ROUND(states, input, 4, temp1, temp2, temp) ROUND(states, input, 5, temp1, temp2, temp)
    ROUND(states, input, 6, temp1, temp2, temp) ROUND(states, input, 7, temp1, temp2, temp)
    ROUND(states, input, 8, temp1, temp2, temp) ROUND(states, input, 9, temp1, temp2, temp)
    ROUND(states, input, 10, temp1, temp2, temp) ROUND(states, input, 11, temp1, temp2, temp)
    ROUND(states, input, 12, temp1, temp2, temp) ROUND(states, input, 13, temp1, temp2, temp)
    ROUND(states, input, 14, temp1, temp2, temp) ROUND(states, input, 15, temp1, temp2, temp)
    ROUND(states, input, 16, temp1, temp2, temp) ROUND(states, input, 17, temp1, temp2, temp)
    ROUND(states, input, 18, temp1, temp2, temp) ROUND(states, input, 19, temp1, temp2, temp)
    ROUND(states, input, 20, temp1, temp2, temp) ROUND(states, input, 21, temp1, temp2, temp)
    ROUND(states, input, 22, temp1, temp2, temp) ROUND(states, input, 23, temp1, temp2, temp)
    ROUND(states, input, 24, temp1, temp2, temp) ROUND(states, input, 25, temp1, temp2, temp)
    ROUND(states, input, 26, temp1, temp2, temp) ROUND(states, input, 27, temp1, temp2, temp)
    ROUND(states, input, 28, temp1, temp2, temp) ROUND(states, input, 29, temp1, temp2, temp)
    ROUND(states, input, 30, temp1, temp2, temp) ROUND(states, input, 31, temp1, temp2, temp)
    ROUND(states, input, 32, temp1, temp2, temp) ROUND(states, input, 33, temp1, temp2, temp)
    ROUND(states, input, 34, temp1, temp2, temp) ROUND(states, input, 35, temp1, temp2, temp)
    ROUND(states, input, 36, temp1, temp2, temp) ROUND(states, input, 37, temp1, temp2, temp)
    ROUND(states, input, 38, temp1, temp2, temp) ROUND(states, input, 39, temp1, temp2, temp)
    ROUND(states, input, 40, temp1, temp2, temp) ROUND(states, input, 41, temp1, temp2, temp)
    ROUND(states, input, 42, temp1, temp2, temp) ROUND(states, input, 43, temp1, temp2, temp)
    ROUND(states, input, 44, temp1, temp2, temp) ROUND(states, input, 45, temp1, temp2, temp)
    ROUND(states, input, 46, temp1, temp2, temp) ROUND(states, input, 47, temp1, temp2, temp)
    ROUND(states, input, 48, temp1, temp2, temp) ROUND(states, input, 49, temp1, temp2, temp)
    ROUND(states, input, 50, temp1, temp2, temp) ROUND(states, input, 51, temp1, temp2, temp)
    ROUND(states, input, 52, temp1, temp2, temp) ROUND(states, input, 53, temp1, temp2, temp)
    ROUND(states, input, 54, temp1, temp2, temp) ROUND(states, input, 55, temp1, temp2, temp)
    ROUND(states, input, 56, temp1, temp2, temp) ROUND(states, input, 57, temp1, temp2, temp)
    ROUND(states, input, 58, temp1, temp2, temp) ROUND(states, input, 59, temp1, temp2, temp)
    ROUND(states, input, 60, temp1, temp2, temp) ROUND(states, input, 61, temp1, temp2, temp)
    ROUND(states, input, 62, temp1, temp2, temp) ROUND(states, input, 63, temp1, temp2, temp)
    output[0] += states[0];
    output[1] += states[1];
    output[2] += states[2];
    output[3] += states[3];
    output[4] += states[4];
    output[5] += states[5];
    output[6] += states[6];
    output[7] += states[7];
}
