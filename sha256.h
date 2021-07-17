#define ADD2(state1, state2) __asm("ADD %w[s1], %w[s1], %w[s2]" : [s1] "+r" (state1) : [s2] "r" (state2))
#define ADD3(state1, state2, state3) __asm("ADD %w[s1], %w[s1], %w[s2]\n\t ADD %w[s1], %w[s1], %w[s3]" : [s1] "+r" (state1) : [s2] "r" (state2), [s3] "r" (state3))
#define ADD4(state1, state2, state3, state4) __asm("ADD %w[s1], %w[s1], %w[s2]\n\t ADD %w[s1], %w[s1], %w[s3]\n\t ADD %w[s1], %w[s1], %w[s4]" : [s1] "+r" (state1) : [s2] "r" (state2), [s3] "r" (state3), [s4] "r" (state4))
#define ADD5(state1, state2, state3, state4, state5) __asm("ADD %w[s1], %w[s1], %w[s2]\n\t ADD %w[s1], %w[s1], %w[s3]\n\t ADD %w[s1], %w[s1], %w[s4]\n\t ADD %w[s1], %w[s1], %w[s5]" : [s1] "+r" (state1) : [s2] "r" (state2), [s3] "r" (state3), [s4] "r" (state4), [s5] "r" (state5))
#define F(input, output, shift1, shift2, shift3) __asm("MOV %w[o], %w[i], ROR #" shift1 "\n\t EOR %w[o], %w[o], %w[i], ROR #" shift2 "\n\t EOR %w[o], %w[o], %w[i], ROR #" shift3 : [o] "+r" (output) :  [i] "r" (input))
#define F_(input, output, shift1, shift2, shift3) __asm("MOV %w[o], %w[i], ROR #" shift1 "\n\t EOR %w[o], %w[o], %w[i], ROR #" shift2 "\n\t EOR %w[o], %w[o], %w[i], LSR #" shift3 : [o] "+r" (output) :  [i] "r" (input))
#define F1(input, output) F(input, output, "2", "13", "22")
#define F2(input, output) F(input, output, "6", "11", "25")
#define F3(input, output) F_(input, output, "7", "18", "3")
#define F4(input, output) F_(input, output, "17", "19", "10")
#define CH(input1, input2, input3, output) { unsigned temp_; __asm("AND %w[o], %w[i1], %w[i2]\n\t BIC %w[t], %w[i3], %w[i1]\n\t EOR %w[o], %w[o], %w[t]" : [o] "+r" (output), [t] "+r" (temp_) : [i1] "r" (input1), [i2] "r" (input2), [i3] "r" (input3)); }
#define MA(input1, input2, input3, output) { unsigned temp_; __asm("AND %w[o], %w[i1], %w[i2]\n\t AND %w[t], %w[i1], %w[i3]\n\t EOR %w[o], %w[o], %w[t]\n\t AND %w[t], %w[i2], %w[i3]\n\t EOR %w[o], %w[o], %w[t]" : [o] "+r" (output), [t] "+r" (temp_) : [i1] "r" (input1), [i2] "r" (input2), [i3] "r" (input3)); }
#define INPUT(input, i, temp) F3(input[i - 15], input[i]); F4(input[i - 2], temp); ADD4(input[i], temp, input[i - 7], input[i - 16]);
#define ROUND(states, input, i, temp1, temp2, temp) F2(states[4], temp1); CH(states[4], states[5], states[6], temp); ADD5(temp1, states[7], temp, K[i], input[i]); F1(states[0], temp2); MA(states[0], states[1], states[2], temp); ADD2(temp2, temp); states[7] = states[6]; states[6] = states[5]; states[5] = states[4]; states[4] = states[3] + temp1; states[3] = states[2]; states[2] = states[1]; states[1] = states[0]; states[0] = temp1 + temp2;

extern const unsigned K[64];
#ifdef COUNT_TEST
extern unsigned long long sha256Counter;
#endif

void sha256(unsigned* input, unsigned* output);
