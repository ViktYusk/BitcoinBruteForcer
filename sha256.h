#define ADD(word1, word2) __asm("ADD %w[w1], %w[w1], %w[w2]" : [w1] "+r" (word1) : [w2] "r" (word2))

#define INPUT_16(input) __asm( \
"MOV %w[o], %w[i15], ROR #7\n\t EOR %w[o], %w[o], %w[i15], ROR #18\n\t EOR %w[o], %w[o], %w[i15], LSR #3\n\t" \
"ADD %w[o], %w[o], %w[i16]" \
: [o] "+r" (input[16]) \
: [i15] "r" (input[1]), [i16] "r" (input[0]))

#define INPUT_17(input) __asm( \
"MOV %w[o], %w[i15], ROR #7\n\t EOR %w[o], %w[o], %w[i15], ROR #18\n\t EOR %w[o], %w[o], %w[i15], LSR #3\n\t" \
"ADD %w[o], %w[o], %w[i16]\n\t ADD %w[o], %w[o], %w[f4]" \
: [o] "+r" (input[17]) \
: [i15] "r" (input[2]), [i16] "r" (input[1]), [f4] "r" (0x00A50000))

#define INPUT_18_21(input, index) __asm( \
"MOV %w[o], %w[i15], ROR #7\n\t EOR %w[o], %w[o], %w[i15], ROR #18\n\t EOR %w[o], %w[o], %w[i15], LSR #3\n\t" \
"MOV w0, %w[i2], ROR #17\n\t EOR w0, w0, %w[i2], ROR #19\n\t EOR w0, w0, %w[i2], LSR #10\n\t" \
"ADD %w[o], %w[o], w0\n\t ADD %w[o], %w[o], %w[i16]" \
: [o] "+r" (input[index]) \
: [i2] "r" (input[index - 2]),[i15] "r" (input[index - 15]),[i16] "r" (input[index - 16]) \
: "w0")

#define INPUT_24(input) __asm( \
"MOV %w[o], %w[i2], ROR #17\n\t EOR %w[o], %w[o], %w[i2], ROR #19\n\t EOR %w[o], %w[o], %w[i2], LSR #10\n\t" \
"ADD %w[o], %w[o], %w[i7]\n\t ADD %w[o], %w[o], %w[i16]" \
: [o] "+r" (input[24]) \
: [i2] "r" (input[22]), [i7] "r" (input[17]), [i16] "r" (input[8]))

#define INPUT_25_29(input, index) __asm( \
"MOV %w[o], %w[i2], ROR #17\n\t EOR %w[o], %w[o], %w[i2], ROR #19\n\t EOR %w[o], %w[o], %w[i2], LSR #10\n\t" \
"ADD %w[o], %w[o], %w[i7]" \
: [o] "+r" (input[index]) \
: [i2] "r" (input[index - 2]), [i7] "r" (input[index - 7]))

#define INPUT_30(input) __asm( \
"MOV %w[o], %w[i2], ROR #17\n\t EOR %w[o], %w[o], %w[i2], ROR #19\n\t EOR %w[o], %w[o], %w[i2], LSR #10\n\t" \
"ADD %w[o], %w[o], %w[i7]\n\t ADD %w[o], %w[o], %w[f3]" \
: [o] "+r" (input[30]) \
: [i2] "r" (input[28]), [i7] "r" (input[23]), [f3] "r" (0x10420023))

#define INPUT(input, index) __asm( \
"MOV %w[o], %w[i15], ROR #7\n\t EOR %w[o], %w[o], %w[i15], ROR #18\n\t EOR %w[o], %w[o], %w[i15], LSR #3\n\t" \
"MOV w0, %w[i2], ROR #17\n\t EOR w0, w0, %w[i2], ROR #19\n\t EOR w0, w0, %w[i2], LSR #10\n\t" \
"ADD %w[o], %w[o], w0\n\t ADD %w[o], %w[o], %w[i7]\n\t ADD %w[o], %w[o], %w[i16]" \
: [o] "+r" (input[index]) \
: [i2] "r" (input[index - 2]), [i7] "r" (input[index - 7]),[i15] "r" (input[index - 15]),[i16] "r" (input[index - 16]) \
: "w0")

