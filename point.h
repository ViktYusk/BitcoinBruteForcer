#pragma once

#include "key.h"

struct Point
{
	static const unsigned ADDRESS0;
	static const unsigned COMPRESSION_ENDING[7];

	static const Point G;

	static Point gPowers[256];
	static Point gMultiples[GROUP_SIZE / 2 + 1];

	static void initialize();

	Key x;
	Key y;
	
	Point();
    Point(Key key);
	Point(Key x, Key y);

	bool operator==(const Point& point) const;
	void operator+=(const Point& point);

    void add(const Point& point, Key& inverse, Point& result) const;
    void addReduced(const Point& point, Key& inverse, Point& result) const;
    void subtract(const Point& point, Key& inverse, Point& result) const;
    void subtractReduced(const Point& point, Key& inverse, Point& result) const;
	void double_();
	void compress(unsigned* output);
};
