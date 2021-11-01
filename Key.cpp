#include "Key.h"

const Key Key::ZERO    = Key(0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000); // 0 NOLINT(cert-err58-cpp)
const Key Key::ONE     = Key(0x0000000000000001, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000); // 1 NOLINT(cert-err58-cpp)
const Key Key::THREE   = Key(0x0000000000000003, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000); // 3 NOLINT(cert-err58-cpp)
const Key Key::R       = Key(0x00000001000003D1, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000); // 4294968273 NOLINT(cert-err58-cpp)
const Key Key::R2      = Key(0x000007A2000E90A1, 0x0000000000000001, 0x0000000000000000, 0x0000000000000000); // 18446752466076602529 NOLINT(cert-err58-cpp)
const Key Key::P2      = Key(0xFFFFFFFF7FFFFE18, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0x7FFFFFFFFFFFFFFF); // 57896044618658097711785492504343953926634992332820282019728792003954417335832 NOLINT(cert-err58-cpp)
const Key Key::P_PRIME = Key(0xD838091DD2253531, 0xBCB223FEDC24A059, 0x9C46C2C295F2B761, 0xC9BD190515538399); // 91248989341183975618893650062416139444822672217621753343178995607987479196977 NOLINT(cert-err58-cpp)
const Key Key::P       = Key(0xFFFFFFFEFFFFFC2F, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF); // 115792089237316195423570985008687907853269984665640564039457584007908834671663 NOLINT(cert-err58-cpp)

/*
void Key::gcd(Key a, Key b, Key& x, Key&y)
{
    if (a.isNotZero())
    {
        Key x_;
        Key y_;
        Key q;
        b.divide(a, q);
        gcd(b, a, x_, y_);
        q *= x_;
        x = y_;
        x -= q;
        y = x_;
    }
    else
    {
        x = ZERO;
        y = ONE;
    }
}
*/

void Key::invertGroup(Key* keys)
{
	Key products[GROUP_SIZE / 2 + 1];
	products[0] = keys[0];
	for (int k = 1; k < GROUP_SIZE / 2 + 1; k++)
	{
		products[k] = products[k - 1];
		products[k] *= keys[k];
	}
	products[GROUP_SIZE / 2].invert();
	for (int k = GROUP_SIZE / 2; k >= 1; k--)
	{
		Key temp = keys[k];
		keys[k] = products[k];
		keys[k] *= products[k - 1];
		products[k - 1] = products[k];
		products[k - 1] *= temp;
	}
	keys[0] = products[0];
}

Key::Key()
= default;

Key::Key(unsigned long long block0, unsigned long long block1, unsigned long long block2, unsigned long long block3)
{
	blocks[0] = block0;
	blocks[1] = block1;
	blocks[2] = block2;
	blocks[3] = block3;
}

Key::Key(unsigned long long block0, unsigned long long block1, unsigned long long block2, unsigned long long block3, unsigned long long block4, unsigned long long block5, unsigned long long block6, unsigned long long block7)
{
	blocks[0] = block0;
	blocks[1] = block1;
	blocks[2] = block2;
	blocks[3] = block3;
	blocks[4] = block4;
	blocks[5] = block5;
	blocks[6] = block6;
	blocks[7] = block7;
}

bool Key::operator==(const Key& key) const
{
	return blocks[0] == key.blocks[0] && blocks[1] == key.blocks[1] && blocks[2] == key.blocks[2] && blocks[3] == key.blocks[3];
}

void Key::operator+=(const Key& key)
{
	if (add(key))
		add(R);
	else if (compare(P) >= 0)
		subtract(P);
}

void Key::operator-=(const Key& key)
{
	if (subtract(key))
		add(P);
}

void Key::operator*=(const Key& key)
{
    multiplyByR2();
	reduce();
	multiply(key, *this);
	reduce();
}

int Key::compare(const Key& key)
{
	COMPARE_BLOCKS(blocks[3], key.blocks[3])
	COMPARE_BLOCKS(blocks[2], key.blocks[2])
	COMPARE_BLOCKS(blocks[1], key.blocks[1])
	COMPARE_BLOCKS(blocks[0], key.blocks[0])
	return 0;
}

