#define ROR(x, n) (x >> n | x << (32 - n))
#define INPUT(i) \
input[i] = (ROR(input[i - 2], 17) ^ ROR(input[i - 2], 19) ^ input[i - 2] >> 10) + input[i - 7] + (ROR(input[i - 15], 7) ^ ROR(input[i - 15], 18) ^ input[i - 15] >> 3) + input[i - 16];
#define INPUT_16() \
input[16] = (ROR(input[1], 7) ^ ROR(input[1], 18) ^ input[1] >> 3) + input[0];
#define INPUT_17() \
input[17] = 0x00A50000 + (ROR(input[2], 7) ^ ROR(input[2], 18) ^ input[2] >> 3) + input[1];
#define INPUT_18_21(i) \
input[i] = (ROR(input[i - 2], 17) ^ ROR(input[i - 2], 19) ^ input[i - 2] >> 10) + (ROR(input[i - 15], 7) ^ ROR(input[i - 15], 18) ^ input[i - 15] >> 3) + input[i - 16];
#define INPUT_24() \
input[24] = (ROR(input[22], 17) ^ ROR(input[22], 19) ^ input[22] >> 10) + input[17] + input[8];
#define INPUT_25_29(i) \
input[i] = (ROR(input[i - 2], 17) ^ ROR(input[i - 2], 19) ^ input[i - 2] >> 10) + input[i - 7];
#define INPUT_30() \
input[30] = (ROR(input[28], 17) ^ ROR(input[28], 19) ^ input[28] >> 10) + input[23] + 0x10420023;
#define ROUND(i, K) \
temp1 = words[7] + (ROR(words[4], 6) ^ ROR(words[4], 11) ^ ROR(words[4], 25)) + (words[4] & words[5] ^ ~words[4] & words[6]) + K + input[i]; \
temp2 = (ROR(words[0], 2) ^ ROR(words[0], 13) ^ ROR(words[0], 22)) + (words[0] & words[1] ^ words[0] & words[2] ^ words[1] & words[2]); \
words[7] = words[6]; words[6] = words[5]; words[5] = words[4]; words[4] = words[3] +temp1; words[3] = words[2]; words[2] = words[1]; words[1] = words[0]; words[0] = temp1 + temp2;

void sha256(unsigned* input, unsigned* output);
