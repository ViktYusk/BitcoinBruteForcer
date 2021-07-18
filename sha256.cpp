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
    unsigned temp1, temp2, temp3, temp4;
    F3(input[1], input[16]); ADD2(input[16], input[0]);
    F3(input[2], input[17]); ADD3(input[17], input[1], 0x00A50000);
    F3(input[3], input[18]); F4(input[16], temp1); ADD3(input[18], temp1, input[2]);
    F3(input[4], input[19]); F4(input[17], temp2); ADD3(input[19], temp2, input[3]);
    F3(input[5], input[20]); F4(input[18], temp3); ADD3(input[20], temp3, input[4]);
    F3(input[6], input[21]); F4(input[19], temp4); ADD3(input[21], temp4, input[5]);
    F3(input[7], input[22]); F4(input[20], temp1); ADD4(input[22], temp1, input[6], 0x00000108);
    INPUT(input, 23);
    F4(input[22], input[24]); ADD3(input[24], input[8], input[17]);
    F4(input[23], input[25]); ADD2(input[25], input[18]);
    F4(input[24], input[26]); ADD2(input[26], input[19]);
    F4(input[25], input[27]); ADD2(input[27], input[20]);
    F4(input[26], input[28]); ADD2(input[28], input[21]);
    F4(input[27], input[29]); ADD2(input[29], input[22]);
    F4(input[28], input[30]); ADD3(input[30], input[23], 0x10420023);
    F3(input[16], input[31]); F4(input[29], temp1); ADD4(input[31], temp1, input[24], 0x00000108);
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
    ROUND(words, input, 0); ROUND(words, input, 1);
    ROUND(words, input, 2); ROUND(words, input, 3);
    ROUND(words, input, 4); ROUND(words, input, 5);
    ROUND(words, input, 6); ROUND(words, input, 7);
    ROUND(words, input, 8); ROUND(words, input, 9);
    ROUND(words, input, 10); ROUND(words, input, 11);
    ROUND(words, input, 12); ROUND(words, input, 13);
    ROUND(words, input, 14); ROUND(words, input, 15);
    ROUND(words, input, 16); ROUND(words, input, 17);
    ROUND(words, input, 18); ROUND(words, input, 19);
    ROUND(words, input, 20); ROUND(words, input, 21);
    ROUND(words, input, 22); ROUND(words, input, 23);
    ROUND(words, input, 24); ROUND(words, input, 25);
    ROUND(words, input, 26); ROUND(words, input, 27);
    ROUND(words, input, 28); ROUND(words, input, 29);
    ROUND(words, input, 30); ROUND(words, input, 31);
    ROUND(words, input, 32); ROUND(words, input, 33);
    ROUND(words, input, 34); ROUND(words, input, 35);
    ROUND(words, input, 36); ROUND(words, input, 37);
    ROUND(words, input, 38); ROUND(words, input, 39);
    ROUND(words, input, 40); ROUND(words, input, 41);
    ROUND(words, input, 42); ROUND(words, input, 43);
    ROUND(words, input, 44); ROUND(words, input, 45);
    ROUND(words, input, 46); ROUND(words, input, 47);
    ROUND(words, input, 48); ROUND(words, input, 49);
    ROUND(words, input, 50); ROUND(words, input, 51);
    ROUND(words, input, 52); ROUND(words, input, 53);
    ROUND(words, input, 54); ROUND(words, input, 55);
    ROUND(words, input, 56); ROUND(words, input, 57);
    ROUND(words, input, 58); ROUND(words, input, 59);
    ROUND(words, input, 60); ROUND(words, input, 61);
    ROUND(words, input, 62); ROUND(words, input, 63);
    ADD2_SIMD(output[0], output[1], output[2], output[3], words[0], words[1], words[2], words[3]);
    ADD2_SIMD(output[4], output[5], output[6], output[7], words[4], words[5], words[6], words[7]);
}

