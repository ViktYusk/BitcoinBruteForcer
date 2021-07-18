#define ADD2(word1, word2) __asm("ADD %w[w1], %w[w1], %w[w2]" : [w1] "+r" (word1) : [w2] "r" (word2))
#define ADD3(word1, word2, word3) __asm("ADD %w[w1], %w[w1], %w[w2]\n\t ADD %w[w1], %w[w1], %w[w3]" : [w1] "+r" (word1) : [w2] "r" (word2), [w3] "r" (word3))
#define ADD4(word1, word2, word3, word4) __asm("ADD %w[w1], %w[w1], %w[w2]\n\t ADD %w[w1], %w[w1], %w[w3]\n\t ADD %w[w1], %w[w1], %w[w4]" : [w1] "+r" (word1) : [w2] "r" (word2), [w3] "r" (word3), [w4] "r" (word4))
#define ADD5(word1, word2, word3, word4, word5) __asm("ADD %w[w1], %w[w1], %w[w2]\n\t ADD %w[w1], %w[w1], %w[w3]\n\t ADD %w[w1], %w[w1], %w[w4]\n\t ADD %w[w1], %w[w1], %w[w5]" : [w1] "+r" (word1) : [w2] "r" (word2), [w3] "r" (word3), [w4] "r" (word4), [w5] "r" (word5))
#define F(input, output, shift1, shift2, shift3) __asm("MOV %w[o], %w[i], ROR #" shift1 "\n\t EOR %w[o], %w[o], %w[i], ROR #" shift2 "\n\t EOR %w[o], %w[o], %w[i], ROR #" shift3 : [o] "+r" (output) :  [i] "r" (input))
#define F_(input, output, shift1, shift2, shift3) __asm("MOV %w[o], %w[i], ROR #" shift1 "\n\t EOR %w[o], %w[o], %w[i], ROR #" shift2 "\n\t EOR %w[o], %w[o], %w[i], LSR #" shift3 : [o] "+r" (output) :  [i] "r" (input))
#define F1(input, output) F(input, output, "2", "13", "22")
#define F2(input, output) F(input, output, "6", "11", "25")
#define F3(input, output) F_(input, output, "7", "18", "3")
#define F4(input, output) F_(input, output, "17", "19", "10")
#define CH(input1, input2, input3, output) __asm("AND %w[o], %w[i1], %w[i2]\n\t BIC w0, %w[i3], %w[i1]\n\t EOR %w[o], %w[o], w0" : [o] "+r" (output) : [i1] "r" (input1), [i2] "r" (input2), [i3] "r" (input3) : "w0");
#define MA(input1, input2, input3, output) __asm("AND %w[o], %w[i1], %w[i2]\n\t AND w0, %w[i1], %w[i3]\n\t EOR %w[o], %w[o], w0\n\t AND w0, %w[i2], %w[i3]\n\t EOR %w[o], %w[o], w0" : [o] "+r" (output): [i1] "r" (input1), [i2] "r" (input2), [i3] "r" (input3) : "w0");
#define INPUT(input, i) __asm( \
"MOV %w[o], %w[i15], ROR #7\n\t EOR %w[o], %w[o], %w[i15], ROR #18\n\t EOR %w[o], %w[o], %w[i15], LSR #3\n\t" \
"MOV w0, %w[i2], ROR #17\n\t EOR w0, w0, %w[i2], ROR #19\n\t EOR w0, w0, %w[i2], LSR #10\n\t" \
"ADD %w[o], %w[o], w0\n\t ADD %w[o], %w[o], %w[i7]\n\t ADD %w[o], %w[o], %w[i16]" \
: [o] "+r" (input[i]) \
: [i2] "r" (input[i - 2]), [i7] "r" (input[i - 7]),[i15] "r" (input[i - 15]),[i16] "r" (input[i - 16]) \
: "w0")

#define ROUND(words, input, index) __asm( \
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

