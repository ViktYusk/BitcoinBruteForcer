#pragma once

#include "Key.h"

struct Point
{
	static const unsigned char ADDRESS[20];
	static const unsigned char COMPRESSION_ENDING[31];
	static const Point G;
	static Point gPowers[64];
	static Point gMultiples[Key::GROUP_SIZE / 2 + 1];
	
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