#define ROUND(input, words, index) __asm( \
"MOV w0, %w[w0], ROR #2\n\t EOR w0, w0, %w[w0], ROR #13\n\t EOR w0, w0, %w[w0], ROR #22\n\t" \
"MOV w1, %w[w4], ROR #6\n\t EOR w1, w1, %w[w4], ROR #11\n\t EOR w1, w1, %w[w4], ROR #25\n\t" \
"AND w2, %w[w0], %w[w1]\n\t AND w3, %w[w0], %w[w2]\n\t EOR w2, w2, w3\n\t AND w3, %w[w1], %w[w2]\n\t EOR w2, w2, w3\n\t" \
"ADD w0, w0, w2\n\t" \
"AND w3, %w[w4], %w[w5]\n\t BIC w2, %w[w6], %w[w4]\n\t EOR w3, w3, w2\n\t" \
"ADD w1, w1, %w[w7]\n\t ADD w1, w1, w3\n\t ADD w1, w1, %w[k]\n\t ADD w1, w1, %w[i]\n\t " \
"MOV %w[w7], %w[w6]\n\t MOV %w[w6], %w[w5]\n\t MOV %w[w5], %w[w4]\n\t MOV %w[w4], %w[w3]\n\t ADD %w[w4], %w[w4], w1\n\t MOV %w[w3], %w[w2]\n\t MOV %w[w2], %w[w1]\n\t MOV %w[w1], %w[w0]\n\t MOV %w[w0], w0\n\t ADD %w[w0], %w[w0], w1" \
: [w0] "+r" (words[0]), [w1] "+r" (words[1]), [w2] "+r" (words[2]), [w3] "+r" (words[3]), [w4] "+r" (words[4]), [w5] "+r" (words[5]), [w6] "+r" (words[6]), [w7] "+r" (words[7]) \
: [i] "r" (input[index]), [k] "r" (K[index]) \
: "w0", "w1", "w2", "w3")