int Key::compareExtended(const Key& key)
{
	COMPARE_BLOCKS(blocks[7], key.blocks[7])
	COMPARE_BLOCKS(blocks[6], key.blocks[6])
	COMPARE_BLOCKS(blocks[5], key.blocks[5])
	COMPARE_BLOCKS(blocks[4], key.blocks[4])
	COMPARE_BLOCKS(blocks[3], key.blocks[3])
	COMPARE_BLOCKS(blocks[2], key.blocks[2])
	COMPARE_BLOCKS(blocks[1], key.blocks[1])
	COMPARE_BLOCKS(blocks[0], key.blocks[0])
	return 0;
}

bool Key::isNotZero() {
    return blocks[0] | blocks[1] | blocks[2] | blocks[3];
}

bool Key::increment()
{
    if (++blocks[0])
        return false;
    if (++blocks[1])
        return false;
    if (++blocks[2])
        return false;
    if (++blocks[3])
        return false;
    return true;
}

bool Key::add(const Key& key)
{
    unsigned long long carry;
    __asm(
        "ADDS %[a0], %[a0], %[b0]\n\t"
        "ADCS %[a1], %[a1], %[b1]\n\t"
        "ADCS %[a2], %[a2], %[b2]\n\t"
        "ADCS %[a3], %[a3], %[b3]\n\t"
        "ADC %[c], xzr, xzr"
        : [a0] "+r" (blocks[0]), [a1] "+r" (blocks[1]), [a2] "+r" (blocks[2]), [a3] "+r" (blocks[3]), [c] "=r" (carry)
        : [b0] "r" (key.blocks[0]), [b1] "r" (key.blocks[1]), [b2] "r" (key.blocks[2]), [b3] "r" (key.blocks[3])
    );
	return carry;
}

bool Key::addExtended(const Key& key)
{
    unsigned long long carry;
    __asm(
        "ADDS %[a0], %[a0], %[b0]\n\t"
        "ADCS %[a1], %[a1], %[b1]\n\t"
        "ADCS %[a2], %[a2], %[b2]\n\t"
        "ADCS %[a3], %[a3], %[b3]\n\t"
        "ADCS %[a4], %[a4], %[b4]\n\t"
        "ADCS %[a5], %[a5], %[b5]\n\t"
        "ADCS %[a6], %[a6], %[b6]\n\t"
        "ADCS %[a7], %[a7], %[b7]\n\t"
        "ADC %[c], xzr, xzr"
        : [a0] "+r" (blocks[0]), [a1] "+r" (blocks[1]), [a2] "+r" (blocks[2]), [a3] "+r" (blocks[3]), [a4] "+r" (blocks[4]), [a5] "+r" (blocks[5]), [a6] "+r" (blocks[6]), [a7] "+r" (blocks[7]), [c] "=r" (carry)
        : [b0] "r" (key.blocks[0]), [b1] "r" (key.blocks[1]), [b2] "r" (key.blocks[2]), [b3] "r" (key.blocks[3]), [b4] "r" (key.blocks[4]), [b5] "r" (key.blocks[5]), [b6] "r" (key.blocks[6]), [b7] "r" (key.blocks[7])
    );
	return carry;
}

bool Key::subtract(const Key& key)
{
    unsigned long long carry;
    __asm(
        "SUBS %[a0], %[a0], %[b0]\n\t"
        "SBCS %[a1], %[a1], %[b1]\n\t"
        "SBCS %[a2], %[a2], %[b2]\n\t"
        "SBCS %[a3], %[a3], %[b3]\n\t"
        "SBC %[c], xzr, xzr"
        : [a0] "+r" (blocks[0]), [a1] "+r" (blocks[1]), [a2] "+r" (blocks[2]), [a3] "+r" (blocks[3]), [c] "=r" (carry)
        : [b0] "r" (key.blocks[0]), [b1] "r" (key.blocks[1]), [b2] "r" (key.blocks[2]), [b3] "r" (key.blocks[3])
    );
	return carry;
}

unsigned long long Key::differenceParity(const Key& subtrahend)
{
    unsigned long long parity = blocks[0] % 2 ^ subtrahend.blocks[0] % 2;
    return compare(subtrahend) == 1 ? parity : 1 - parity;
}

void Key::rightShift()
{
    __asm("MOV %[b0], %[b0], LSR #1\n\t ADD %[b0], %[b0], %[b1], LSL #63\n\t"
          "MOV %[b1], %[b1], LSR #1\n\t ADD %[b1], %[b1], %[b2], LSL #63\n\t"
          "MOV %[b2], %[b2], LSR #1\n\t ADD %[b2], %[b2], %[b3], LSL #63\n\t"
          "MOV %[b3], %[b3], LSR #1"
    : [b0] "+r" (blocks[0]), [b1] "+r" (blocks[1]), [b2] "+r" (blocks[2]), [b3] "+r" (blocks[3]));
}

