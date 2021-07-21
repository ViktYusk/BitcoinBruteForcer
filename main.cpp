#include <chrono>
#include <iostream>
#include <mutex>
#include <pthread.h>

#include "Point.h"
#include "sha256.h"
#include "ripemd160.h"
#include "test.h"

using namespace std;
using namespace chrono;

const int BLOCK_BITS    = 26;
const int THREAD_BITS   = 2;
const int PROGRESS_BITS = 14;
const int SUBBLOCK_BITS = 63 - BLOCK_BITS - THREAD_BITS - PROGRESS_BITS - Key::GROUP_BITS;
const int THREADS_NUMBER    = 1 << THREAD_BITS;
const int PROGRESSES_NUMBER = 1 << PROGRESS_BITS;
const int SUBBLOCKS_NUMBER  = 1 << SUBBLOCK_BITS;
const int GROUPS_PER_PROGRESS = THREADS_NUMBER * SUBBLOCKS_NUMBER;

int block;
int threadsProgresses[THREADS_NUMBER] = { 0 };
mutex mutex_;
Point threadsPoints[THREADS_NUMBER + 1];
Timer timer;

void print(unsigned char digit)
{
    cout << (char)(digit < 10 ? '0' + digit : 'A' + digit - 10);
}

void print(unsigned long long key)
{
    for (int i = 0; i < 16; i++)
        print((unsigned char)(key >> 4 * (15 - i) & 0xF));
}

void print(unsigned* address)
{
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 4; j++)
        {
            unsigned char byte = *((unsigned char*)(address + i) + j);
            print((unsigned char)((byte & 0xF0) >> 4));
            print((unsigned char)(byte & 0x0F));
        }
}

void* thread(void* id) 
{
    Point point;
    Point center(threadsPoints[*(int*)id]);
	unsigned compression[64];
	memcpy(compression + 9, Point::COMPRESSION_ENDING, sizeof(Point::COMPRESSION_ENDING));
	for (int p = 0; p < PROGRESSES_NUMBER; p++)
	{
	    for (int s = 0; s < SUBBLOCKS_NUMBER; s++)
		{
            Key inverses[Key::GROUP_SIZE / 2 + 1];
            for (int i = 0; i <= Key::GROUP_SIZE / 2; i++)
            {
                inverses[i] = Point::gMultiples[i].x;
                inverses[i] -= center.x;
            }
            Key::invertGroup(inverses);
            for (int i = 0; i < Key::GROUP_SIZE; i++)
            {
                if (i < Key::GROUP_SIZE / 2)
                    center.subtract(Point::gMultiples[Key::GROUP_SIZE / 2 - 1 - i], inverses[Key::GROUP_SIZE / 2 - 1 - i], point);
                else if (i > Key::GROUP_SIZE / 2)
                    center.add(Point::gMultiples[i - (Key::GROUP_SIZE / 2 + 1)], inverses[i - (Key::GROUP_SIZE / 2 + 1)], point);
                else
                    point = center;
                point.compress(compression);
                unsigned output[8];
                sha256(compression, output);
                unsigned address[5];
                ripemd160(output, address);
                if (address[0] == Point::ADDRESS0)
                {
                    mutex_.lock();
                    print((((((((((1ULL << BLOCK_BITS) + block) << THREAD_BITS) + *(int*)id) << PROGRESS_BITS) + p) << SUBBLOCK_BITS) + s) << Key::GROUP_BITS) + i);
                    cout << " ";
                    print(address);
                    cout << endl;
                    mutex_.unlock();
                }
            }
			center.add(Point::gMultiples[Key::GROUP_SIZE / 2], inverses[Key::GROUP_SIZE / 2], point);
            center = point;
		}
#ifdef DEBUG
		mutex_.lock();
		threadsProgresses[*(int*)id]++;
		bool log = true;
		for (int threadProgress : threadsProgresses)
			log &= threadProgress >= threadsProgresses[*(int*)id];
		if (log)
            cout << "Progress = " << (p + 1) * 100.0 / PROGRESSES_NUMBER << " %   [" << (int)(GROUPS_PER_PROGRESS * Key::GROUP_SIZE / timer.stop() / 1000) << " Kkeys/second]" << endl;
		mutex_.unlock();
#endif
	}
	if (!(center == threadsPoints[*(int*)id + 1]))
	{
		mutex_.lock();
		cout << "Wrong finish point for thread # " << *(int*)id << endl;
		mutex_.unlock();
	}
	pthread_exit(0);
}

int main(int argc, char* argv[])
{
#ifdef DEBUG
	if (test())
		return -1;
#endif
	if (argc < 2)
	{
		cout << "No argument with block number" << endl;
		return -1;
	}
	for (int i = 0; argv[1][i] != 0; i++)
		if (argv[1][i] < '0' || argv[1][i] > '9')
		{
			cout << "Block number must be non-negative integer" << endl;
			return -1;
		}
	block = atoi(argv[1]);
	if (block < 0 || block >= 1 << BLOCK_BITS)
	{
		cout << "Block is not in range" << endl;
		return -1;
	}
	unsigned long long startKey = 1;
	startKey <<= BLOCK_BITS;
	startKey += block;
	startKey <<= THREAD_BITS;
	startKey <<= PROGRESS_BITS + SUBBLOCK_BITS + Key::GROUP_BITS;
	startKey += Key::GROUP_SIZE / 2;
	Point::initialize();
	threadsPoints[0] = Point(startKey);
	unsigned long long threadKeys = 1ULL << (PROGRESS_BITS + SUBBLOCK_BITS + Key::GROUP_BITS);
	unsigned long long threadKey = threadKeys;
	for (int i = 1; i <= THREADS_NUMBER; i++)
	{
		threadsPoints[i] = Point(threadKey);
		threadsPoints[i] += threadsPoints[0];
		threadKey += threadKeys;
	}
	timer = Timer();
	int thread_ids[THREADS_NUMBER];
	pthread_t threads[THREADS_NUMBER];
	for (int id = 0; id < THREADS_NUMBER; id++)
	{
		thread_ids[id] = id;
		if (pthread_create(&threads[id], NULL, thread, &thread_ids[id]) != 0)
		{
			cout << "Error while creating thread # " << id << endl;
			return -1;
		}
	}
	for (int id = 0; id < THREADS_NUMBER; id++)
		if (pthread_join(threads[id], NULL) != 0)
		{
			cout << "Error while joining thread" << endl;
			return -1;
		}
	return 0;
}
