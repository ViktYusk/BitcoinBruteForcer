#pragma once

#define COMPARE_BLOCKS(block1, block2) if (block1 > block2) return 1; if (block1 < block2) return -1;
#define MULTIPLY_BLOCK_BY_BLOCK(i, j) result += (__uint128_t)blocks[i] * key.blocks[j]; table[i][j] = result; result >>= 64;
#define MULTIPLY_BLOCK_BY_BLOCK_LAST(i) result += (__uint128_t)blocks[i] * key.blocks[3]; table[i][3] = result; table[i][4] = result >> 64;
#define MULTIPLY_BLOCK_BY_BLOCK_REDUCED(i, j) table[i][j] = result + blocks[i] * key.blocks[j];
#define MULTIPLY_BLOCK_BY_R2(j) result += (__uint128_t)R2.blocks[0] * blocks[j]; table[j] = result; result >>= 64;
#define MULTIPLY_BLOCKS_BY_BLOCK(i) result = 0; MULTIPLY_BLOCK_BY_BLOCK(i, 0) MULTIPLY_BLOCK_BY_BLOCK(i, 1) MULTIPLY_BLOCK_BY_BLOCK(i, 2) MULTIPLY_BLOCK_BY_BLOCK_LAST(i)
#define ADD_MULTIPLIED_BLOCKS(i, sum) result >>= 64; result += (__uint128_t)sum; product.blocks[i] = result;
#define ADD_MULTIPLIED_BLOCKS_SELF(i, sum) result >>= 64; result += (__uint128_t)sum; blocks[i] = result;
#define ADD_MULTIPLIED_BLOCKS_REDUCED(i, sum) product.blocks[i] = (unsigned long long)(result >> 64) + sum;

struct Key
{
	static const int GROUP_BITS = 12;
	static const int GROUP_SIZE = 1 << GROUP_BITS;
	static const unsigned long long B = 0x0000000100000000;
	
	static const Key ZERO;
	static const Key ONE;
	static const Key THREE;
	static const Key R;
	static const Key R2;
    //static const Key P2;
	static const Key P_PRIME;
	static const Key P;

	static void gcd(Key a, Key b, Key& x, Key& y);
	static void invertGroup(Key* keys);

	unsigned long long blocks[8];
	
	Key();
	Key(unsigned long long block0, unsigned long long block1, unsigned long long block2, unsigned long long block3);
	Key(unsigned long long block0, unsigned long long block1, unsigned long long block2, unsigned long long block3, unsigned long long block4, unsigned long long block5, unsigned long long block6, unsigned long long block7);

	bool operator==(const Key& key) const;
	void operator+=(const Key& key);
	void operator-=(const Key& key);
	void operator*=(const Key& key);

	int compare(const Key& key);
	int compareExtended(const Key& key);

    bool isNotZero();
	bool increment();
	bool add(const Key& key);
	bool addExtended(const Key& key);
	bool subtract(const Key& key);
	unsigned long long differenceParity(const Key& subtrahend);
    //void rightShift();
    void multiply(const Key& key, Key& product);
    void multiplyLow(const Key& key, Key& product);
	void multiplyByR2();
	void reduce();
	void divide(const Key& divisor, Key& quotient);
	void invert();
};
