#include "Key.h"

const Key Key::ZERO    = Key(0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000); // 0
const Key Key::ONE     = Key(0x0000000000000001, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000); // 1
//const Key Key::TWO     = Key(0x0000000000000002, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000); // 2
const Key Key::THREE   = Key(0x0000000000000003, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000); // 3
const Key Key::R       = Key(0x00000001000003D1, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000); // 4294968273
const Key Key::R2      = Key(0x000007A2000E90A1, 0x0000000000000001, 0x0000000000000000, 0x0000000000000000); // 18446752466076602529
const Key Key::P_PRIME = Key(0xD838091DD2253531, 0xBCB223FEDC24A059, 0x9C46C2C295F2B761, 0xC9BD190515538399); // 91248989341183975618893650062416139444822672217621753343178995607987479196977
const Key Key::P       = Key(0xFFFFFFFEFFFFFC2F, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF); // 115792089237316195423570985008687907853269984665640564039457584007908834671663

void Key::gcd(Key a, Key b, Key& x, Key&y)
{
	if (a == ZERO)
	{
		x = ZERO;
		y = ONE;
		return;
	}
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

bool Key::operator==(const Key& key) // TODO: сделать эффективно (как в Vanity)
{
	return compare(key) == 0;
}

void Key::operator<<=(int shift)
{
	/*
	blocks[7] = shift < 8 ? blocks[7 - shift] : 0;
	blocks[6] = shift < 7 ? blocks[6 - shift] : 0;
	blocks[5] = shift < 6 ? blocks[5 - shift] : 0;
	blocks[4] = shift < 5 ? blocks[4 - shift] : 0;
	blocks[3] = shift < 4 ? blocks[3 - shift] : 0;
	blocks[2] = shift < 3 ? blocks[2 - shift] : 0;
	blocks[1] = shift < 2 ? blocks[1 - shift] : 0;
	blocks[0] = shift < 1 ? blocks[0] : 0;
	*/
	///*
	int shifts32 = shift / 32;
	blocks[7] = shifts32 < 8 ? blocks[7 - shifts32] : 0;
	blocks[6] = shifts32 < 7 ? blocks[6 - shifts32] : 0;
	blocks[5] = shifts32 < 6 ? blocks[5 - shifts32] : 0;
	blocks[4] = shifts32 < 5 ? blocks[4 - shifts32] : 0;
	blocks[3] = shifts32 < 4 ? blocks[3 - shifts32] : 0;
	blocks[2] = shifts32 < 3 ? blocks[2 - shifts32] : 0;
	blocks[1] = shifts32 < 2 ? blocks[1 - shifts32] : 0;
	blocks[0] = shifts32 < 1 ? blocks[0] : 0;
	int shifts1 = shift % 32;
	blocks[7] = (blocks[7] << shifts1) + (blocks[6] >> 32 - shifts1);
	blocks[6] = (blocks[6] << shifts1) + (blocks[5] >> 32 - shifts1);
	blocks[5] = (blocks[5] << shifts1) + (blocks[4] >> 32 - shifts1);
	blocks[4] = (blocks[4] << shifts1) + (blocks[3] >> 32 - shifts1);
	blocks[3] = (blocks[3] << shifts1) + (blocks[2] >> 32 - shifts1);
	blocks[2] = (blocks[2] << shifts1) + (blocks[1] >> 32 - shifts1);
	blocks[1] = (blocks[1] << shifts1) + (blocks[0] >> 32 - shifts1);
	blocks[0] = blocks[0] << shifts1;
	//*/
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
	multiply();
	reduce();
	multiply(key);
	reduce();
}

/*
void Key::operator++()
{
	unsigned long long result;
	unsigned carry = 1;
	int b = 0;
	while (carry)
	{
		result = (unsigned long long)blocks[b] + carry;
		blocks[b] = result;
		carry = result >> 32;
		b++;
	}
}
*/

int Key::compare(const Key& key) // TODO: сделать отдельные методы для ==, >, <, ... (как в Vanity)
{
	COMPARE_BLOCKS(blocks[3], key.blocks[3]);
	COMPARE_BLOCKS(blocks[2], key.blocks[2]);
	COMPARE_BLOCKS(blocks[1], key.blocks[1]);
	COMPARE_BLOCKS(blocks[0], key.blocks[0]);
	return 0;
}

int Key::compareExtended(const Key& key)
{
	COMPARE_BLOCKS(blocks[7], key.blocks[7]);
	COMPARE_BLOCKS(blocks[6], key.blocks[6]);
	COMPARE_BLOCKS(blocks[5], key.blocks[5]);
	COMPARE_BLOCKS(blocks[4], key.blocks[4]);
	COMPARE_BLOCKS(blocks[3], key.blocks[3]);
	COMPARE_BLOCKS(blocks[2], key.blocks[2]);
	COMPARE_BLOCKS(blocks[1], key.blocks[1]);
	COMPARE_BLOCKS(blocks[0], key.blocks[0]);
	return 0;
}

bool Key::add(const Key& key)
{
	unsigned long long carry = 0;
	ADD_BLOCKS("ADDS", blocks[0], key.blocks[0]);
	ADD_BLOCKS("ADCS", blocks[1], key.blocks[1]);
	ADD_BLOCKS("ADCS", blocks[2], key.blocks[2]);
	ADD_BLOCKS("ADCS", blocks[3], key.blocks[3]);
	ADD_BLOCKS("ADC", carry, 0ULL);
	return carry;
}

bool Key::addExtended(const Key& key)
{
	unsigned long long carry = 0;
	ADD_BLOCKS("ADDS", blocks[0], key.blocks[0]);
	ADD_BLOCKS("ADCS", blocks[1], key.blocks[1]);
	ADD_BLOCKS("ADCS", blocks[2], key.blocks[2]);
	ADD_BLOCKS("ADCS", blocks[3], key.blocks[3]);
	ADD_BLOCKS("ADCS", blocks[4], key.blocks[4]);
	ADD_BLOCKS("ADCS", blocks[5], key.blocks[5]);
	ADD_BLOCKS("ADCS", blocks[6], key.blocks[6]);
	ADD_BLOCKS("ADCS", blocks[7], key.blocks[7]);
	ADD_BLOCKS("ADC", carry, 0ULL);
	return carry;
}

bool Key::subtract(const Key& key)
{
	unsigned long long carry = 0;
	SUBTRACT_BLOCKS("SUBS", blocks[0], key.blocks[0]);
	SUBTRACT_BLOCKS("SBCS", blocks[1], key.blocks[1]);
	SUBTRACT_BLOCKS("SBCS", blocks[2], key.blocks[2]);
	SUBTRACT_BLOCKS("SBCS", blocks[3], key.blocks[3]);
	SUBTRACT_BLOCKS("SBC", carry, 0ULL);
	return carry;
}

void Key::multiply(const Key& key)
{
    unsigned long long tempLow;
    unsigned long long tempHigh;
    unsigned long long zero;
    __asm("MOV %[z], #0\n\t MOV %[r2], #0\n\t MOV %[r3], #0\n\t MOV %[r4], #0\n\t MOV %[r5], #0\n\t MOV %[r6], #0\n\t MOV %[r7], #0\n\t"
          "MUL %[r0], %[a0], %[b0]\n\t UMULH %[r1], %[a0], %[b0]\n\t"
          MULTIPLY_BLOCKS("0", "1", "1", "2", "3") MULTIPLY_BLOCKS("1", "0", "1", "2", "3")
          MULTIPLY_BLOCKS("2", "0", "2", "3", "4") MULTIPLY_BLOCKS("1", "1", "2", "3", "4") MULTIPLY_BLOCKS("0", "2", "2", "3", "4")
          MULTIPLY_BLOCKS("3", "0", "3", "4", "5") MULTIPLY_BLOCKS("2", "1", "3", "4", "5") MULTIPLY_BLOCKS("1", "2", "3", "4", "5") MULTIPLY_BLOCKS("0", "3", "3", "4", "5")
          MULTIPLY_BLOCKS("3", "1", "4", "5", "6") MULTIPLY_BLOCKS("2", "2", "4", "5", "6") MULTIPLY_BLOCKS("1", "3", "4", "5", "6")
          MULTIPLY_BLOCKS("3", "2", "5", "6", "7") MULTIPLY_BLOCKS("2", "3", "5", "6", "7")
          "MUL %[t_l], %[a3], %[b3]\n\t UMULH %[t_h], %[a3], %[b3]\n\t ADDS %[r6], %[r6], %[t_l]\n\t ADC %[r7], %[r7], %[z]\n\t ADD %[r7], %[r7], %[t_h]"
          : [t_l] "+r" (tempLow), [t_h] "+r" (tempHigh), [z] "+r" (zero), [r0] "+r" (blocks[0]), [r1] "+r" (blocks[1]), [r2] "+r" (blocks[2]), [r3] "+r" (blocks[3]), [r4] "+r" (blocks[4]), [r5] "+r" (blocks[5]), [r6] "+r" (blocks[6]), [r7] "+r" (blocks[7])
          : [a0] "r" (blocks[0]), [a1] "r" (blocks[1]), [a2] "r" (blocks[2]), [a3] "r" (blocks[3]), [b0] "r" (key.blocks[0]), [b1] "r" (key.blocks[1]), [b2] "r" (key.blocks[2]), [b3] "r" (key.blocks[3]));
}

void Key::multiply()
{
    /*
	unsigned results[16] = { 0x00000000 };
	unsigned tempLow;
	unsigned tempHigh;
	__asm("MULTIPLY_BLOCKS %[r0], %[r1], %[a0], %[b0]"
		: [r0] "=r" (results[0]), [r1] "=r" (results[1]) 
		: [a0] "r" (blocks[0]), [b0] "r" (R2.blocks[0]));
	__asm(MULTIPLY_BLOCKS("1", "0", "1", "2", "3")
		  MULTIPLY_BLOCKS("0", "1", "1", "2", "3")
		: [r1] "+r" (results[1]), [r2] "+r" (results[2]), [r3] "+r" (results[3]), [t_l] "+&r" (tempLow), [t_h] "+&r" (tempHigh) 
		: [a0] "r" (blocks[0]), [a1] "r" (blocks[1]), [b0] "r" (R2.blocks[0]), [b1] "r" (R2.blocks[1]));
	__asm(MULTIPLY_BLOCKS("2", "0", "2", "3", "4")
		  MULTIPLY_BLOCKS("1", "1", "2", "3", "4")
		  "ADDS %[r2], %[r2], %[a0]\n\t ADCS %[r3], %[r3], $0\n\t ADC %[r4], %[r4], $0"
		: [r2] "+r" (results[2]), [r3] "+r" (results[3]), [r4] "+r" (results[4]), [t_l] "+&r" (tempLow), [t_h] "+&r" (tempHigh) 
		: [a0] "r" (blocks[0]), [a1] "r" (blocks[1]), [a2] "r" (blocks[2]), [b0] "r" (R2.blocks[0]), [b1] "r" (R2.blocks[1]));
	__asm(MULTIPLY_BLOCKS("3", "0", "3", "4", "5")
		  MULTIPLY_BLOCKS("2", "1", "3", "4", "5")
		  "ADDS %[r3], %[r3], %[a1]\n\t ADCS %[r4], %[r4], $0\n\t ADC %[r5], %[r5], $0"
		: [r3] "+r" (results[3]), [r4] "+r" (results[4]), [r5] "+r" (results[5]), [t_l] "+&r" (tempLow), [t_h] "+&r" (tempHigh) 
		: [a1] "r" (blocks[1]), [a2] "r" (blocks[2]), [a3] "r" (blocks[3]), [b0] "r" (R2.blocks[0]), [b1] "r" (R2.blocks[1]));
	__asm(MULTIPLY_BLOCKS("4", "0", "4", "5", "6")
		  MULTIPLY_BLOCKS("3", "1", "4", "5", "6")
		  "ADDS %[r4], %[r4], %[a2]\n\t ADCS %[r5], %[r5], $0\n\t ADC %[r6], %[r6], $0"
		: [r4] "+r" (results[4]), [r5] "+r" (results[5]), [r6] "+r" (results[6]), [t_l] "+&r" (tempLow), [t_h] "+&r" (tempHigh) 
		: [a2] "r" (blocks[2]), [a3] "r" (blocks[3]),  [a4] "r" (blocks[4]), [b0] "r" (R2.blocks[0]), [b1] "r" (R2.blocks[1]));
	__asm(MULTIPLY_BLOCKS("5", "0", "5", "6", "7")
		  MULTIPLY_BLOCKS("4", "1", "5", "6", "7")
		  "ADDS %[r5], %[r5], %[a3]\n\t ADCS %[r6], %[r6], $0\n\t ADC %[r7], %[r7], $0"
		: [r5] "+r" (results[5]), [r6] "+r" (results[6]), [r7] "+r" (results[7]), [t_l] "+&r" (tempLow), [t_h] "+&r" (tempHigh) 
		: [a3] "r" (blocks[3]), [a4] "r" (blocks[4]),  [a5] "r" (blocks[5]), [b0] "r" (R2.blocks[0]), [b1] "r" (R2.blocks[1]));
	__asm(MULTIPLY_BLOCKS("6", "0", "6", "7", "8")
		  MULTIPLY_BLOCKS("5", "1", "6", "7", "8")
		  "ADDS %[r6], %[r6], %[a4]\n\t ADCS %[r7], %[r7], $0\n\t ADC %[r8], %[r8], $0"
		: [r6] "+r" (results[6]), [r7] "+r" (results[7]), [r8] "+r" (results[8]), [t_l] "+&r" (tempLow), [t_h] "+&r" (tempHigh) 
		: [a4] "r" (blocks[4]), [a5] "r" (blocks[5]),  [a6] "r" (blocks[6]), [b0] "r" (R2.blocks[0]), [b1] "r" (R2.blocks[1]));
	__asm(MULTIPLY_BLOCKS("7", "0", "7", "8", "9")
		  MULTIPLY_BLOCKS("6", "1", "7", "8", "9")
		  "ADDS %[r7], %[r7], %[a5]\n\t ADCS %[r8], %[r8], $0\n\t ADC %[r9], %[r9], $0"
		: [r7] "+r" (results[7]), [r8] "+r" (results[8]), [r9] "+r" (results[9]), [t_l] "+&r" (tempLow), [t_h] "+&r" (tempHigh) 
		: [a5] "r" (blocks[5]), [a6] "r" (blocks[6]),  [a7] "r" (blocks[7]), [b0] "r" (R2.blocks[0]), [b1] "r" (R2.blocks[1]));
	__asm(MULTIPLY_BLOCKS("7", "1", "8", "9", "10")
		  "ADDS %[r8], %[r8], %[a6]\n\t ADCS %[r9], %[r9], $0\n\t ADC %[r10], %[r10], $0"
		: [r8] "+r" (results[8]), [r9] "+r" (results[9]), [r10] "+r" (results[10]), [t_l] "+&r" (tempLow), [t_h] "+&r" (tempHigh) 
		: [a6] "r" (blocks[6]),  [a7] "r" (blocks[7]), [b1] "r" (R2.blocks[1]));
	__asm("ADDS %[r9], %[r9], %[a7]\n\t ADC %[r10], %[r10], $0"
		: [r9] "+r" (results[9]), [r10] "+r" (results[10])
		: [a7] "r" (blocks[7]));
	memcpy(blocks, results, 64);
     */
}

void Key::multiply(const unsigned block)
{
    /*
	unsigned results[16] = { 0x00000000 };
	unsigned tempLow;
	unsigned tempHigh;
	__asm("MULTIPLY_BLOCKS %[r0], %[r1], %[a0], %[b0]"
		: [r0] "=r" (results[0]), [r1] "=r" (results[1]) 
		: [a0] "r" (blocks[0]), [b0] "r" (block));
	__asm("MULTIPLY_BLOCKS %[t_l], %[t_h], %[a1], %[b0]\n\t ADDS %[r1], %[r1], %[t_l]\n\t ADC %[r2], %[r2], $0\n\t ADD %[r2], %[r2], %[t_h]"
		: [r1] "+r" (results[1]), [r2] "+r" (results[2]), [t_l] "+&r" (tempLow), [t_h] "+&r" (tempHigh) 
		: [a1] "r" (blocks[1]), [b0] "r" (block));
	__asm("MULTIPLY_BLOCKS %[t_l], %[t_h], %[a2], %[b0]\n\t ADDS %[r2], %[r2], %[t_l]\n\t ADC %[r3], %[r3], $0\n\t ADD %[r3], %[r3], %[t_h]"
		: [r2] "+r" (results[2]), [r3] "+r" (results[3]), [t_l] "+&r" (tempLow), [t_h] "+&r" (tempHigh) 
		: [a2] "r" (blocks[2]), [b0] "r" (block));
	__asm("MULTIPLY_BLOCKS %[t_l], %[t_h], %[a3], %[b0]\n\t ADDS %[r3], %[r3], %[t_l]\n\t ADC %[r4], %[r4], $0\n\t ADD %[r4], %[r4], %[t_h]"
		: [r3] "+r" (results[3]), [r4] "+r" (results[4]), [t_l] "+&r" (tempLow), [t_h] "+&r" (tempHigh) 
		: [a3] "r" (blocks[3]), [b0] "r" (block));
	__asm("MULTIPLY_BLOCKS %[t_l], %[t_h], %[a4], %[b0]\n\t ADDS %[r4], %[r4], %[t_l]\n\t ADC %[r5], %[r5], $0\n\t ADD %[r5], %[r5], %[t_h]"
		: [r4] "+r" (results[4]), [r5] "+r" (results[5]), [t_l] "+&r" (tempLow), [t_h] "+&r" (tempHigh) 
		: [a4] "r" (blocks[4]), [b0] "r" (block));
	__asm("MULTIPLY_BLOCKS %[t_l], %[t_h], %[a5], %[b0]\n\t ADDS %[r5], %[r5], %[t_l]\n\t ADC %[r6], %[r6], $0\n\t ADD %[r6], %[r6], %[t_h]"
		: [r5] "+r" (results[5]), [r6] "+r" (results[6]), [t_l] "+&r" (tempLow), [t_h] "+&r" (tempHigh) 
		: [a5] "r" (blocks[5]), [b0] "r" (block));
	__asm("MULTIPLY_BLOCKS %[t_l], %[t_h], %[a6], %[b0]\n\t ADDS %[r6], %[r6], %[t_l]\n\t ADC %[r7], %[r7], $0\n\t ADD %[r7], %[r7], %[t_h]"
		: [r6] "+r" (results[6]), [r7] "+r" (results[7]), [t_l] "+&r" (tempLow), [t_h] "+&r" (tempHigh) 
		: [a6] "r" (blocks[6]), [b0] "r" (block));
	__asm("MULTIPLY_BLOCKS %[t_l], %[t_h], %[a7], %[b0]\n\t ADDS %[r7], %[r7], %[t_l]\n\t ADC %[r8], %[r8], $0\n\t ADD %[r8], %[r8], %[t_h]"
		: [r7] "+r" (results[7]), [r8] "+r" (results[8]), [t_l] "+&r" (tempLow), [t_h] "+&r" (tempHigh) 
		: [a7] "r" (blocks[7]), [b0] "r" (block));
	memcpy(blocks, results, 64);
     */
}

void Key::reduce()
{
	Key temp = *this;
	temp.multiply(P_PRIME);
	temp.multiply(P);
	bool carry = addExtended(temp);
	memcpy(blocks, blocks + 8, 32);
	if (carry)
		add(R);
	else if (compare(P) >= 0)
		subtract(P);
}


bool Key::getBit(int position)
{
	return blocks[position / 32] & 1 << position % 32;
}

void Key::setBit(int position)
{
	blocks[position / 32] |= 1 << position % 32;
}

/*
void Key::shiftLeft()
{
	blocks[7] = (blocks[7] << 1) + (blocks[6] >> 31);
	blocks[6] = (blocks[6] << 1) + (blocks[5] >> 31);
	blocks[5] = (blocks[5] << 1) + (blocks[4] >> 31);
	blocks[4] = (blocks[4] << 1) + (blocks[3] >> 31);
	blocks[3] = (blocks[3] << 1) + (blocks[2] >> 31);
	blocks[2] = (blocks[2] << 1) + (blocks[1] >> 31);
	blocks[1] = (blocks[1] << 1) + (blocks[0] >> 31);
	blocks[0] = blocks[0] << 1;
}
*/

/*
void Key::shiftLeft32()
{
	blocks[7] = blocks[6];
	blocks[6] = blocks[5];
	blocks[5] = blocks[4];
	blocks[4] = blocks[3];
	blocks[3] = blocks[2];
	blocks[2] = blocks[1];
	blocks[1] = blocks[0];
	blocks[0] = 0;
}
*/

void Key::shiftRight()
{
	/*
	blocks[0] = blocks[1];
	blocks[1] = blocks[2];
	blocks[2] = blocks[3];
	blocks[3] = blocks[4];
	blocks[4] = blocks[5];
	blocks[5] = blocks[6];
	blocks[6] = blocks[7];
	blocks[7] = 0;
	*/
	///*
	__asm("MOV %[b0], %[b0], LSR #1\n\t ADD %[b0], %[b0], %[b1], LSL #31\n\t"
		  "MOV %[b1], %[b1], LSR #1\n\t ADD %[b1], %[b1], %[b2], LSL #31\n\t"
		  "MOV %[b2], %[b2], LSR #1\n\t ADD %[b2], %[b2], %[b3], LSL #31\n\t"
		  "MOV %[b3], %[b3], LSR #1\n\t ADD %[b3], %[b3], %[b4], LSL #31\n\t"
		  "MOV %[b4], %[b4], LSR #1\n\t ADD %[b4], %[b4], %[b5], LSL #31\n\t"
		  "MOV %[b5], %[b5], LSR #1\n\t ADD %[b5], %[b5], %[b6], LSL #31\n\t"
		  "MOV %[b6], %[b6], LSR #1\n\t ADD %[b6], %[b6], %[b7], LSL #31\n\t"
		  "MOV %[b7], %[b7], LSR #1\n\t"
		: [b0] "+r" (blocks[0]), [b1] "+r" (blocks[1]), [b2] "+r" (blocks[2]), [b3] "+r" (blocks[3]), [b4] "+r" (blocks[4]), [b5] "+r" (blocks[5]), [b6] "+r" (blocks[6]), [b7] "+r" (blocks[7]));
	//*/
}

// TODO: Knuth section 4.3.1
void Key::divide(Key& divisor, Key& quotient)
{
	int shifts = 255;
	while (!divisor.getBit(shifts))
		shifts--;
    shifts = 255 - shifts;
	divisor <<= shifts;
	quotient = ZERO;
	if (compare(divisor) >= 0)
	{
		subtract(divisor);
		quotient.setBit(shifts);
	}
	while (--shifts >= 0)
	{
		divisor.shiftRight();
		if (compare(divisor) >= 0)
		{
			subtract(divisor);
			quotient.setBit(shifts);
		}
	}
}


void Key::invert()
{
	///*
	Key x;
	Key y;
	gcd(*this, P, x, y);
	*this = x;
	//*/
	/*
	Key t = *this;
	t.multiplyByR2();
	t.reduce();
	*this = R2;
	reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); // 223 * 1
	multiply(*this); reduce(); // 0
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce();
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); // 22 * 1
	multiply(*this); reduce(); multiply(*this); reduce(); multiply(*this); reduce(); multiply(*this); reduce(); // 0 0 0 0
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); // 1 0
	multiply(*this); reduce(); multiply(t); reduce(); multiply(*this); reduce(); multiply(t); reduce(); // 1 1
	multiply(*this); reduce(); multiply(*this); reduce(); multiply(t); reduce(); // 0 1
	memset(blocks + 8, 0x00, 32);
	reduce();
	*/
}
