#define ADD(word1, word2) __asm("ADD %w[w1], %w[w1], %w[w2]" : [w1] "+&r" (word1) : [w2] "r" (word2))

#define INPUT_16(input) __asm( \
"MOV %w[o], %w[i15], ROR #7\n\t EOR %w[o], %w[o], %w[i15], ROR #18\n\t EOR %w[o], %w[o], %w[i15], LSR #3\n\t" \
"ADD %w[o], %w[o], %w[i16]" \
: [o] "+&r" (input[16]) \
: [i15] "r" (input[1]), [i16] "r" (input[0]))

#define INPUT_17(input) __asm( \
"MOV %w[o], %w[i15], ROR #7\n\t EOR %w[o], %w[o], %w[i15], ROR #18\n\t EOR %w[o], %w[o], %w[i15], LSR #3\n\t" \
"ADD %w[o], %w[o], %w[i16]\n\t ADD %w[o], %w[o], %w[f4]" \
: [o] "+&r" (input[17]) \
: [i15] "r" (input[2]), [i16] "r" (input[1]), [f4] "r" (0x00A50000))

#define INPUT_18_21(input, index) __asm( \
"MOV %w[o], %w[i15], ROR #7\n\t EOR %w[o], %w[o], %w[i15], ROR #18\n\t EOR %w[o], %w[o], %w[i15], LSR #3\n\t" \
"MOV w0, %w[i2], ROR #17\n\t EOR w0, w0, %w[i2], ROR #19\n\t EOR w0, w0, %w[i2], LSR #10\n\t" \
"ADD %w[o], %w[o], w0\n\t ADD %w[o], %w[o], %w[i16]" \
: [o] "+&r" (input[index]) \
: [i2] "r" (input[index - 2]),[i15] "r" (input[index - 15]),[i16] "r" (input[index - 16]) \
: "w0")

#define INPUT_24(input) __asm( \
"MOV %w[o], %w[i2], ROR #17\n\t EOR %w[o], %w[o], %w[i2], ROR #19\n\t EOR %w[o], %w[o], %w[i2], LSR #10\n\t" \
"ADD %w[o], %w[o], %w[i7]\n\t ADD %w[o], %w[o], %w[i16]" \
: [o] "+&r" (input[24]) \
: [i2] "r" (input[22]), [i7] "r" (input[17]), [i16] "r" (input[8]))

#define INPUT_25_29(input, index) __asm( \
"MOV %w[o], %w[i2], ROR #17\n\t EOR %w[o], %w[o], %w[i2], ROR #19\n\t EOR %w[o], %w[o], %w[i2], LSR #10\n\t" \
"ADD %w[o], %w[o], %w[i7]" \
: [o] "+&r" (input[index]) \
: [i2] "r" (input[index - 2]), [i7] "r" (input[index - 7]))

#define INPUT_30(input) __asm( \
"MOV %w[o], %w[i2], ROR #17\n\t EOR %w[o], %w[o], %w[i2], ROR #19\n\t EOR %w[o], %w[o], %w[i2], LSR #10\n\t" \
"ADD %w[o], %w[o], %w[i7]\n\t ADD %w[o], %w[o], %w[f3]" \
: [o] "+&r" (input[30]) \
: [i2] "r" (input[28]), [i7] "r" (input[23]), [f3] "r" (0x10420023))

#define INPUT(input, index) __asm( \
"MOV %w[o], %w[i15], ROR #7\n\t EOR %w[o], %w[o], %w[i15], ROR #18\n\t EOR %w[o], %w[o], %w[i15], LSR #3\n\t" \
"MOV w0, %w[i2], ROR #17\n\t EOR w0, w0, %w[i2], ROR #19\n\t EOR w0, w0, %w[i2], LSR #10\n\t" \
"ADD %w[o], %w[o], w0\n\t ADD %w[o], %w[o], %w[i7]\n\t ADD %w[o], %w[o], %w[i16]" \
: [o] "+&r" (input[index]) \
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

extern const unsigned K[64];

void sha256(unsigned* input, unsigned* output);
