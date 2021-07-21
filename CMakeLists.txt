cmake_minimum_required(VERSION 3.16)
project(BitcoinPuzzleRaspberry)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread -O1")
if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG)
    #    add_definitions(-DCOUNT_TEST)
endif()
add_executable(bitcoin_puzzle
        sha256.h sha256.cpp
        ripemd160.h ripemd160.cpp
        Key.h Key.cpp
        Point.h Point.cpp
        test.h test.cpp
        main.cpp)
