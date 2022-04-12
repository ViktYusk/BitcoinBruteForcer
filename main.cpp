#include <cstring>
#include <mutex>
#include <pthread.h>

#include "test.h"

const int blockBits = 27;
const int blocksNumber = 1 << blockBits;
const int maxThreadBits = 10;
int threadBits = 2;
int threadsNumber;
const int subblockBits = 12;
const int subblocksNumber = 1 << subblockBits;
int progressBits;
int progressesNumber;

int block;
int* threadsProgresses;
std::mutex mutex_;
Point* threadsPoints;
Timer timer;

bool is_natural(const char* string)
{
    for (int i = 0; string[i] != 0; i++)
        if (string[i] < '0' || string[i] > '9')
            return false;
    return true;
}

void print(unsigned char digit)
{
    std::cout << (char)(digit < 10 ? '0' + digit : 'A' + digit - 10);
}

void print(unsigned long long block)
{
    for (int i = 0; i < 16; i++)
        print((unsigned char)(block >> 4 * (15 - i) & 0xF));
}

void print(const unsigned* address)
{
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 4; j++)
        {
            unsigned char byte = *((unsigned char*)(address + i) + j);
            print((unsigned char)((byte & 0xF0) >> 4));
            print((unsigned char)(byte & 0x0F));
        }
}

void updateParameters()
{
    threadsNumber = 1 << threadBits;
    progressBits = 63 - blockBits - threadBits - subblockBits - GROUP_BITS;
    progressesNumber = 1 << progressBits;
}

void* thread_function(void* id)
{
    Point point;
    Point center(threadsPoints[*(int*)id]);
	unsigned compression[64];
	for (int i = 0; i < 7; i++)
	    compression[9 + i] = Point::COMPRESSION_ENDING[i];
	for (int p = 0; p < progressesNumber; p++)
	{
	    for (int s = 0; s < subblocksNumber; s++)
		{
            Key inverses[GROUP_SIZE / 2 + 1];
            for (int i = 0; i <= GROUP_SIZE / 2; i++)
            {
                inverses[i] = Point::gMultiples[i].x;
                inverses[i] -= center.x;
            }
            Key::invertGroup(inverses);
            for (int i = 0; i < GROUP_SIZE; i++)
            {
                if (i < GROUP_SIZE / 2)
                    center.subtractReduced(Point::gMultiples[GROUP_SIZE / 2 - 1 - i], inverses[GROUP_SIZE / 2 - 1 - i], point);
                else if (i > GROUP_SIZE / 2)
                    center.addReduced(Point::gMultiples[i - (GROUP_SIZE / 2 + 1)], inverses[i - (GROUP_SIZE / 2 + 1)], point);
                else
                {
                    point = center;
                    point.y.blocks[0] &= 1;
                }
                point.compress(compression);
                unsigned output[8];
                sha256(compression, output);
                if (ripemd160(output) == Point::ADDRESS0)
                {
                    unsigned address[5];
                    ripemd160(output, address);
                    mutex_.lock();
                    std::cout << "[W] ";
                    print((((((((((1ULL << blockBits) + block) << threadBits) + *(int*)id) << progressBits) + p) << subblockBits) + s) << GROUP_BITS) + i);
                    std::cout << " ";
                    print(address);
                    std::cout << std::endl;
                    mutex_.unlock();
                }
            }
			center.add(Point::gMultiples[GROUP_SIZE / 2], inverses[GROUP_SIZE / 2], point);
            center = point;
		}
//#ifdef DEBUG
		mutex_.lock();
	    threadsProgresses[*(int*)id]++;
		bool log = true;
		for (int i = 0; i < threadsNumber; i++)
		    log &= threadsProgresses[i] >= threadsProgresses[*(int*)id];
		if (log)
            std::cout << "[I] Progress = " << std::fixed << (p + 1) * 100.0 / progressesNumber << " % [" << std::fixed << (threadsNumber * subblocksNumber * GROUP_SIZE / timer.stop() / 1000000.0) << " Mkeys/sec]" << std::endl;
		mutex_.unlock();
//#endif
	}
	if (!(center == threadsPoints[*(int*)id + 1]))
	{
		mutex_.lock();
		std::cout << "[E] Wrong finish point for thread # " << *(int*)id << std::endl;
		mutex_.unlock();
	}
	pthread_exit(0);
}

int main(int argc, char* argv[])
{
    std::cout.precision(2);
#ifdef DEBUG
    int code = test();
    if (code == -1)
        return 0;
#else
    if (argc < 2)
        return test();
#endif
    if (!is_natural(argv[1]))
    {
        std::cout << "[E] Block number must be non-negative integer: " << argv[1] << std::endl;
        return 0;
    }
    if (strlen(argv[1]) > 9)
    {
        std::cout << "[E] Block number can't be so long: " << argv[1] << std::endl;
        return 0;
    }
    block = atoi(argv[1]);
	if (block >= blocksNumber)
	{
		std::cout << "[E] Block number must be less than " << blocksNumber << ": " << block << std::endl;
		return 0;
	}
    updateParameters();
    if (argc > 2)
    {
        if (!is_natural(argv[2]))
            std::cout << "[E] Thread bits number must be non-negative integer: " << argv[2] << std::endl;
        else if (strlen(argv[1]) > 9)
            std::cout << "[E] Thread bits number can't be so long: " << argv[2] << std::endl;
        else
        {
            int threadBits_ = atoi(argv[2]);
            if (threadBits_ > maxThreadBits)
                std::cout << "[E] Thread bits number must be less than " << maxThreadBits << ": " << threadBits_ << std::endl;
            else
            {
                threadBits = threadBits_;
                updateParameters();
            }
        }
    }
    threadsProgresses = new int[threadsNumber];
    for (int i = 0; i < threadsNumber; i++)
        threadsProgresses[i] = 0;
    threadsPoints = new Point[threadsNumber + 1];
//#ifdef DEBUG
	std::cout << "[I] CHECKING BLOCK # " << block << ":" << std::endl;
//#endif
	unsigned long long startKey = 1;
	startKey <<= blockBits;
	startKey += block;
	startKey <<= threadBits;
	startKey <<= progressBits + subblockBits + GROUP_BITS;
	startKey += GROUP_SIZE / 2;
	Point::initialize();
	threadsPoints[0] = Point(startKey);
	unsigned long long threadKeys = 1ULL << (progressBits + subblockBits + GROUP_BITS);
	unsigned long long threadKey = threadKeys;
	for (int i = 1; i <= threadsNumber; i++)
	{
		threadsPoints[i] = Point(threadKey);
		threadsPoints[i] += threadsPoints[0];
		threadKey += threadKeys;
	}
	timer = Timer();
    int* thread_ids = new int[threadsNumber];
	pthread_t* threads = new pthread_t[threadsNumber];
	for (int id = 0; id < threadsNumber; id++)
	{
		thread_ids[id] = id;
		if (pthread_create(&threads[id], NULL, thread_function, &thread_ids[id]) != 0)
		{
		    std::cout << "[E] Error while creating thread # " << id << std::endl;
		    return 0;
		}
	}
	for (int id = 0; id < threadsNumber; id++)
		if (pthread_join(threads[id], NULL) != 0)
		{
			std::cout << "[E] Error while joining thread" << std::endl;
			return 0;
		}
//#ifdef DEBUG
    std::cout << "[I] BLOCK # " << block << " IS CHECKED" << std::endl;
//#endif
    delete[] threadsProgresses;
    delete[] threadsPoints;
    delete[] thread_ids;
    delete[] threads;
	return 0;
}
