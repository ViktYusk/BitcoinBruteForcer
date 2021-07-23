#include "ripemd160.h"

void ripemd160(unsigned* input, unsigned* output)
{
    output[0] = 0x67452301;
    output[1] = 0xEFCDAB89;
    output[2] = 0x98BADCFE;
    output[3] = 0x10325476;
    output[4] = 0xC3D2E1F0;

    unsigned chunk[16];
    REVERSE(input[0], &chunk[0])
    REVERSE(input[1], &chunk[1])
    REVERSE(input[2], &chunk[2])
    REVERSE(input[3], &chunk[3])
    REVERSE(input[4], &chunk[4])
    REVERSE(input[5], &chunk[5])
    REVERSE(input[6], &chunk[6])
    REVERSE(input[7], &chunk[7])
    chunk[8] = 128;
    chunk[9] = 0;
    chunk[10] = 0;
    chunk[11] = 0;
    chunk[12] = 0;
    chunk[13] = 0;
    chunk[14] = 256;
    chunk[15] = 0;
    compress(output, chunk);

	//memcpy(hash, digest, 20);
	/*
    for (i = 0; i < 5; ++i) {
		*(hash++) = digest[i];
		*(hash++) = digest[i] >> 8;
		*(hash++) = digest[i] >> 16;
		*(hash++) = digest[i] >> 24;
	}
	 */
}
