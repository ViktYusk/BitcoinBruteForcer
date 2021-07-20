#pragma once

#include "Key.h"
#include "ripemd160.h"

struct Point
{
	static const unsigned ADDRESS0;
	static const unsigned COMPRESSION_ENDING[7];
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
	
	//unsigned long long key;
	Key x;
	Key y;
	
	Point();
	Point(unsigned long long key);
	Point(Key x, Key y);

    void add(const Point& point, Key& inverse, Point& result);
    void subtract(const Point& point, Key& inverse, Point& result);
	
	bool operator==(const Point& point);
	void operator+=(const Point& point);
	//void operator++();
	
	void double_();
	void compress(unsigned* output);
	//void group(Point* points);
};
