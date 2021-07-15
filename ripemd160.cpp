#include "ripemd160.h"

#ifdef COUNT_TEST
unsigned long long Ripemd160Counter::counter = 0;
#endif

void ripemd160(const uint8_t* msg, uint8_t* hash)
{
#ifdef COUNT_TEST
    Ripemd160Counter::counter++;
#endif
	uint32_t i;
	int j;
	uint32_t digest[5] = { 0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476, 0xc3d2e1f0UL };

	for (i = 0; i < (MSG_LEN >> 6); ++i) {
		uint32_t chunk[16];

		for (j = 0; j < 16; ++j) {
			chunk[j] = (uint32_t)(*(msg++));
			chunk[j] |= (uint32_t)(*(msg++)) << 8;
			chunk[j] |= (uint32_t)(*(msg++)) << 16;
			chunk[j] |= (uint32_t)(*(msg++)) << 24;
		}

		compress(digest, chunk);
	}

	// Last chunk
	{
		uint32_t chunk[16] = { 0 };

		for (i = 0; i < (MSG_LEN & 63); ++i) {
			chunk[i >> 2] ^= (uint32_t)*msg++ << ((i & 3) << 3);
		}

		chunk[(MSG_LEN >> 2) & 15] ^= (uint32_t)1 << (8 * (MSG_LEN & 3) + 7);

		if ((MSG_LEN & 63) > 55) {
			compress(digest, chunk);
			memset(chunk, 0, 64);
		}

		chunk[14] = MSG_LEN << 3;
		chunk[15] = (MSG_LEN >> 29);
		compress(digest, chunk);
	}

	for (i = 0; i < 5; ++i) {
		*(hash++) = digest[i];
		*(hash++) = digest[i] >> 8;
		*(hash++) = digest[i] >> 16;
		*(hash++) = digest[i] >> 24;
	}
}
