#pragma once

#include "Key.h"

struct Point
{
	static const unsigned ADDRESS0;
	static const unsigned COMPRESSION_ENDING[7];

	static const Point G;

	static Point gPowers[64];
	static Point gMultiples[Key::GROUP_SIZE / 2 + 1];

	static void initialize();
	
	//unsigned long long key;
	Key x;
	Key y;
	
	Point();
	explicit Point(unsigned long long key);
	Point(Key x, Key y);

    void add(const Point& point, Key& inverse, Point& result) const;
    void subtract(const Point& point, Key& inverse, Point& result) const;
	
	bool operator==(const Point& point);
	void operator+=(const Point& point);
	//void operator++();
	
	void double_();
	void compress(unsigned* output);
	//void group(Point* points);
};
