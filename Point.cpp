#include <iostream>

#include "Point.h"
#include "Key.h"
#include "sha256.h"
#include "ripemd160.h"

using namespace std;

#ifdef DEBUG
// TODO: выбрать больший ключ
const unsigned char Point::ADDRESS[20] = { 0x2B, 0xA9, 0xA7, 0xE8, 0xEB, 0x11, 0x34, 0xF8, 0xF8, 0xB7, 0x70, 0x3A, 0x8B, 0x18, 0x3F, 0x72, 0x2C, 0x84, 0x03, 0x71 }; // 12056798280269540698
#else
const unsigned char Point::ADDRESS[20] = { 0x3E, 0xE4, 0x13, 0x3D, 0x99, 0x1F, 0x52, 0xFD, 0xF6, 0xA2, 0x5C, 0x98, 0x34, 0xE0, 0x74, 0x5A, 0xC7, 0x42, 0x48, 0xA4 };
#endif

const unsigned char Point::COMPRESSION_ENDING[31] = { 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x08 };

const Point Point::G = Point(1, Key(0x59F2815B16F81798, 0x029BFCDB2DCE28D9, 0x55A06295CE870B07, 0x79BE667EF9DCBBAC), Key(0x9C47D08FFB10D4B8, 0xFD17B448A6855419, 0x5DA4FBFC0E1108A8, 0x483ADA7726A3C465));  // (55066263022277343669578718895168534326250603453777594175500187360389116729240, 32670510020758816978083085130507043184471273380659243275938904335757337482424)

Point Point::gPowers[64] = {};
Point Point::gMultiples[Key::GROUP_SIZE / 2 + 1] = {};

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

bool Point::check(unsigned char* address1)
{
	for (int c = 0; c < 20; c++)
		if (address1[c] != ADDRESS[c])
			return false;
	return true;
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

// TODO: можно получать байты с помощью указателей
void Point::compress(unsigned char* output) // TODO: компрессировать в unsigned, а не unsigned char
{
	output[0] = y.blocks[0] % 2 == 0 ? 0x02 : 0x03;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 8; j++)
			output[1 + 8 * i + j] = x.blocks[3 - i] >> 8 * (7 - j);
}

void Point::group(Point* points)
{
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
