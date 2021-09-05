#define ROL(x, n) (x << n | x >> (32 - n))
#define F(x, y, z) (x ^ y ^ z)
#define G(x, y, z) (x & y | ~x & z)
#define H(x, y, z) ((x | ~y) ^ z)
#define IQ(x, y, z) (x & z | y & ~z)
#define J(x, y, z) (x ^ (y | ~z))
#define FF(a, b, c, d, e, x, s) \
a += F(b, c, d) + x; \
a = ROL(a, s) + e; \
c = ROL(c, 10);
#define GG(a, b, c, d, e, x, s) \
a += G(b, c, d) + x + 0x5A827999; \
a = ROL(a, s) + e; \
c = ROL(c, 10);
#define HH(a, b, c, d, e, x, s) \
a += H(b, c, d) + x + 0x6ED9EBA1; \
a = ROL(a, s) + e; \
c = ROL(c, 10);
#define II(a, b, c, d, e, x, s) \
a += IQ(b, c, d) + x + 0x8F1BBCDC; \
a = ROL(a, s) + e; \
c = ROL(c, 10);
#define JJ(a, b, c, d, e, x, s) \
a += J(b, c, d) + x + 0xA953FD4E; \
a = ROL(a, s) + e; \
c = ROL(c, 10);
#define FFF(a, b, c, d, e, x, s) \
a += F(b, c, d) + x; \
a = ROL(a, s) + e; \
c = ROL(c, 10);
#define GGG(a, b, c, d, e, x, s) \
a += G(b, c, d) + x + 0x7A6D76E9; \
a = ROL(a, s) + e; \
c = ROL(c, 10);
#define HHH(a, b, c, d, e, x, s) \
a += H(b, c, d) + x + 0x6D703EF3; \
a = ROL(a, s) + e; \
c = ROL(c, 10);
#define III(a, b, c, d, e, x, s) \
a += IQ(b, c, d) + x + 0x5C4DD124; \
a = ROL(a, s) + e; \
c = ROL(c, 10);
#define JJJ(a, b, c, d, e, x, s) \
a += J(b, c, d) + x + 0x50A28BE6; \
a = ROL(a, s) + e; \
c = ROL(c, 10);
#define REVERSE(input, output) *((unsigned char*)output + 3) = input; *((unsigned char*)output + 2) = input >> 8; *((unsigned char*)output + 1) = input >> 16; *(unsigned char*)output = input >> 24;

void ripemd160(unsigned* input, unsigned* output);
