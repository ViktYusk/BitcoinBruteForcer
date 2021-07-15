#pragma once

#include "Key.h"
#include "sha256.h"
#include "ripemd160.h"

struct Point
{
	static const unsigned char ADDRESS[20];
	static const unsigned char COMPRESSION_ENDING[31];
	static const Point G;

	static Point gPowers[64];
	static Point gMultiples[Key::GROUP_SIZE / 2 + 1];

#ifdef COUNT_TEST
	static unsigned long long checkCounter;
    static unsigned long long addCounter;
    static unsigned long long subtractCounter;
    static unsigned long long compressCounter;
    static unsigned long long groupCounter;
#endif

	static void initialize();
	static bool check(unsigned char* address);
	
	unsigned long long key;
	Key x;
	Key y;
	
	Point();
	Point(unsigned long long key);
	Point(unsigned long long key, Key x, Key y);
	
	void add(const Point& point, Key inverse);
	void subtract(const Point& point, Key inverse);
	
	bool operator==(const Point& point);
	void operator+=(const Point& point);
	//void operator++();
	
	void double_();
	void compress(unsigned char* output);
	void group(Point* points);
};