#define ADD2_SIMD(word00, word01, word02, word03, word10, word11, word12, word13) __asm( \
"MOV v0.s[0], %w[w00]\n\t MOV v0.s[1], %w[w01]\n\t MOV v0.s[2], %w[w02]\n\t MOV v0.s[3], %w[w03]\n\t" \
"MOV v1.s[0], %w[w10]\n\t MOV v1.s[1], %w[w11]\n\t MOV v1.s[2], %w[w12]\n\t MOV v1.s[3], %w[w13]\n\t" \
"ADD v0.4s, v0.4s, v1.4s\n\t" \
"MOV %w[w00], v0.s[0]\n\t MOV %w[w01], v0.s[1]\n\t MOV %w[w02], v0.s[2]\n\t MOV %w[w03], v0.s[3]" \
: [w00] "+r" (word00), [w01] "+r" (word01), [w02] "+r" (word02), [w03] "+r" (word03) \
: [w10] "r" (word10), [w11] "r" (word11), [w12] "r" (word12), [w13] "r" (word13))

#define ADD3_SIMD(word00, word01, word02, word03, word10, word11, word12, word13, word20, word21, word22, word23) __asm( \
"MOV v0.s[0], %w[w00]\n\t MOV v0.s[1], %w[w01]\n\t MOV v0.s[2], %w[w02]\n\t MOV v0.s[3], %w[w03]\n\t" \
"MOV v1.s[0], %w[w10]\n\t MOV v1.s[1], %w[w11]\n\t MOV v1.s[2], %w[w12]\n\t MOV v1.s[3], %w[w13]\n\t" \
"MOV v2.s[0], %w[w20]\n\t MOV v2.s[1], %w[w21]\n\t MOV v2.s[2], %w[w22]\n\t MOV v2.s[3], %w[w23]\n\t" \
"ADD v0.4s, v0.4s, v1.4s\n\t" \
"ADD v0.4s, v0.4s, v2.4s\n\t" \
"MOV %w[w00], v0.s[0]\n\t MOV %w[w01], v0.s[1]\n\t MOV %w[w02], v0.s[2]\n\t MOV %w[w03], v0.s[3]" \
: [w00] "+r" (word00), [w01] "+r" (word01), [w02] "+r" (word02), [w03] "+r" (word03) \
: [w10] "r" (word10), [w11] "r" (word11), [w12] "r" (word12), [w13] "r" (word13), [w20] "r" (word20), [w21] "r" (word21), [w22] "r" (word22), [w23] "r" (word23))

#define ADD4_SIMD(word00, word01, word02, word03, word10, word11, word12, word13, word20, word21, word22, word23, word30, word31, word32, word33) __asm( \
"MOV v0.s[0], %w[w00]\n\t MOV v0.s[1], %w[w01]\n\t MOV v0.s[2], %w[w02]\n\t MOV v0.s[3], %w[w03]\n\t" \
"MOV v1.s[0], %w[w10]\n\t MOV v1.s[1], %w[w11]\n\t MOV v1.s[2], %w[w12]\n\t MOV v1.s[3], %w[w13]\n\t" \
"MOV v2.s[0], %w[w20]\n\t MOV v2.s[1], %w[w21]\n\t MOV v2.s[2], %w[w22]\n\t MOV v2.s[3], %w[w23]\n\t" \
"MOV v3.s[0], %w[w30]\n\t MOV v3.s[1], %w[w31]\n\t MOV v3.s[2], %w[w32]\n\t MOV v3.s[3], %w[w33]\n\t" \
"ADD v0.4s, v0.4s, v1.4s\n\t" \
"ADD v0.4s, v0.4s, v2.4s\n\t" \
"ADD v0.4s, v0.4s, v3.4s\n\t" \
"MOV %w[w00], v0.s[0]\n\t MOV %w[w01], v0.s[1]\n\t" "MOV %w[w02], v0.s[2]\n\t MOV %w[w03], v0.s[3]" \
: [w00] "+r" (word00), [w01] "+r" (word01), [w02] "+r" (word02), [w03] "+r" (word03) \
: [w10] "r" (word10), [w11] "r" (word11), [w12] "r" (word12), [w13] "r" (word13), [w20] "r" (word20), [w21] "r" (word21), [w22] "r" (word22), [w23] "r" (word23), [w30] "r" (word30), [w31] "r" (word31), [w32] "r" (word32), [w33] "r" (word33))

