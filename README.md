Use [GCC](https://gcc.gnu.org/) to compile this project.

To compile this project on Windows, I use [Cygwin](https://www.cygwin.com/).

To cross-compile this project on [Raspberry Pi](https://www.raspberrypi.org/), I use [CLion](https://www.jetbrains.com/clion/) IDE. The important thing is that you need to install a 64-bit Raspbian operating system which is not default, but it can be found as a beta-version [here](https://downloads.raspberrypi.org/raspios_lite_arm64/images/). In order to optimize speed, for some operations [GCC Inline Assembly](https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html) with some specific instructions of AArch64 processors is used, but the code is compatible with other processors.

This project can be compiled for other platforms as well.

## Bitcoin Puzzle

A brief introduction to Bitcoin Puzzle, also known as BTC32, all necessary references, and some nice statistical analysis can be found is [this repository](https://github.com/HomelessPhD/BTC32). The whole Bitcoin Puzzle is a set of 160 puzzles with increasing difficulty, some of them are already solved (using brute force), and others are not. The easiest non-solved yet puzzle is # 64, and this project is specifically made to brute force this puzzle.

## Code Overview

### Key

`Key` represents a [Bitcoin](https://bitcoin.org/) private key which is a 256-bit number. In some cases, it also may represent just 256-bit numbers which are not valid Bitcoin private keys or even 512-bit numbers. In the project, it is stored as an array of four or eight `unsigned long long` i. e. 64-bit digits. To work with such data, multiprecision arithmetic is implemented.

A key can be constructed either from four 64-bit digits, or eight 64-bit digits (for some further algorithms it is sometimes necessary to consider 512-bit numbers).

There are some static constant fields such as `ZERO`, `ONE`, etc. for some important key constants.

The next principal methods for keys are implemented:
* `add`, `subtract` for arithmetical 256-bit addition and subtraction with a carry/borrow flag; `addExtended` adds 512-bit keys; `increment` effectively add 1 to a key
* `multiply` for arithmetical multiplication; `multiplyByR2` for optimized multiplication by `R2`; `multiplyLow` for calculating only low part of the result; `reduce` does [Montgomery reduction](https://en.wikipedia.org/wiki/Montgomery_modular_multiplication#The_REDC_algorithm)
* `divide` for division ([Knuth's D-Algorithm](https://skanthak.homepage.t-online.de/division.html) is used)
* `operator+=`, `operator-=`, `operator*=`, `invert` for modular addition, subtraction, multiplication, and inversion modulo `P`, respectively; `invertGroup` effectively inverts a group of keys as described [here](https://en.wikipedia.org/wiki/Modular_multiplicative_inverse#Multiple_inverses)
* `gcd` performs the extended Euclidean algorithm

### Point

`Point` represents a Bitcoin public point which is a point on [`secp256k1` Curve](https://en.bitcoin.it/wiki/Secp256k1) which is a. k. a. *Bitcoin Curve*. It consists of two 256-bit keys `x` and `y`. `G` is the generator point. `ADDRESS0` is the first 4 bytes of the puzzle address.

A point can be constructed either from two keys, or from `unsigned long long` private key as the corresponding public point (the last operation is quite slow).

The next principal methods for points are implemented:
* `initialize` is a static method to initialize `gPowers` and `gMultiples` which are some useful pre-computed points
* `operator+=`, `add`, `subtract` for [elliptic curve addition and subtraction](https://en.wikipedia.org/wiki/Elliptic_curve#The_group_law) (two last methods use the pre-computed inverse of the abscissas difference); `addReduced` and `subtractReduced` calculate `y % 2` instead of `y` for the result point
* `double_` for elliptic curve point doubling i. e. adding a point with itself
* `compress` method compresses the point to pass the result to `sha256` (it is not usual compression of a public point but an optimized one)

### sha256 and ripemd160

`sha256` and `ripemd160` are hashes which are used for converting a public point into a Bitcoin address. Addresses in this project are not common addresses of Bitcoin like `16jY7qLJnxb7CHZyqBP8qca9d51gAjyXQN` but Base58Check-decoded ones like `3EE4133D991F52FDF6A25C9834E0745AC74248A4` (by the way, the address in previous examples is the address of the puzzle). In the project, these hash-functions are highly optimized such that they break their specification rules; for example, SHA256 does not do packing and unpacking to optimize speed.

### test

All the methods from `Key`, `Point`, and functions from `sha256`, and `ripemd160` are tested in `test`. Some of them, which are directly used in brute-forcing keys, are speed-tested.

On Windows (64-bit) with Intel(R) Celeron(R) J4125 processor the total speed is about **2.2 million keys per second**.

On Raspberry Pi 3B+ with a 64-bit operating system, the total speed is more than **1.2 million keys per second**. There are approximate time consumptions for the principal operations using 1 thread on Raspberry Pi 3B+ with a 64-bit operating system:

Operation | Time for 1 operation | Usages for 1 key | Time for 1 key | % of total time
--------- | -------------------- | ---------------- | -------------- | ---------------
`sha256` | 750 ns | 1.0000 | 750 ns | 25 %
`Point::subtractReduced` | 1200 ns | 0.5000 | 600 ns | 20 %
`Point::addReduced` | 1200 ns | 0.4998 | 600 ns | 20 %
`ripemd160` | 510 ns | 1.0000 | 510 ns | 17 %
`Key::invertGroup` | 2 300 000 ns | 0.0002 | 460 ns | 16 %
`Point::compress` | 15 ns | 1.0000 | 15 ns | 1 %
`Key::operator-=` | 17 ns | 0.5002 | 9 ns | 0 %
`Point::add` | 1200 ns | 0.0002 | 0 ns | 0 %

### main

The sought private key consists of 64 bits. In the project, the bits are divided into groups, where Bit # 1 is a higher one:

Bit # 1 | Bits # 2—28 | Bits # 29—30 | Bits # 31—40 | Bits # 41—52 | Bits # 53—64
------- | ----------- | ------------ | ------------ | ------------ | ------------
`1` | block bits | thread bits | progress bits | subblock bits | group bits

The total number of private keys to brute force is 2^63. The whole work is divided into 2^27 = 134 217 728 so called *blocks*, each of them contains 2^36 keys. *Block bits* correspond to a specific block. Since my laptop and Raspberry Pi 3B+ have 4 cores, I use 2 bits as *thread bits*, i.e. for the first thread, thread bits are `00`, for the second thread, thread bits are `01`, etc. When *progress bits* group changes, there prints a progress message. Other bit groups are just being brute-forced. With current speed of calculations, it takes about 15 hours to process a block on Raspberry Pi 3B+ with a 64-bit operating system.

The compiled program needs one argument with block number in decimal to process. If no argument is passed then the program will test itself.

The program prints lines to console with different prefixes:
* `[I]`: an informational message (for example, a progress message)
* `[W]`: a wallet with a partially coinciding address
* `[E]`: an error message

There is an example of a wallet in the program's output: 
```
[W] 9C7592C05DC70C7D 3EE4133DC0048754A7B8EB3A085032FADD54E184
```

It contains a pair of a hexadecimal key (`9C7592C05DC70C7D`) and the corresponding Base58Check-decoded address (`3EE4133DC0048754A7B8EB3A085032FADD54E184`). The program does not check the exact coincidence with the sought decoded address `3EE4133D991F52FDF6A25C9834E0745AC74248A4`; 20%-coincidence for an address is enough to get into the output. Statistically, each block should have about 16 wallets in its output.

There is also finish point check. If the finish point for some thread is wrong (normally, it doesn't happen), then there will be an error message in the output.

## Contributions and Donations

I really want to optimize speed as much as possible up to several million keys in a second. I would appreciate any advice or help. You can use my email to contact me:
```
viktyusk@gmail.com
```

If you want to donate me, please, use the next Bitcoin address:
```
bc1qdf30mdgp4ajltv2xktmcajja08lusjysqar9mz
```
