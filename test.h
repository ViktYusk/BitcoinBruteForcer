#pragma once

#include <string>

struct Timer
{
	static unsigned long long time();
	
	unsigned long long timer;
	
	Timer();
	unsigned long long stop(int iterations);
	unsigned long long stop();
};

bool check(unsigned char* array1, unsigned char* array2, int length);
long long test();