#define ADD5_SIMD(word00, word01, word02, word03, word10, word11, word12, word13, word20, word21, word22, word23, word30, word31, word32, word33, word40, word41, word42, word43) __asm( \
"MOV v0.s[0], %w[w00]\n\t MOV v0.s[1], %w[w01]\n\t MOV v0.s[2], %w[w02]\n\t MOV v0.s[3], %w[w03]\n\t" \
"MOV v1.s[0], %w[w10]\n\t MOV v1.s[1], %w[w11]\n\t MOV v1.s[2], %w[w12]\n\t MOV v1.s[3], %w[w13]\n\t" \
"MOV v2.s[0], %w[w20]\n\t MOV v2.s[1], %w[w21]\n\t MOV v2.s[2], %w[w22]\n\t MOV v2.s[3], %w[w23]\n\t" \
"MOV v3.s[0], %w[w30]\n\t MOV v3.s[1], %w[w31]\n\t MOV v3.s[2], %w[w32]\n\t MOV v3.s[3], %w[w33]\n\t" \
"MOV v4.s[0], %w[w40]\n\t MOV v4.s[1], %w[w41]\n\t MOV v4.s[2], %w[w42]\n\t MOV v4.s[3], %w[w43]\n\t" \
"ADD v0.4s, v0.4s, v1.4s\n\t" \
"ADD v0.4s, v0.4s, v2.4s\n\t" \
"ADD v0.4s, v0.4s, v3.4s\n\t" \
"ADD v0.4s, v0.4s, v4.4s\n\t" \
"MOV %w[w00], v0.s[0]\n\t MOV %w[w01], v0.s[1]\n\t" "MOV %w[w02], v0.s[2]\n\t MOV %w[w03], v0.s[3]" \
: [w00] "+r" (word00), [w01] "+r" (word01), [w02] "+r" (word02), [w03] "+r" (word03) \
: [w10] "r" (word10), [w11] "r" (word11), [w12] "r" (word12), [w13] "r" (word13), [w20] "r" (word20), [w21] "r" (word21), [w22] "r" (word22), [w23] "r" (word23), [w30] "r" (word30), [w31] "r" (word31), [w32] "r" (word32), [w33] "r" (word33), [w40] "r" (word40), [w41] "r" (word41), [w42] "r" (word42), [w43] "r" (word43))



