#include "Key.h"

const Key Key::ZERO      = Key(0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000); // 0
const Key Key::ONE       = Key(0x0000000000000001, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000); // 1
//const Key Key::TWO     = Key(0x0000000000000002, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000); // 2
const Key Key::THREE     = Key(0x0000000000000003, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000); // 3
const Key Key::R         = Key(0x00000001000003D1, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000); // 4294968273
const Key Key::R2        = Key(0x000007A2000E90A1, 0x0000000000000001, 0x0000000000000000, 0x0000000000000000); // 18446752466076602529
const Key Key::P_PRIME   = Key(0xD838091DD2253531, 0xBCB223FEDC24A059, 0x9C46C2C295F2B761, 0xC9BD190515538399); // 91248989341183975618893650062416139444822672217621753343178995607987479196977
const Key Key::P         = Key(0xFFFFFFFEFFFFFC2F, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF); // 115792089237316195423570985008687907853269984665640564039457584007908834671663

#ifdef COUNT_TEST
unsigned long long Key::gcdCounter = 0;
unsigned long long Key::invertGroupCounter = 0;
unsigned long long Key::operatorEqualToCounter = 0;
unsigned long long Key::operatorBitwiseLeftShiftAssignmentCounter = 0;
unsigned long long Key::operatorSubtractionAssignmentCounter = 0;
unsigned long long Key::operatorMultiplicationAssignmentCounter = 0;
unsigned long long Key::compareCounter = 0;
unsigned long long Key::addCounter = 0;
unsigned long long Key::addExtendedCounter = 0;
unsigned long long Key::subtractCounter = 0;
unsigned long long Key::multiplyCounter = 0;
unsigned long long Key::multiplyByR2Counter = 0;
unsigned long long Key::reduceCounter = 0;
unsigned long long Key::setBitCounter = 0;
unsigned long long Key::getBitCounter = 0;
unsigned long long Key::rightShiftCounter = 0;
unsigned long long Key::divideCounter = 0;
unsigned long long Key::invertCounter = 0;
#endif

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
#ifdef COUNT_TEST
    invertGroupCounter++;
#endif
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
{
}

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

bool Key::operator==(const Key& key)
{
#ifdef COUNT_TEST
    operatorEqualToCounter++;
#endif
	return blocks[0] == key.blocks[0] && blocks[1] == key.blocks[1] && blocks[2] == key.blocks[2] && blocks[3] == key.blocks[3];
}

void Key::operator<<=(int shift)
{
#ifdef COUNT_TEST
    operatorBitwiseLeftShiftAssignmentCounter++;
#endif
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
    int shifts64 = shift / 64;
    if (shifts64)
    {
        blocks[3] = shifts64 < 4 ? blocks[3 - shifts64] : 0;
        blocks[2] = shifts64 < 3 ? blocks[2 - shifts64] : 0;
        blocks[1] = shifts64 < 2 ? blocks[1 - shifts64] : 0;
        blocks[0] = shifts64 < 1 ? blocks[0] : 0;
    }
    int shifts1 = shift % 64;
    if (shifts1)
    {
        blocks[3] = (blocks[3] << shifts1) + (blocks[2] >> (64 - shifts1));
        blocks[2] = (blocks[2] << shifts1) + (blocks[1] >> (64 - shifts1));
        blocks[1] = (blocks[1] << shifts1) + (blocks[0] >> (64 - shifts1));
        blocks[0] = blocks[0] << shifts1;
    }
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
#ifdef COUNT_TEST
    operatorSubtractionAssignmentCounter++;
#endif
	if (subtract(key))
		add(P);
}