/*
void sha256(unsigned* input0, unsigned* input1, unsigned* input2, unsigned* input3, unsigned* output0, unsigned* output1, unsigned* output2, unsigned* output3)
{
#ifdef COUNT_TEST
sha256Counter++;
#endif
    unsigned temp0;
    F3_SIMD(input0[1], input1[1], input2[1], input3[1], input0[16], input1[16], input2[16], input3[16]); ADD2_SIMD(input0[16], input1[16], input2[16], input3[16], input0[0], input1[0], input2[0], input3[0]);
    F3_SIMD(input0[2], input1[2], input2[2], input3[2], input0[17], input1[17], input2[17], input3[17]); ADD3_SIMD(input0[17], input1[17], input2[17], input3[17], input0[1], input1[1], input2[1], input3[1], 0x00A50000, 0x00A50000, 0x00A50000, 0x00A50000);
    F3_SIMD(input0[3], input1[3], input2[3], input3[3], input0[18], input1[18], input2[18], input3[18]); F4_SIMD(input0[16], input1[16], input2[16], input3[16], temp0); ADD3_SIMD(input0[18], input1[18], input2[18], input3[18], temp0, input0[2], input1[2], input2[2], input3[2]);
    F3_SIMD(input0[4], input1[4], input2[4], input3[4], input0[19], input1[19], input2[19], input3[19]); F4_SIMD(input0[17], input1[17], input2[17], input3[17], temp0); ADD3_SIMD(input0[19], input1[19], input2[19], input3[19], temp0, input0[3], input1[3], input2[3], input3[3]);
    F3_SIMD(input0[5], input1[5], input2[5], input3[5], input0[20], input1[20], input2[20], input3[20]); F4_SIMD(input0[18], input1[18], input2[18], input3[18], temp0); ADD3_SIMD(input0[20], input1[20], input2[20], input3[20], temp0, input0[4], input1[4], input2[4], input3[4]);
    F3_SIMD(input0[6], input1[6], input2[6], input3[6], input0[21], input1[21], input2[21], input3[21]); F4_SIMD(input0[19], input1[19], input2[19], input3[19], temp0); ADD3_SIMD(input0[21], input1[21], input2[21], input3[21], temp0, input0[5], input1[5], input2[5], input3[5]);
    F3_SIMD(input0[7], input1[7], input2[7], input3[7], input0[22], input1[22], input2[22], input3[22]); F4_SIMD(input0[20], input1[20], input2[20], input3[20], temp0); ADD4_SIMD(input0[22], input1[22], input2[22], input3[22], temp0, input0[6], input1[6], input2[6], input3[6], 0x00000108, 0x00000108, 0x00000108, 0x00000108);
    INPUT_SIMD(input0, input1, input2, input3, 23, temp0)
    F4_SIMD(input0[22], input1[22], input2[22], input3[22], input0[24], input1[24], input2[24], input3[24]); ADD3_SIMD(input0[24], input1[24], input2[24], input3[24], input0[8], input1[8], input2[8], input3[8], input0[17], input1[17], input2[17], input3[17]);
    F4_SIMD(input0[23], input1[23], input2[23], input3[23], input0[25], input1[25], input2[25], input3[25]); ADD2_SIMD(input0[25], input1[25], input2[25] ,input3[25], input0[18], input1[18], input2[18], input3[18]);
    F4_SIMD(input0[24], input1[24], input2[24], input3[24], input0[26], input1[26], input2[26], input3[26]); ADD2_SIMD(input0[26], input1[26], input2[26], input3[26], input0[19], input1[19], input2[19], input3[19]);
    F4_SIMD(input0[25], input1[25], input2[25], input3[25], input0[27], input1[27], input2[27], input3[27]); ADD2_SIMD(input0[27], input1[27], input2[27], input3[27], input0[20], input1[20], input2[20], input3[20]);
    F4_SIMD(input0[26], input1[26], input2[26], input3[26], input0[28], input1[28], input2[28], input3[28]); ADD2_SIMD(input0[28], input1[28], input2[28], input3[28], input0[21], input1[21], input2[21], input3[21]);
    F4_SIMD(input0[27], input1[27], input2[27], input3[27], input0[29], input1[29], input2[29], input3[29]); ADD2_SIMD(input0[29], input1[29], input2[29], input3[29], input0[22], input1[22], input2[22], input3[22]);
    F4_SIMD(input0[28], input1[28], input2[28], input3[28], input0[30], input1[30], input2[30], input3[30]); ADD3_SIMD(input0[30], input1[30], input2[30], input3[30], input0[23], input1[23], input2[23], input3[23], 0x10420023, 0x10420023, 0x10420023, 0x10420023);
    F3_SIMD(input0[16], input1[16], input2[16], input3[16], input0[31], input1[31], input2[31], input3[31]); F4_SIMD(input0[29], input1[29], input2[29], input3[29], temp0); ADD4_SIMD(input0[31], input1[31], input2[31], input3[31], temp0,input0[24], input1[24], input2[24], input3[24], 0x00000108, 0x00000108, 0x00000108, 0x00000108);
    INPUT_SIMD(input0, input1, input2, input3, 32, temp0) INPUT_SIMD(input0, input1, input2, input3, 33, temp0) INPUT_SIMD(input0, input1, input2, input3, 34, temp0) INPUT_SIMD(input0, input1, input2, input3, 35, temp0)
    INPUT_SIMD(input0, input1, input2, input3, 36, temp0) INPUT_SIMD(input0, input1, input2, input3, 37, temp0) INPUT_SIMD(input0, input1, input2, input3, 38, temp0) INPUT_SIMD(input0, input1, input2, input3, 39, temp0)
    INPUT_SIMD(input0, input1, input2, input3, 40, temp0) INPUT_SIMD(input0, input1, input2, input3, 41, temp0) INPUT_SIMD(input0, input1, input2, input3, 42, temp0) INPUT_SIMD(input0, input1, input2, input3, 43, temp0)
    INPUT_SIMD(input0, input1, input2, input3, 44, temp0) INPUT_SIMD(input0, input1, input2, input3, 45, temp0) INPUT_SIMD(input0, input1, input2, input3, 46, temp0) INPUT_SIMD(input0, input1, input2, input3, 47, temp0)
    INPUT_SIMD(input0, input1, input2, input3, 48, temp0) INPUT_SIMD(input0, input1, input2, input3, 49, temp0) INPUT_SIMD(input0, input1, input2, input3, 50, temp0) INPUT_SIMD(input0, input1, input2, input3, 51, temp0)
    INPUT_SIMD(input0, input1, input2, input3, 52, temp0) INPUT_SIMD(input0, input1, input2, input3, 53, temp0) INPUT_SIMD(input0, input1, input2, input3, 54, temp0) INPUT_SIMD(input0, input1, input2, input3, 55, temp0)
    INPUT_SIMD(input0, input1, input2, input3, 56, temp0) INPUT_SIMD(input0, input1, input2, input3, 57, temp0) INPUT_SIMD(input0, input1, input2, input3, 58, temp0) INPUT_SIMD(input0, input1, input2, input3, 59, temp0)
    INPUT_SIMD(input0, input1, input2, input3, 60, temp0) INPUT_SIMD(input0, input1, input2, input3, 61, temp0) INPUT_SIMD(input0, input1, input2, input3, 62, temp0) INPUT_SIMD(input0, input1, input2, input3, 63, temp0)
    unsigned words0[8], words1[8], words2[8], words3[8];
    output0[0] = output1[0] = output2[0] = output3[0] = words0[0] = words1[0] = words2[0] = words3[0] = 0x6A09E667;
    output0[1] = output1[1] = output2[1] = output3[1] = words0[1] = words1[1] = words2[1] = words3[1] = 0xBB67AE85;
    output0[2] = output1[2] = output2[2] = output3[2] = words0[2] = words1[2] = words2[2] = words3[2] = 0x3C6EF372;
    output0[3] = output1[3] = output2[3] = output3[3] = words0[3] = words1[3] = words2[3] = words3[3] = 0xA54FF53A;
    output0[4] = output1[4] = output2[4] = output3[4] = words0[4] = words1[4] = words2[4] = words3[4] = 0x510E527F;
    output0[5] = output1[5] = output2[5] = output3[5] = words0[5] = words1[5] = words2[5] = words3[5] = 0x9B05688C;
    output0[6] = output1[6] = output2[6] = output3[6] = words0[6] = words1[6] = words2[6] = words3[6] = 0x1F83D9AB;
    output0[7] = output1[7] = output2[7] = output3[7] = words0[7] = words1[7] = words2[7] = words3[7] = 0x5BE0CD19;
    unsigned temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13;
    ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 0, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0) ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 1, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0)
    ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 2, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0) ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 3, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0)
    ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 4, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0) ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 5, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0)
    ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 6, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0) ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 7, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0)
    ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 8, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0) ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 9, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0)
    ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 10, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0) ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 11, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0)
    ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 12, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0) ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 13, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0)
    ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 14, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0) ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 15, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0)
    ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 16, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0) ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 17, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0)
    ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 18, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0) ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 19, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0)
    ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 20, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0) ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 21, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0)
    ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 22, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0) ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 23, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0)
    ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 24, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0) ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 25, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0)
    ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 26, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0) ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 27, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0)
    ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 28, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0) ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 29, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0)
    ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 30, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0) ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 31, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0)
    ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 32, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0) ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 33, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0)
    ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 34, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0) ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 35, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0)
    ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 36, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0) ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 37, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0)
    ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 38, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0) ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 39, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0)
    ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 40, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0) ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 41, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0)
    ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 42, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0) ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 43, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0)
    ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 44, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0) ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 45, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0)
    ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 46, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0) ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 47, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0)
    ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 48, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0) ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 49, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0)
    ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 50, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0) ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 51, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0)
    ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 52, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0) ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 53, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0)
    ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 54, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0) ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 55, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0)
    ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 56, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0) ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 57, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0)
    ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 58, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0) ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 59, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0)
    ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 60, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0) ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 61, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0)
    ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 62, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0) ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, 63, temp00, temp01, temp02, temp03, temp10, temp11, temp12, temp13, temp0)
    ADD2_SIMD(output0[0], output1[0], output2[0], output3[0], words0[0], words1[0], words2[0], words3[0]);
    ADD2_SIMD(output0[1], output1[1], output2[1], output3[1], words0[1], words1[1], words2[1], words3[1]);
    ADD2_SIMD(output0[2], output1[2], output2[2], output3[2], words0[2], words1[2], words2[2], words3[2]);
    ADD2_SIMD(output0[3], output1[3], output2[3], output3[3], words0[3], words1[3], words2[3], words3[3]);
    ADD2_SIMD(output0[4], output1[4], output2[4], output3[4], words0[4], words1[4], words2[4], words3[4]);
    ADD2_SIMD(output0[5], output1[5], output2[5], output3[5], words0[5], words1[5], words2[5], words3[5]);
    ADD2_SIMD(output0[6], output1[6], output2[6], output3[6], words0[6], words1[6], words2[6], words3[6]);
    ADD2_SIMD(output0[7], output1[7], output2[7], output3[7], words0[7], words1[7], words2[7], words3[7]);
}
*/