/*
#define F_SIMD(input0, input1, input2, input3, output0, output1, output2, output3, shift1, shift2, shift3) \
__asm(                                                                                                     \
"MOV v0.s[0], %w[i0]\n\t MOV v0.s[1], %w[i1]\n\t MOV v0.s[2], %w[i2]\n\t MOV v0.s[3], %w[i3]\n\t" \
"MOV v1.4s, v0.4s, ROR #" shift1 "\n\t EOR v1.4s, v1.4s, v0.4s, ROR #" shift2 "\n\t EOR v1.4s, v1.4s, v0.4s, ROR #" shift3 "\n\t" \
"MOV %w[o0], v1.s[0]\n\t MOV %w[o1], v1.s[1]\n\t" "MOV %w[o2], v1.s[2]\n\t MOV %w[o3], v1.s[3]" \
: [o0] "=r" (output0), [o1] "=r" (output1), [o2] "=r" (output2), [o3] "=r" (output3) \
: [i0] "r" (input0), [i1] "r" (input0), [i2] "r" (input0), [i3] "r" (input0))

#define F__SIMD(input0, input1, input2, input3, output0, output1, output2, output3, shift1, shift2, shift3) \
__asm(                                                                                                      \
"MOV v0.s[0], %w[i0]\n\t MOV v0.s[1], %w[i1]\n\t MOV v0.s[2], %w[i2]\n\t MOV v0.s[3], %w[i3]\n\t" \
"MOV v1.4s, v0.4s, ROR #" shift1 "\n\t EOR v1.4s, v1.4s, v0.4s, ROR #" shift2 "\n\t EOR v1.4s, v1.4s, v0.4s, LSR #" shift3 "\n\t" \
"MOV %w[o0], v1.s[0]\n\t MOV %w[o1], v1.s[1]\n\t" "MOV %w[o2], v1.s[2]\n\t MOV %w[o3], v1.s[3]" \
: [o0] "=r" (output0), [o1] "=r" (output1), [o2] "=r" (output2), [o3] "=r" (output3) \
: [i0] "r" (input0), [i1] "r" (input0), [i2] "r" (input0), [i3] "r" (input0))

#define F1_SIMD(input0, input1, input2, input3, output0, output1, output2, output3) F_SIMD(input0, input1, input2, input3, output0, output1, output2, output3, "2", "13", "22")
#define F2_SIMD(input0, input1, input2, input3, output0, output1, output2, output3) F_SIMD(input0, input1, input2, input3, output0, output1, output2, output3, "6", "11", "25")
#define F3_SIMD(input0, input1, input2, input3, output0, output1, output2, output3) F__SIMD(input0, input1, input2, input3, output0, output1, output2, output3, "7", "18", "3")
#define F4_SIMD(input0, input1, input2, input3, output0, output1, output2, output3) F__SIMD(input0, input1, input2, input3, output0, output1, output2, output3, "17", "19", "10")

#define CH_SIMD(input00, input01, input02, input03, input10, input11, input12, input13, input20, input21, input22, input23, output0, output1, output2, output3) \
__asm( \
"MOV v0.s[0], %w[i00]\n\t MOV v0.s[1], %w[i01]\n\t MOV v0.s[2], %w[i02]\n\t MOV v0.s[3], %w[i03]\n\t" \
"MOV v1.s[0], %w[i10]\n\t MOV v1.s[1], %w[i11]\n\t MOV v1.s[2], %w[i12]\n\t MOV v1.s[3], %w[i13]\n\t" \
"MOV v2.s[0], %w[i20]\n\t MOV v2.s[1], %w[i21]\n\t MOV v2.s[2], %w[i22]\n\t MOV v2.s[3], %w[i23]\n\t" \
"AND v3.4s, v0.4s, v1.4s\n\t BIC v4.4s, v2.4s, v0.4s\n\t EOR v3.4s, v3.4s, v4.4s\n\t" \
"MOV %w[o0], v4.s[0]\n\t MOV %w[o1], v4.s[1]\n\t" "MOV %w[o2], v4.s[2]\n\t MOV %w[o3], v4.s[3]" \
: [o0] "=r" (output0), [o1] "=r" (output1), [o2] "=r" (output2), [o3] "=r" (output3) \
: [i00] "r" (input00), [i01] "r" (input01), [i02] "r" (input02), [i03] "r" (input03), [i10] "r" (input10), [i11] "r" (input11), [i12] "r" (input12), [i13] "r" (input13), [i20] "r" (input20), [i21] "r" (input21), [i22] "r" (input22), [i23] "r" (input23));

#define MA_SIMD(input00, input01, input02, input03, input10, input11, input12, input13, input20, input21, input22, input23, output0, output1, output2, output3) \
__asm( \
"MOV v0.s[0], %w[i00]\n\t MOV v0.s[1], %w[i01]\n\t MOV v0.s[2], %w[i02]\n\t MOV v0.s[3], %w[i03]\n\t" \
"MOV v1.s[0], %w[i10]\n\t MOV v1.s[1], %w[i11]\n\t MOV v1.s[2], %w[i12]\n\t MOV v1.s[3], %w[i13]\n\t" \
"MOV v2.s[0], %w[i20]\n\t MOV v2.s[1], %w[i21]\n\t MOV v2.s[2], %w[i22]\n\t MOV v2.s[3], %w[i23]\n\t" \
"AND v3.4s, v0.4s, v1.4s\n\t AND v4.4s, v0.4s, v2.4s\n\t EOR v3.4s, v3.4s, v4.4s\n\t AND v4.4s, v1.4s, v2.4s\n\t EOR v3.4s, v3.4s, v4.4s" \
"MOV %w[o0], v4.s[0]\n\t MOV %w[o1], v4.s[1]\n\t" "MOV %w[o2], v4.s[2]\n\t MOV %w[o3], v4.s[3]" \
: [o0] "=r" (output0), [o1] "=r" (output1), [o2] "=r" (output2), [o3] "=r" (output3) \
: [i00] "r" (input00), [i01] "r" (input01), [i02] "r" (input02), [i03] "r" (input03), [i10] "r" (input10), [i11] "r" (input11), [i12] "r" (input12), [i13] "r" (input13), [i20] "r" (input20), [i21] "r" (input21), [i22] "r" (input22), [i23] "r" (input23));

#define INPUT_SIMD(input0, input1, input2, input3, i, temp0, temp1, temp2, temp3) \
F3_SIMD(input0[i - 15], input1[i - 15], input2[i - 15], input3[i - 15], input0[i], input1[i], input2[i], input3[i]); \
F4_SIMD(input0[i - 2], input1[i - 2], input2[i - 2], input3[i - 2], temp0, temp1, temp2, temp3); \
ADD4_SIMD(input0[i], input1[i], input2[i], input3[i], temp0, temp1, temp2, temp3, input0[i - 7], input1[i - 7], input2[i - 7], input3[i - 7], input0[i - 16], input1[i - 16], input2[i - 16], input3[i - 16]);

#define ROUND_SIMD(words0, words1, words2, words3, input0, input1, input2, input3, i, temp10, temp11, temp12, temp13, temp20, temp21, temp22, temp23, temp0, temp1, temp2, temp3) \
F2_SIMD(words0[4], words1[4], words2[4], words3[4], temp10, temp11, temp12, temp13); \
CH_SIMD(words0[4], words1[4], words2[4], words3[4], words0[5], words1[5], words2[5], words3[5], words0[6], words1[6], words2[6], words3[6], temp0, temp1, temp2, temp3); \
ADD5_SIMD(temp10, temp11, temp12, temp13, words0[7], words1[7], words2[7], words3[7], temp0, temp1, temp2, temp3, K[i], K[i], K[i], K[i], input0[i], input1[i], input2[i], input3[i]); \
F1_SIMD(words0[0], words0[1], words0[2], words0[3], temp20, temp21, temp22, temp23); \
MA_SIMD(words0[0], words1[0], words2[0], words3[0], words0[1], words1[1], words2[1], words3[1], words0[2], words1[2], words1[2], words1[2], temp0, temp1, temp2, temp3); \
ADD2_SIMD(temp20, temp21, temp22, temp23, temp0, temp1, temp2, temp3); \
words0[7] = words0[6]; words0[6] = words0[5]; words0[5] = words0[4]; words0[4] = words0[3] + temp1; words0[3] = words0[2]; words0[2] = words0[1]; words0[1] = words0[0]; words0[0] = temp1 + temp2; \
words1[7] = words1[6]; words1[6] = words1[5]; words1[5] = words1[4]; words1[4] = words1[3] + temp1; words1[3] = words1[2]; words1[2] = words1[1]; words1[1] = words1[0]; words1[0] = temp1 + temp2; \
words2[7] = words2[6]; words2[6] = words2[5]; words2[5] = words2[4]; words2[4] = words2[3] + temp1; words2[3] = words2[2]; words2[2] = words2[1]; words2[1] = words2[0]; words2[0] = temp1 + temp2; \
words3[7] = words3[6]; words3[6] = words3[5]; words3[5] = words3[4]; words3[4] = words3[3] + temp1; words3[3] = words3[2]; words3[2] = words3[1]; words3[1] = words3[0]; words3[0] = temp1 + temp2;

extern const unsigned K[64];
#ifdef COUNT_TEST
extern unsigned long long sha256Counter;
#endif
*/
void sha256(unsigned* input, unsigned* output);

void sha256(unsigned* input0, unsigned* input1, unsigned* input2, unsigned* input3, unsigned* output0, unsigned* output1, unsigned* output2, unsigned* output3);
