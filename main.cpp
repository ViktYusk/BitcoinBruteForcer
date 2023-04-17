#include <cstring>
#include <mutex>
#include <pthread.h>
#include <string>

#include "key.h"
#include "point.h"
#include "ripemd160.h"
#include "sha256.h"
#include "test.h"

int prefixBits;
const int maxThreadBits = 10;
int threadBits = 2;
int threadsNumber;
const int subrangeBits = 12;
const int subrangesNumber = 1 << subrangeBits;
int progressBits;
int progressesNumber;

unsigned addressPrefix;
int* threadsProgresses;
std::mutex mutex_;
Key* threadsKeys;
Timer timer;

bool isNatural(const char* string)
{
    for (int i = 0; string[i] != 0; i++)
        if (string[i] < '0' || string[i] > '9')
            return false;
    return true;
}
bool isHex(const char* hex)
{
    int i = 0;
    while (hex[i])
    {
        if (!(hex[i] >= '0' && hex[i] <= '9' || hex[i] >= 'A' && hex[i] <= 'F' || hex[i] >= 'a' && hex[i] <= 'f'))
            return false;
        i++;
    }
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
void print(Key key)
{
    for (int i = 0; i < 4; i++)
        print(key.blocks[3 - i]);
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

void* threadFunction(void* id)
{
    Key key = threadsKeys[*(int*)id];
    Point centerPoint(key);
    Point semigroupPoint = Point(Key(1 << (GROUP_BITS - 1), 0, 0, 0));
    centerPoint += semigroupPoint;
    Point point;
	unsigned compression[64];
	for (int i = 0; i < 7; i++)
	    compression[9 + i] = Point::COMPRESSION_ENDING[i];
	for (int p = 0; p < progressesNumber; p++)
	{
	    for (int s = 0; s < subrangesNumber; s++)
		{
            Key inverses[GROUP_SIZE / 2 + 1];
            for (int i = 0; i <= GROUP_SIZE / 2; i++)
            {
                inverses[i] = Point::gMultiples[i].x;
                inverses[i] -= centerPoint.x;
            }
            Key::invertGroup(inverses);
            for (int i = 0; i < GROUP_SIZE; i++)
            {
                if (i < GROUP_SIZE / 2)
                    centerPoint.subtractReduced(Point::gMultiples[GROUP_SIZE / 2 - 1 - i], inverses[GROUP_SIZE / 2 - 1 - i], point);
                else if (i > GROUP_SIZE / 2)
                    centerPoint.addReduced(Point::gMultiples[i - (GROUP_SIZE / 2 + 1)], inverses[i - (GROUP_SIZE / 2 + 1)], point);
                else
                {
                    point = centerPoint;
                    point.y.blocks[0] &= 1;
                }
                point.compress(compression);
                unsigned output[8];
                sha256(compression, output);
                if (ripemd160(output) == addressPrefix)
                {
                    unsigned address[5];
                    ripemd160(output, address);
                    mutex_.lock();
                    std::cout << "[W] ";
                    print(key);
                    std::cout << " ";
                    print(address);
                    std::cout << std::endl;
                    mutex_.unlock();
                }
                key.increment();
            }
			centerPoint.add(Point::gMultiples[GROUP_SIZE / 2], inverses[GROUP_SIZE / 2], point);
            centerPoint = point;
		}
//#ifdef DEBUG
		mutex_.lock();
	    threadsProgresses[*(int*)id]++;
		bool log = true;
		for (int i = 0; i < threadsNumber; i++)
		    log &= threadsProgresses[i] >= threadsProgresses[*(int*)id];
		if (log)
            std::cout << "[I] Progress = " << std::fixed << (p + 1) * 100.0 / progressesNumber << " % [" << std::fixed << (threadsNumber * subrangesNumber * GROUP_SIZE / timer.stop() / 1000000.0) << " Mkeys/sec]" << std::endl;
		mutex_.unlock();
//#endif
	}
	Point nextCenterPoint = Point(threadsKeys[*(int*)id + 1]);
    nextCenterPoint += semigroupPoint;
    if (!(centerPoint == nextCenterPoint))
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
    if (test() == -1)
        return 0;
#endif
    if (argc < 3)
    {
        std::cout << "[E] There are two required parameters" << std::endl;
        return 0;
    }
    if (!isHex(argv[1]))
    {
        std::cout << "[E] Key prefix must be in HEX format" << std::endl;
        return 0;
    }
    if (strlen(argv[2]) != 8)
    {
        std::cout << "[E] Address prefix length must be 8" << std::endl;
        return 0;
    }
    if (!isHex(argv[2]))
    {
        std::cout << "[E] Address prefix must be in HEX format" << std::endl;
        return 0;
    }
    if (argc > 3)
    {
        if (!isNatural(argv[3])) {
            std::cout << "[E] Thread bits number must be non-negative integer: " << argv[3] << std::endl;
            return 0;
        } else {
            threadBits = atoi(argv[3]);
            if (threadBits > maxThreadBits) {
                std::cout << "[E] Thread bits number must be less than " << maxThreadBits << ": " << threadBits << std::endl;
                return 0;
            }
        }
    }
    int prefixLength = strlen(argv[1]);
    prefixBits = 4 * prefixLength;
    threadsNumber = 1 << threadBits;
    progressBits = 256 - prefixBits - threadBits - subrangeBits - GROUP_BITS;
    if (progressBits < 2)
    {
        std::cout << "[E] Prefix is too long" << std::endl;
        return 0;
    }
    else if (progressBits > 30)
    {
        std::cout << "[E] Prefix is too short" << std::endl;
        return 0;
    }
    progressesNumber = 1 << progressBits;
    char startKey[65];
    for (int i = 0; i < prefixLength; i++)
        startKey[i] = argv[1][i];
    for (int i = prefixLength; i < 64; i++)
        startKey[i] = '0';
    startKey[64] = 0;
    threadsKeys = new Key[threadsNumber + 1];
    threadsKeys[0] = Key(startKey);
    Key threadKeys(1ULL << (progressBits + subrangeBits + GROUP_BITS), 0, 0, 0);
    for (int i = 1; i <= threadsNumber; i++)
    {
        threadsKeys[i] = threadsKeys[i - 1];
        threadsKeys[i] += threadKeys;
    }
    threadsProgresses = new int[threadsNumber];
    for (int i = 0; i < threadsNumber; i++)
        threadsProgresses[i] = 0;
    char addressPrefixReversed[9];
    for (int i = 0; i < 4; i++)
    {
        addressPrefixReversed[2 * (3 - i)] = argv[2][2 * i];
        addressPrefixReversed[2 * (3 - i) + 1] = argv[2][2 * i + 1];
    }
    addressPrefixReversed[8] = 0;
    addressPrefix = std::strtoul(addressPrefixReversed, nullptr, 16);
    Point::initialize();
    std::cout << "[I] CHECKING PREFIX " << argv[1] << ":" << std::endl;
    timer = Timer();
    int* thread_ids = new int[threadsNumber];
	pthread_t* threads = new pthread_t[threadsNumber];
	for (int id = 0; id < threadsNumber; id++)
	{
		thread_ids[id] = id;
		if (pthread_create(&threads[id], nullptr, threadFunction, &thread_ids[id]) != 0)
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
    std::cout << "[I] PREFIX " << argv[1] << " CHECKED" << std::endl;
    delete[] threadsProgresses;
    delete[] threadsKeys;
    delete[] thread_ids;
    delete[] threads;
	return 0;
}
