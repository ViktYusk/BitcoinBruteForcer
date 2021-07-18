#include <iostream>

#include "Point.h"

using namespace std;

#ifdef DEBUG
// TODO: выбрать больший ключ
const unsigned Point::ADDRESS0 = 0xE8A7A92B; // 2BA9A7E8EB1134F8F8B7703A8B183F722C840371
#else
const unsigned Point::ADDRESS0 = 0x3D13E43E; // 3EE4133D991F52FDF6A25C9834E0745AC74248A4
#endif

const unsigned Point::COMPRESSION_ENDING[7] = { 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000108 };

const Point Point::G = Point(1, Key(0x59F2815B16F81798, 0x029BFCDB2DCE28D9, 0x55A06295CE870B07, 0x79BE667EF9DCBBAC), Key(0x9C47D08FFB10D4B8, 0xFD17B448A6855419, 0x5DA4FBFC0E1108A8, 0x483ADA7726A3C465));  // (55066263022277343669578718895168534326250603453777594175500187360389116729240, 32670510020758816978083085130507043184471273380659243275938904335757337482424)

Point Point::gPowers[64] = {};
Point Point::gMultiples[Key::GROUP_SIZE / 2 + 1] = {};

#ifdef COUNT_TEST
unsigned long long Point::checkCounter = 0;
unsigned long long Point::addCounter = 0;
unsigned long long Point::subtractCounter = 0;
unsigned long long Point::compressCounter = 0;
unsigned long long Point::groupCounter = 0;
#endif

void Point::initialize()
{
	gPowers[0] = G;
	for (int i = 1; i < 64; i++)
	{
		gPowers[i] = gPowers[i - 1];
		gPowers[i].double_();
	}
	gMultiples[0] = Point::G;
	gMultiples[1] = gMultiples[0];
	gMultiples[1].double_();
	for (int i = 2; i < Key::GROUP_SIZE / 2; i++)
	{
		gMultiples[i] = gMultiples[i - 1];
		gMultiples[i] += Point::G;
	}
	gMultiples[Key::GROUP_SIZE / 2] = gMultiples[Key::GROUP_SIZE / 2 - 1];
	gMultiples[Key::GROUP_SIZE / 2].double_();
}

Point::Point()
{
}

Point::Point(unsigned long long key)
{
	this->key = key;
	int bit = 0;
	while (!(key & 1ULL << bit))
		bit++;
	*this = gPowers[bit];
	for (bit += 1; bit < 64; bit++)
		if (key & 1ULL << bit)
			operator+=(gPowers[bit]);
}

Point::Point(unsigned long long key, Key x, Key y)
{
	this->key = key;
	this->x = x;
	this->y = y;
}

void Point::add(const Point& point, Key inverse)
{
#ifdef COUNT_TEST
    addCounter++;
#endif
	key += point.key;
	Key slope = point.y;
	slope -= y;
	slope *= inverse;
	Key result_x = slope;
	result_x *= slope;
	result_x -= x;
	result_x -= point.x;
	Key result_y = x;
	result_y -= result_x;
	result_y *= slope;
	result_y -= y;
	x = result_x;
	y = result_y;
}

void Point::subtract(const Point& point, Key inverse)
{
#ifdef COUNT_TEST
    subtractCounter++;
#endif
	key -= point.key;
	Key slope = Key::P;
	slope.subtract(point.y);
	slope -= y;
	slope *= inverse;
	Key result_x = slope;
	result_x *= slope;
	result_x -= x;
	result_x -= point.x;
	Key result_y = x;
	result_y -= result_x;
	result_y *= slope;
	result_y -= y;
	x = result_x;
	y = result_y;
}

bool Point::operator==(const Point& point)
{
	return key == point.key && x == point.x && y == point.y;
}

void Point::operator+=(const Point& point)
{
	Key inverse = point.x;
	inverse -= x;
	inverse.invert();
	add(point, inverse);
	/*
	key += point.key;
	Key dx = point.x;
	dx -= x;
	dx.invert();
	Key slope = point.y;
	slope -= y;
	slope *= dx;
	Key result_x = slope;
	result_x *= slope;
	result_x -= x;
	result_x -= point.x;
	Key result_y = x;
	result_y -= result_x;
	result_y *= slope;
	result_y -= y;
	x = result_x;
	y = result_y;
	*/
}

/*
void Point::operator++()
{
	operator+=(G);
}
*/

void Point::double_()
{
	key += key;
	Key slope = y;
	slope += y;
	slope.invert();
	slope *= x;
	slope *= x;
	slope *= Key::THREE;
	Key result_x = slope;
	result_x *= slope;
	result_x -= x;
	result_x -= x;
	Key result_y = x;
	result_y -= result_x;
	result_y *= slope;
	result_y -= y;
	x = result_x;
	y = result_y;
}

void Point::compress(unsigned* output)
{
#ifdef COUNT_TEST
    compressCounter++;
#endif
	output[0] = ((0x02 + (unsigned)y.blocks[0] % 2) << 24) + (unsigned)(x.blocks[3] >> 40);
	output[1] = x.blocks[3] >> 8;
    output[2] = ((unsigned)x.blocks[3] << 24) + (unsigned)(x.blocks[2] >> 40);
    output[3] = x.blocks[2] >> 8;
    output[4] = ((unsigned)x.blocks[2] << 24) + (unsigned)(x.blocks[1] >> 40);
    output[5] = x.blocks[1] >> 8;
    output[6] = ((unsigned)x.blocks[1] << 24) + (unsigned)(x.blocks[0] >> 40);
    output[7] = x.blocks[0] >> 8;
    output[8] = (x.blocks[0] << 24) + 0x00800000;

    /*
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 8; j++)
			output[1 + 8 * i + j] = x.blocks[3 - i] >> 8 * (7 - j);
     */
}

void Point::group(Point* points)
{
#ifdef COUNT_TEST
    groupCounter++;
#endif
	points[Key::GROUP_SIZE / 2] = *this;
	Key inverses[Key::GROUP_SIZE / 2 + 1];
	for (int i = 0; i <= Key::GROUP_SIZE / 2; i++)
	{
		inverses[i] = gMultiples[i].x;
		inverses[i] -= x;
	}
	Key::invertGroup(inverses);
	for (int i = 0; i < Key::GROUP_SIZE / 2 - 1; i++)
	{
		points[Key::GROUP_SIZE / 2 + 1 + i] = *this;
		points[Key::GROUP_SIZE / 2 + 1 + i].add(gMultiples[i], inverses[i]);
	}
	points[Key::GROUP_SIZE] = *this;
	points[Key::GROUP_SIZE].add(gMultiples[Key::GROUP_SIZE / 2], inverses[Key::GROUP_SIZE / 2]);
	for (int i = 0; i < Key::GROUP_SIZE / 2; i++)
	{
		points[Key::GROUP_SIZE / 2 - 1 - i] = *this;
		points[Key::GROUP_SIZE / 2 - 1 - i].subtract(gMultiples[i], inverses[i]);
	}
}
