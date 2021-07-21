#pragma once

#include <cstring>

#define COMPARE_BLOCKS(block1, block2) if (block1 > block2) return 1; if (block1 < block2) return -1;
#define ADD_BLOCKS(command, block1, block2) __asm(command " %[b1], %[b1], %[b2]" : [b1] "+r" (block1) : [b2] "r" (block2));
#define SUBTRACT_BLOCKS(command, block1, block2) __asm(command " %[b1], %[b1], %[b2]" : [b1] "+r" (block1) : [b2] "r" (block2));
#define MULTIPLY_BLOCKS(a, b, r1, r2, r3) "MUL x0, %[a" a "], %[b" b "]\n\t UMULH x1, %[a" a "], %[b" b "]\n\t ADDS %[r" r1 "], %[r" r1 "], x0\n\t ADCS %[r" r2 "], %[r" r2 "], xzr\n\t ADC %[r" r3 "], %[r" r3 "], xzr\n\t ADDS %[r" r2 "], %[r" r2 "], x1\n\t ADC %[r" r3 "], %[r" r3 "], xzr\n\t"

//using namespace std;

struct Key
{
	static const int GROUP_BITS = 12;
	static const int GROUP_SIZE = 1 << GROUP_BITS;
	
	static const Key ZERO;
	static const Key ONE;
	//static const Key TWO;
	static const Key THREE;
	static const Key R;
	static const Key R2;
	static const Key P_PRIME;
	static const Key P;

#ifdef COUNT_TEST
	static unsigned long long gcdCounter;
	static unsigned long long invertGroupCounter;
    static unsigned long long operatorEqualToCounter;
    static unsigned long long operatorBitwiseLeftShiftAssignmentCounter;
    static unsigned long long operatorSubtractionAssignmentCounter;
    static unsigned long long operatorMultiplicationAssignmentCounter;
    static unsigned long long compareCounter;
    static unsigned long long addCounter;
    static unsigned long long addExtendedCounter;
    static unsigned long long subtractCounter;
    static unsigned long long multiplyCounter;
    static unsigned long long multiplyByR2Counter;
    static unsigned long long reduceCounter;
    static unsigned long long setBitCounter;
    static unsigned long long getBitCounter;
    static unsigned long long rightShiftCounter;
    static unsigned long long divideCounter;
    static unsigned long long invertCounter;
#endif


	static void gcd(Key a, Key b, Key& x, Key& y);
	static void invertGroup(Key* keys);

	unsigned long long blocks[8];
	
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