/*
#define INPUT_16_SIMD() \
INPUT_16(inputs[0]); \
INPUT_16(inputs[1]); \
INPUT_16(inputs[2]); \
INPUT_16(inputs[3])

#define INPUT_17_SIMD() \
INPUT_17(inputs[0]); \
INPUT_17(inputs[1]); \
INPUT_17(inputs[2]); \
INPUT_17(inputs[3])

#define INPUT_18_21_SIMD(index) \
INPUT_18_21(inputs[0], index); \
INPUT_18_21(inputs[1], index); \
INPUT_18_21(inputs[2], index); \
INPUT_18_21(inputs[3], index)

#define INPUT_24_SIMD() \
INPUT_24(inputs[0]); \
INPUT_24(inputs[1]); \
INPUT_24(inputs[2]); \
INPUT_24(inputs[3])

#define INPUT_25_29_SIMD(index) \
INPUT_25_29(inputs[0], index); \
INPUT_25_29(inputs[1], index); \
INPUT_25_29(inputs[2], index); \
INPUT_25_29(inputs[3], index)

#define INPUT_30_SIMD() \
INPUT_30(inputs[0]); \
INPUT_30(inputs[1]); \
INPUT_30(inputs[2]); \
INPUT_30(inputs[3])

#define INPUT_SIMD(index) \
INPUT(inputs[0], index); \
INPUT(inputs[1], index); \
INPUT(inputs[2], index); \
INPUT(inputs[3], index)

//#define INPUT_SIMD(index) __asm( \
"MOV v2.s[0], %w[i02]\n\t MOV v2.s[1], %w[i12]\n\t MOV v2.s[2], %w[i22]\n\t MOV v2.s[3], %w[i32]\n\t" \
"MOV v7.s[0], %w[i07]\n\t MOV v7.s[1], %w[i17]\n\t MOV v7.s[2], %w[i27]\n\t MOV v7.s[3], %w[i37]\n\t" \
"MOV v15.s[0], %w[i015]\n\t MOV v15.s[1], %w[i115]\n\t MOV v15.s[2], %w[i215]\n\t MOV v15.s[3], %w[i315]\n\t" \
"MOV v16.s[0], %w[i016]\n\t MOV v16.s[1], %w[i116]\n\t MOV v16.s[2], %w[i216]\n\t MOV v16.s[3], %w[i316]\n\t" \
 \
"USHR v0.4s, v15.4s, #7\n\t SHL v1.4s, v15.4s, #25\n\t ORR v0.16b, v0.16b, v1.16b\n\t" \
"USHR v10.4s, v15.4s, #18\n\t SHL v11.4s, v15.4s, #14\n\t ORR v10.16b, v10.16b, v11.16b\n\t EOR v0.16b, v0.16b, v10.16b\n\t" \
"USHR v10.4s, v15.4s, #3\n\t EOR v0.16b, v0.16b, v10.16b\n\t" \
 \
"USHR v10.4s, v2.4s, #17\n\t SHL v11.4s, v2.4s, #15\n\t ORR v10.16b, v10.16b, v11.16b\n\t" \
"USHR v11.4s, v2.4s, #19\n\t SHL v12.4s, v2.4s, #13\n\t ORR v11.16b, v11.16b, v12.16b\n\t EOR v10.16b, v10.16b, v11.16b\n\t" \
"USHR v11.4s, v2.4s, #10\n\t EOR v10.16b, v10.16b, v11.16b\n\t" \
 \
"ADD v0.4s, v0.4s, v10.4s\n\t ADD v0.4s, v0.4s, v7.4s\n\t ADD v0.4s, v0.4s, v16.4s\n\t" \
 \
"MOV %w[o0], v0.s[0]\n\t MOV %w[o1], v0.s[1]\n\t MOV %w[o2], v0.s[2]\n\t MOV %w[o3], v0.s[3]" \
: [o0] "=r" (inputs[0][index]), [o1] "=r" (inputs[1][index]), [o2] "=r" (inputs[2][index]), [o3] "=r" (inputs[3][index]) \
: \
[i02] "r" (inputs[0][index - 2]), [i07] "r" (inputs[0][index - 7]),[i015] "r" (inputs[0][index - 15]),[i016] "r" (inputs[0][index - 16]), \
[i12] "r" (inputs[1][index - 2]), [i17] "r" (inputs[1][index - 7]),[i115] "r" (inputs[1][index - 15]),[i116] "r" (inputs[1][index - 16]), \
[i22] "r" (inputs[2][index - 2]), [i27] "r" (inputs[2][index - 7]),[i215] "r" (inputs[2][index - 15]),[i216] "r" (inputs[2][index - 16]), \
[i32] "r" (inputs[3][index - 2]), [i37] "r" (inputs[3][index - 7]),[i315] "r" (inputs[3][index - 15]),[i316] "r" (inputs[3][index - 16]) \
: "v0", "v1", "v2", "v7", "v10", "v11", "v12", "v15", "v16")


//#define ROUND_SIMD(index) __asm( \
                                 \
: \
[w00] "+r" (((unsigned long long*)words)[0][0]), [w02] "+r" (((unsigned long long*)words)[0][2]), [w04] "+r" (((unsigned long long*)words)[0][4]), [w06] "+r" (((unsigned long long*)words)[0][6]), \
[w10] "+r" (((unsigned long long*)words)[1][0]), [w12] "+r" (((unsigned long long*)words)[1][2]), [w14] "+r" (((unsigned long long*)words)[1][4]), [w16] "+r" (((unsigned long long*)words)[1][6]), \
[w20] "+r" (((unsigned long long*)words)[2][0]), [w22] "+r" (((unsigned long long*)words)[2][2]), [w24] "+r" (((unsigned long long*)words)[2][4]), [w26] "+r" (((unsigned long long*)words)[2][6]), \
[w30] "+r" (((unsigned long long*)words)[3][0]), [w32] "+r" (((unsigned long long*)words)[3][2]), [w34] "+r" (((unsigned long long*)words)[3][4]), [w36] "+r" (((unsigned long long*)words)[3][6]) \
: [i0] "r" (input0[index]), [i1] "r" (input1[index]), [i2] "r" (input2[index]), [i3] "r" (input3[index]), [k] "r" (K[index]) \
: )


#define ROUND_SIMD(index) \
ROUND(inputs[0], words[0], index); \
ROUND(inputs[1], words[1], index); \
ROUND(inputs[2], words[2], index); \
ROUND(inputs[3], words[3], index)

#define ADD_SIMD(word00, word01, word02, word03, word10, word11, word12, word13) __asm( \
"MOV v0.s[0], %w[w00]\n\t MOV v0.s[1], %w[w01]\n\t MOV v0.s[2], %w[w02]\n\t MOV v0.s[3], %w[w03]\n\t" \
"MOV v1.s[0], %w[w10]\n\t MOV v1.s[1], %w[w11]\n\t MOV v1.s[2], %w[w12]\n\t MOV v1.s[3], %w[w13]\n\t" \
"ADD v0.4s, v0.4s, v1.4s\n\t" \
"MOV %w[w00], v0.s[0]\n\t MOV %w[w01], v0.s[1]\n\t MOV %w[w02], v0.s[2]\n\t MOV %w[w03], v0.s[3]" \
: [w00] "+r" (word00), [w01] "+r" (word01), [w02] "+r" (word02), [w03] "+r" (word03) \
: [w10] "r" (word10), [w11] "r" (word11), [w12] "r" (word12), [w13] "r" (word13)        \
: "v0", "v1")
*/

extern const unsigned K[64];

void sha256(unsigned* input, unsigned* output);
//void sha256(unsigned inputs[][64], unsigned outputs[][8]);