void Key::multiply(const Key& key, Key& product)
{
    unsigned long long table[4][5];
    __uint128_t result;
    MULTIPLY_BLOCKS_BY_BLOCK(0)
    MULTIPLY_BLOCKS_BY_BLOCK(1)
    MULTIPLY_BLOCKS_BY_BLOCK(2)
    MULTIPLY_BLOCKS_BY_BLOCK(3)
    product.blocks[0] = table[0][0];
    result = (__uint128_t)table[0][1] + table[1][0];
    product.blocks[1] = result;
    ADD_MULTIPLIED_BLOCKS(2, table[0][2] + table[1][1] + table[2][0])
    ADD_MULTIPLIED_BLOCKS(3, table[0][3] + table[1][2] + table[2][1] + table[3][0])
    ADD_MULTIPLIED_BLOCKS(4, table[0][4] + table[1][3] + table[2][2] + table[3][1])
    ADD_MULTIPLIED_BLOCKS(5, table[1][4] + table[2][3] + table[3][2])
    ADD_MULTIPLIED_BLOCKS(6, table[2][4] + table[3][3])
    ADD_MULTIPLIED_BLOCKS_REDUCED(7, table[3][4])
}

void Key::multiplyLow(const Key& key, Key& product)
{
    unsigned long long table[4][4];
    __uint128_t result;
    result = 0; MULTIPLY_BLOCK_BY_BLOCK(0, 0) MULTIPLY_BLOCK_BY_BLOCK(0, 1) MULTIPLY_BLOCK_BY_BLOCK(0, 2) MULTIPLY_BLOCK_BY_BLOCK_REDUCED(0, 3)
    result = 0; MULTIPLY_BLOCK_BY_BLOCK(1, 0) MULTIPLY_BLOCK_BY_BLOCK(1, 1) MULTIPLY_BLOCK_BY_BLOCK_REDUCED(1, 2)
    result = 0; MULTIPLY_BLOCK_BY_BLOCK(2, 0) MULTIPLY_BLOCK_BY_BLOCK_REDUCED(2, 1)
    result = 0; MULTIPLY_BLOCK_BY_BLOCK_REDUCED(3, 0)
    product.blocks[0] = table[0][0];
    result = (__uint128_t)table[0][1] + table[1][0];
    product.blocks[1] = result;
    ADD_MULTIPLIED_BLOCKS(2, table[0][2] + table[1][1] + table[2][0])
    ADD_MULTIPLIED_BLOCKS_REDUCED(3, table[0][3] + table[1][2] + table[2][1] + table[3][0])
}

void Key::multiplyByR2()
{
    unsigned long long table[5];
    __uint128_t result = 0;
    MULTIPLY_BLOCK_BY_R2(0)
    MULTIPLY_BLOCK_BY_R2(1)
    MULTIPLY_BLOCK_BY_R2(2)
    result += (__uint128_t)R2.blocks[0] * blocks[3]; table[3] = result; table[4] = result >> 64;
    blocks[4] = blocks[0];
    blocks[5] = blocks[1];
    blocks[6] = blocks[2];
    blocks[7] = blocks[3];
    blocks[0] = table[0];
    result = (__uint128_t)table[1] + blocks[4];
    blocks[1] = result;
    ADD_MULTIPLIED_BLOCKS_SELF(2, table[2] + blocks[5])
    ADD_MULTIPLIED_BLOCKS_SELF(3, table[3] + blocks[6])
    ADD_MULTIPLIED_BLOCKS_SELF(4, table[4] + blocks[7])
    blocks[5] = result >> 64;
    blocks[6] = blocks[7] = 0;
}

void Key::reduce()
{
    Key temp1, temp2;
    multiplyLow(P_PRIME, temp1);
    __uint128_t result = (__uint128_t)R.blocks[0] * temp1.blocks[0]; result >>= 64;
    result += (__uint128_t)R.blocks[0] * temp1.blocks[1]; result >>= 64;
    result += (__uint128_t)R.blocks[0] * temp1.blocks[2]; result >>= 64;
    result += (__uint128_t)R.blocks[0] * temp1.blocks[3]; temp2.blocks[0] = result >> 64;
    temp2.blocks[1] = temp2.blocks[2] = temp2.blocks[3] = 0;
    if (temp1.isNotZero())
        temp2.increment();
    temp1.subtract(temp2);
    bool notZero = isNotZero();
	blocks[0] = blocks[4];
	blocks[1] = blocks[5];
	blocks[2] = blocks[6];
	blocks[3] = blocks[7];
	bool overflow = add(temp1);
	if (notZero)
	    overflow |= increment();
	if (overflow)
		add(R);
	else if (compare(P) >= 0)
		subtract(P);
}

