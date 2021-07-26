#pragma once

#define COMPARE_BLOCKS(block1, block2) if (block1 > block2) return 1; if (block1 < block2) return -1;
#define ADD_BLOCKS(command, block1, block2) __asm(command " %[b1], %[b1], %[b2]" : [b1] "+r" (block1) : [b2] "r" (block2))
#define SUBTRACT_BLOCKS(command, block1, block2) __asm(command " %[b1], %[b1], %[b2]" : [b1] "+r" (block1) : [b2] "r" (block2))

struct Key
{
	static const int GROUP_BITS = 12;
	static const int GROUP_SIZE = 1 << GROUP_BITS;
	
	static const Key ZERO;
	static const Key ONE;
	static const Key THREE;
	static const Key R;
	static const Key R2;
	static const Key P_PRIME;
	static const Key P;

	static void gcd(Key a, Key b, Key& x, Key& y);
	static void invertGroup(Key* keys);

	unsigned long long blocks[8]{};
	
	Key();
	Key(unsigned long long block0, unsigned long long block1, unsigned long long block2, unsigned long long block3);
	Key(unsigned long long block0, unsigned long long block1, unsigned long long block2, unsigned long long block3, unsigned long long block4, unsigned long long block5, unsigned long long block6, unsigned long long block7);
	
	bool operator==(const Key& key);
    void operator<<=(int shift);
	//void operator++();
	void operator+=(const Key& key);
	void operator-=(const Key& key);
	void operator*=(const Key& key);

	int compare(const Key& key);
	int compareExtended(const Key& key);
	
	bool add(const Key& key);
	bool addExtended(const Key& key);
	bool subtract(const Key& key);
    void multiply(const Key& key, Key& result);
    void multiplyReduced(const Key& key, Key& result);
	void multiplyByR2(Key& result);
	//void multiply(const unsigned block);
	void reduce();
	bool getBit(int position);
	void setBit(int position);
	//void shiftLeft();
	//void shiftLeft32();
	void rightShift();
	void divide(Key& divisor, Key& quotient);
	void invert();
};
