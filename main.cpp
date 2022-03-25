#include <mutex>
#include <pthread.h>

#include "test.h"

#define BLOCK_BITS 27
#define THREAD_BITS 2
#define PROGRESS_BITS 10
#define SUBBLOCK_BITS (63 - BLOCK_BITS - THREAD_BITS - PROGRESS_BITS - GROUP_BITS)
#define THREADS_NUMBER (1 << THREAD_BITS)
#define PROGRESSES_NUMBER (1 << PROGRESS_BITS)
#define SUBBLOCKS_NUMBER (1 << SUBBLOCK_BITS)

int block;
int threadsProgresses[THREADS_NUMBER] = { 0 };
std::mutex mutex_;
Point threadsPoints[THREADS_NUMBER + 1];
Timer timer;

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

void* thread_function(void* id)
{
    Point point;
    Point center(threadsPoints[*(int*)id]);
	unsigned compression[64];
	for (int i = 0; i < 7; i++)
	    compression[9 + i] = Point::COMPRESSION_ENDING[i];
	for (int p = 0; p < PROGRESSES_NUMBER; p++)
	{
	    for (int s = 0; s < SUBBLOCKS_NUMBER; s++)
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
                    print((((((((((1ULL << BLOCK_BITS) + block) << THREAD_BITS) + *(int*)id) << PROGRESS_BITS) + p) << SUBBLOCK_BITS) + s) << GROUP_BITS) + i);
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
		for (int threadProgress : threadsProgresses)
		    log &= threadProgress >= threadsProgresses[*(int*)id];
		if (log)
            std::cout << "[I] Progress = " << std::fixed << (p + 1) * 100.0 / PROGRESSES_NUMBER << " % [" << std::fixed << (THREADS_NUMBER * SUBBLOCKS_NUMBER * GROUP_SIZE / timer.stop() / 1000000.0) << " Mkeys/sec]" << std::endl;
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
    for (int i = 0; argv[1][i] != 0; i++)
        if (argv[1][i] < '0' || argv[1][i] > '9')
        {
            std::cout << "[E] Block number must be non-negative integer" << std::endl;
            return 0;
        }
    block = atoi(argv[1]);
	if (block < 0 || block >= 1 << BLOCK_BITS)
	{
		std::cout << "[E] Number " << block << " is not in range" << std::endl;
		return 0;
	}
//#ifdef DEBUG
	std::cout << "[I] CHECKING BLOCK # " << block << ":" << std::endl;
//#endif
	unsigned long long startKey = 1;
	startKey <<= BLOCK_BITS;
	startKey += block;
	startKey <<= THREAD_BITS;
	startKey <<= PROGRESS_BITS + SUBBLOCK_BITS + GROUP_BITS;
	startKey += GROUP_SIZE / 2;
	Point::initialize();
	threadsPoints[0] = Point(startKey);
	unsigned long long threadKeys = 1ULL << (PROGRESS_BITS + SUBBLOCK_BITS + GROUP_BITS);
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
		if (pthread_create(&threads[id], NULL, thread_function, &thread_ids[id]) != 0)
		{
		    std::cout << "[E] Error while creating thread # " << id << std::endl;
		    return 0;
		}
	}
	for (int id = 0; id < THREADS_NUMBER; id++)
		if (pthread_join(threads[id], NULL) != 0)
		{
			std::cout << "[E] Error while joining thread" << std::endl;
			return 0;
		}
//#ifdef DEBUG
    std::cout << "[I] BLOCK # " << block << " IS CHECKED" << std::endl;
//#endif
	return 0;
}