void Key::operator*=(const Key& key)
{
#ifdef COUNT_TEST
    operatorMultiplicationAssignmentCounter++;
#endif
    multiplyByR2();
	reduce();
	multiply(key, *this);
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

int Key::compare(const Key& key)
{
#ifdef COUNT_TEST
    compareCounter++;
#endif
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
#ifdef COUNT_TEST
    addCounter++;
#endif
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
#ifdef COUNT_TEST
    addExtendedCounter++;
#endif
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
#ifdef COUNT_TEST
    subtractCounter++;
#endif
    unsigned long long carry = 0;
	SUBTRACT_BLOCKS("SUBS", blocks[0], key.blocks[0]);
	SUBTRACT_BLOCKS("SBCS", blocks[1], key.blocks[1]);
	SUBTRACT_BLOCKS("SBCS", blocks[2], key.blocks[2]);
	SUBTRACT_BLOCKS("SBCS", blocks[3], key.blocks[3]);
	SUBTRACT_BLOCKS("SBC", carry, 0ULL);
	return carry;
}

void Key::multiply(const Key& key, Key& result)
{
#ifdef COUNT_TEST
    multiplyCounter++;
#endif
    __asm(
    "MUL %[r0], %[a0], %[b0]\n\t UMULH %[r1], %[a0], %[b0]\n\t MUL x11, %[a1], %[b0]\n\t UMULH %[r2], %[a1], %[b0]\n\t MUL x12, %[a2], %[b0]\n\t UMULH %[r3], %[a2], %[b0]\n\t MUL x13, %[a3], %[b0]\n\t UMULH %[r4], %[a3], %[b0]\n\t"
    "ADDS %[r1], %[r1], x11\n\t ADCS %[r2], %[r2], x12\n\t ADCS %[r3], %[r3], x13\n\t ADC %[r4], %[r4], xzr\n\t"
    "MUL x0, %[a0], %[b1]\n\t UMULH x1, %[a0], %[b1]\n\t MUL x11, %[a1], %[b1]\n\t UMULH x2, %[a1], %[b1]\n\t MUL x12, %[a2], %[b1]\n\t UMULH x3, %[a2], %[b1]\n\t MUL x13, %[a3], %[b1]\n\t UMULH x4, %[a3], %[b1]\n\t"
    "ADDS x1, x1, x11\n\t ADCS x2, x2, x12\n\t ADCS x3, x3, x13\n\t ADC x4, x4, xzr\n\t"
    "ADDS %[r1], %[r1], x0\n\t ADCS %[r2], %[r2], x1\n\t ADCS %[r3], %[r3], x2\n\t ADCS %[r4], %[r4], x3\n\t ADC %[r5], x4, xzr\n\t"
    "MUL x0, %[a0], %[b2]\n\t UMULH x1, %[a0], %[b2]\n\t MUL x11, %[a1], %[b2]\n\t UMULH x2, %[a1], %[b2]\n\t MUL x12, %[a2], %[b2]\n\t UMULH x3, %[a2], %[b2]\n\t MUL x13, %[a3], %[b2]\n\t UMULH x4, %[a3], %[b2]\n\t"
    "ADDS x1, x1, x11\n\t ADCS x2, x2, x12\n\t ADCS x3, x3, x13\n\t ADC x4, x4, xzr\n\t"
    "ADDS %[r2], %[r2], x0\n\t ADCS %[r3], %[r3], x1\n\t ADCS %[r4], %[r4], x2\n\t ADCS %[r5], %[r5], x3\n\t ADC %[r6], x4, xzr\n\t"
    "MUL x0, %[a0], %[b3]\n\t UMULH x1, %[a0], %[b3]\n\t MUL x11, %[a1], %[b3]\n\t UMULH x2, %[a1], %[b3]\n\t MUL x12, %[a2], %[b3]\n\t UMULH x3, %[a2], %[b3]\n\t MUL x13, %[a3], %[b3]\n\t UMULH x4, %[a3], %[b3]\n\t"
    "ADDS x1, x1, x11\n\t ADCS x2, x2, x12\n\t ADCS x3, x3, x13\n\t ADC x4, x4, xzr\n\t"
    "ADDS %[r3], %[r3], x0\n\t ADCS %[r4], %[r4], x1\n\t ADCS %[r5], %[r5], x2\n\t ADCS %[r6], %[r6], x3\n\t ADC %[r7], x4, xzr"
    : [r0] "+r" (result.blocks[0]), [r1] "+r" (result.blocks[1]), [r2] "+r" (result.blocks[2]), [r3] "+r" (result.blocks[3]), [r4] "+r" (result.blocks[4]), [r5] "+r" (result.blocks[5]), [r6] "+r" (result.blocks[6]), [r7] "+r" (result.blocks[7])
    : [a0] "r" (blocks[0]), [a1] "r" (blocks[1]), [a2] "r" (blocks[2]), [a3] "r" (blocks[3]), [b0] "r" (key.blocks[0]), [b1] "r" (key.blocks[1]), [b2] "r" (key.blocks[2]), [b3] "r" (key.blocks[3])
    : "x0", "x1", "x2", "x3", "x4", "x11", "x12", "x13"
    );
}

void Key::multiplyByR2()
{
#ifdef COUNT_TEST
    multiplyByR2Counter++;
#endif
    __asm(
        "MUL %[r0], %[a0], %[b0]\n\t UMULH %[r1], %[a0], %[b0]\n\t MUL x11, %[a1], %[b0]\n\t UMULH %[r2], %[a1], %[b0]\n\t MUL x12, %[a2], %[b0]\n\t UMULH %[r3], %[a2], %[b0]\n\t MUL x13, %[a3], %[b0]\n\t UMULH %[r4], %[a3], %[b0]\n\t"
        "ADDS %[r1], %[r1], x11\n\t ADCS %[r2], %[r2], x12\n\t ADCS %[r3], %[r3], x13\n\t ADC %[r4], %[r4], xzr\n\t"
        "ADDS %[r1], %[r1], %[a0]\n\t ADCS %[r2], %[r2], %[a1]\n\t ADCS %[r3], %[r3], %[a2]\n\t ADCS %[r4], %[r4], %[a3]\n\t ADC %[r5], xzr, xzr"
        : [r0] "+r" (blocks[0]), [r1] "+r" (blocks[1]), [r2] "+r" (blocks[2]), [r3] "+r" (blocks[3]), [r4] "+r" (blocks[4]), [r5] "+r" (blocks[5])
        : [a0] "r" (blocks[0]), [a1] "r" (blocks[1]), [a2] "r" (blocks[2]), [a3] "r" (blocks[3]), [b0] "r" (R2.blocks[0])
        : "x11", "x12", "x13"
    );
    blocks[6] = blocks[7] = 0;
}

void Key::reduce()
{
#ifdef COUNT_TEST
    reduceCounter++;
#endif
    Key temp;
    multiply(P_PRIME, temp);
	temp.multiply(P, temp);
	bool carry = addExtended(temp);
	blocks[0] = blocks[4];
    blocks[1] = blocks[5];
    blocks[2] = blocks[6];
    blocks[3] = blocks[7];
	if (carry)
		add(R);
	else if (compare(P) >= 0)
		subtract(P);
}

bool Key::getBit(int position)
{
#ifdef COUNT_TEST
    getBitCounter++;
#endif
    return blocks[position / 64] & 1ULL << position % 64;
}

void Key::setBit(int position)
{
#ifdef COUNT_TEST
    setBitCounter++;
#endif
	blocks[position / 64] |= 1ULL << position % 64;
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

void Key::rightShift()
{
#ifdef COUNT_TEST
    rightShiftCounter++;
#endif
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
	__asm("MOV %[b0], %[b0], LSR #1\n\t ADD %[b0], %[b0], %[b1], LSL #63\n\t"
		  "MOV %[b1], %[b1], LSR #1\n\t ADD %[b1], %[b1], %[b2], LSL #63\n\t"
		  "MOV %[b2], %[b2], LSR #1\n\t ADD %[b2], %[b2], %[b3], LSL #63\n\t"
		  "MOV %[b3], %[b3], LSR #1\n\t"
		: [b0] "+r" (blocks[0]), [b1] "+r" (blocks[1]), [b2] "+r" (blocks[2]), [b3] "+r" (blocks[3]));
}

void Key::divide(Key& divisor, Key& quotient)
{
#ifdef COUNT_TEST
    divideCounter++;
#endif
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
        divisor.rightShift();
		if (compare(divisor) >= 0)
		{
			subtract(divisor);
			quotient.setBit(shifts);
		}
	}
}


void Key::invert()
{
#ifdef COUNT_TEST
    invertCounter++;
    gcdCounter++;
#endif
	///*
	Key x;
	Key y;
	gcd(*this, P, x, y);
	*this = x;
}