/*
void Key::divide(const Key& divisor, Key& quotient)
{
    auto* u = (unsigned*)blocks;
    auto* v = (unsigned*)divisor.blocks;
    int m = 8;
    while (!u[m - 1])
        m--;
    int n = 8;
    while (!v[n - 1])
        n--;
    if (m < n)
    {
        quotient = ZERO;
        return;
    }
    unsigned s = 32;
    unsigned x = v[n - 1], y;
    y = x >> 16; if (y) { s -= 16; x = y; }
    y = x >> 8; if (y) { s -= 8; x = y; }
    y = x >> 4; if (y) { s -= 4; x = y; }
    y = x >> 2; if (y) { s -= 2; x = y; }
    y = x >> 1; if (y) { s -= 1; x = y; }
    s -= x;
    unsigned un[m + 1];
    un[m] = (unsigned long long)u[m - 1] >> (32 - s);
    for (int i = m - 1; i > 0; i--)
        un[i] = u[i] << s | (unsigned long long)u[i - 1] >> (32 - s);
    un[0] = u[0] << s;
    unsigned vn[n];
    for (int i = n - 1; i > 0; i--)
        vn[i] = v[i] << s | (unsigned long long)v[i - 1] >> (32 - s);
    vn[0] = v[0] << s;
    quotient = ZERO;
    auto* q = (unsigned*)quotient.blocks;
    for (int j = m - n; j >= 0; j--)
    {
        unsigned long long _ = un[n + j] * B + un[n + j - 1];
        unsigned long long q_ = _ / vn[n - 1];
        unsigned long long r_ = _ % vn[n - 1];
        label:
        if (q_ >= B || (unsigned)q_ * (unsigned long long)vn[n - 2] > B * r_ + un[n + j - 2])
        {
            q_--;
            r_ += vn[n - 1];
            if (r_ < B)
                goto label;
        }
        long long k = 0, t;
        for (int i = 0; i < n; i++)
        {
            unsigned long long p = (unsigned)q_ * (unsigned long long)vn[i];
            t = un[i + j] - k - (p & 0xFFFFFFFF);
            un[i + j] = t;
            k = (p >> 32) - (t >> 32);
        }
        t = un[n + j] - k;
        un[n + j] = t;
        q[j] = q_;
        if (t < 0)
        {
            q[j]--;
            k = 0;
            for (int i = 0; i < n; i++)
            {
                t = (unsigned long long)un[i + j] + vn[i] + k;
                un[i + j] = t;
                k = t >> 32;
            }
            un[n + j] += k;
        }
    }
    unsigned r[8] = { 0 };
    for (int i = 0; i < n - 1; i++)
        r[i] = un[i] >> s | (unsigned long long)un[i + 1] << (32 - s);
    r[n - 1] = un[n - 1] >> s;
    blocks[0] = r[0] + ((unsigned long long)r[1] << 32);
    blocks[1] = r[2] + ((unsigned long long)r[3] << 32);
    blocks[2] = r[4] + ((unsigned long long)r[5] << 32);
    blocks[3] = r[6] + ((unsigned long long)r[7] << 32);
}
*/

void Key::invert()
{
    /*
    Key x;
	Key y;
	gcd(*this, P, x, y);
	*this = x;
    */
    ///*
    Key u = P;
    Key v = *this;
    *this = ZERO;
    Key s = ONE;
    while (v.isNotZero())
    {
        if (!(u.blocks[0] % 2))
        {
            u.rightShift();
            bool odd = blocks[0] % 2;
            rightShift();
            if (odd)
                add(P2);
        }
        else if (!(v.blocks[0] % 2))
        {
            v.rightShift();
            bool odd = s.blocks[0] % 2;
            s.rightShift();
            if (odd)
                s.add(P2);
        }
        else if (u.compare(v) > 0)
        {
            u.subtract(v);
            *this -= s;
        }
        else
        {
            v.subtract(u);
            s -= *this;
        }
    }
    //*/
}
