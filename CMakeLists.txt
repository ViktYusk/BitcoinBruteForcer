cmake_minimum_required(VERSION 3.16)
project(BitcoinPuzzleRaspberry)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread")

add_executable(BitcoinPuzzleRaspberry Key.cpp Key.h main.cpp Point.cpp Point.h ripemd160.cpp ripemd160.h sha256.cpp sha256.h test.cpp test.h)

if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG)
endif()
