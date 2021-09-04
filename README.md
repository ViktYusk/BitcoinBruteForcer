The target device of this project is [Raspberry Pi](https://www.raspberrypi.org/) (specifically, I use Raspberry Pi 3B+, but I think it is no problem to run the program on any of models). The important thing is that you need to use 64-bit operating system which is not common but it can be found as a beta-version [here](https://downloads.raspberrypi.org/raspios_lite_arm64/images/).

The project is written in C++. In order to optimize speed, for some operations [GCC Inline Assembly](https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html) with some specific intructions of ARM64 processors is used. To cross-compile this code from my laptop to Raspberry Pi, I use [CLion](https://www.jetbrains.com/clion/).

## Bitcoin Puzzle

A brief introduction to Bitcoin Puzzle, also known as BTC32, all necessary references, and some nice statistical analysis can be found is [this repository](https://github.com/HomelessPhD/BTC32). The whole Bitcoin Puzzle is a set of 160 puzzles with increasing difficulty, some of them are already solved (using brute force), and others are not. The easiest non-solved yet puzzle is # 64, and this project is specifically made to brute force this puzzle.

## Code Overview

### Key

`Key` represents a [Bitcoin](https://bitcoin.org/) private key which is a 256-bit number. In some cases, it also may represent just 256-bit numbers which are not valid Bitcoin private keys or even 512-bit numbers. In the project, it is stored as an array of four or eight `unsigned long long` i. e. 64-bit digits. To work with such data, multiprecision arithmetic is implemented.

A key can be contructed either from four 64-bit digits, or eight 64-bit digits (for some futher algorithms it is sometimes necessary to consider 512-bit keys).

There are some static constant fields such as `ZERO`, `ONE`, etc. for some important key constants.

The next principal methods for keys are implemented:
* `add`, `subtract` for arithmetical 256-bit addition and subtraction with a carry/borrow flag; `addExtended` adds 512-bit keys
* `multiply` for arithmetical multiplication; `multiplyByR2` for optimized multiplication by `R2`; `multiplyLow` and `multiplyHigh` for calculating only low/high part of the result; `reduce` does [Montgomery reduction](https://en.wikipedia.org/wiki/Montgomery_modular_multiplication#The_REDC_algorithm)
* `divide` for division ([Knuth's D-Algorithm](https://skanthak.homepage.t-online.de/division.html) is used)
* `operator+=`, `operator-=`, `operator*=`, `invert` for modular addition, subtraction, multiplication, and inversion modulo `P`, respectively; `invertGroup` effectively inverts a group of keys as described [here](https://en.wikipedia.org/wiki/Modular_multiplicative_inverse#Multiple_inverses)

### Point

`Point` represents a Bitcoin public point which is a point on [SECP256K1 Curve](https://en.bitcoin.it/wiki/Secp256k1) which is a. k. a. *Bitcoin Curve*. It consists of two 256-bit keys `x` and `y`. `G` is the generator point. `ADDRESS0` is the first 4 bytes of the puzzle address.

A point can be contructed either from two keys, or from `unsigned long long` private key as the corresponding public point (the last operation is quite slow).

The next principal methods for points are implemented:
* `initialize` is a static method to initialize `gPowers` and `gMultiples` which are some useful pre-computed points
* `operator+=`, `add`, `subtract` for [elliptic curve addition and subtraction](https://en.wikipedia.org/wiki/Elliptic_curve#The_group_law) (two last methods use the pre-computed inverse of the abscissas difference); `addReduced` and `subtractReduced` calculate `y % 2` instead of `y` for the result point
* `double_` for elliptic curve point doubling i. e. adding a point with itself
* `compress` method compresses the point to pass the result to `sha256` (it is not usual compression of a public point but an optimized one)

### sha256 and ripemd160

`sha256` and `ripemd160` are hashes which are used for converting a public point into a Bitcoin address. Addresses in this project are not common addresses of Bitcoin like `16jY7qLJnxb7CHZyqBP8qca9d51gAjyXQN` but Base58Check-decoded ones like `3EE4133D991F52FDF6A25C9834E0745AC74248A4` (by the way, the address in previous examples is the address of the puzzle). In the project, these hash-functions are highly optimized such that they break its specification rules; for example, SHA256 does not do packing and unpacking to optimize speed.

### test

All the methods from `Key`, `Point`, and functions from `sha256`, and `ripemd160` are tested in `test`. Some of them, which are directly used in brute-forcing keys, are speed-tested. There are time consumptions for the principal operations using 1 thread:
Operation | Time for 1 operation | Usages for 1 key | Time for 1 key | % of total time
--------- | -------------------- | ---------------- | -------------- | ---------------
`sha256` | 763 ns | 1.0000 | 763 ns | 22 %
`Key::invertGroup` | 2 900 000 ns | 0.0002 | 708 ns | 21 %
`Point::subtractReduced` | 1400 ns | 0.5000 | 700 ns | 20 %
`Point::addReduced` | 1400 ns | 0.4998 | 700 ns | 20 %
`ripemd160` | 521 ns | 1.0000 | 521 ns | 15 %
`Point::compress` | 22 ns | 1.0000 | 22 ns | 1 %
`Key::operator-=` | 25 ns | 0.5002 | 13 ns | 0 %
`Point::add` | 1500 ns | 0.0002 | 0 ns | 0 %

For now, it is about 3400 ns needed to check 1 private key using 1 thread. Therefore, the total speed is about 1.1M keys/second.

### main

The sought private key consists of 64 bits. In the project, the bits are divided into groups, where Bit # 1 is a higher one:
Bit # 1 | Bits # 2—27 | Bits # 28—29 | Bits # 30—40 | Bits # 41—52 | Bits # 53—64
------- | ----------- | ------------ | ------------ | ------------ | ------------
`1` | block bits | thread bits | progress bits | subblock bits | group bits

The total number of private keys to brute force is 2^63. The whole work is divided into 2^26 = 67 108 864 so called *blocks*, each of them corresponds to *block bits*. Since Raspberry Pi 3B+ has 4 cores, I use 2 bits as *thread bits*, i. e. for the first thread thread bits are `00`, for the second thread thread bits are `01`, etc. When *progress bits* group changes, there prints progress message in Debug configuration. Other bit groups are just being brute-forced.

The compiled program `bitcoin_puzzle` for Linux needs an argument with block number in decimal to process. For example, run
```
bitcoin_puzzle 14920854
```

in order to process block # 14920854. With current speed of calculations, it takes about 2 days to process a block, and after this you will see the next output:

```
9C7592C05DC70C7D
9C7592C88B0EE91F
9C7592C0FFB209D8
9C7592C11DA59D3D
9C7592D13D39CD7A
9C7592D944483217
9C7592D1518F88A6
9C7592DA0F75E300
9C7592D2566158E4
9C7592C2633A61C7
9C7592D2F15C577C
9C7592DB4103F871
9C7592D346F4F338
9C7592D3608A7772
9C7592DBA1C60A00
9C7592C3CA63B617
9C7592C3D365C3D3
9C7592DC5F1762E1
9C7592DD205DDAB9
9C7592DD7B363167
9C7592C58DD943CF
9C7592D592804387
9C7592CE22C27C33
9C7592D67433755C
9C7592D679E0D3F1
9C7592C6DE2B1726
9C7592D6ECB2C5B2
9C7592DF05329C3B
9C7592CF911F3295
9C7592D7F3436F0D
```

The output is a report which contains private keys with partially coinciding addresses (20%-coincidence is enough to get into a report). The program does not check the exact coincidence and does not show the addresses. Statistically, each block should have about 32 private keys in its report.

There is also finish point check. If the finish point for some thread is wrong (normally, it doesn't happen), then there will be an error message in the report.

## Contributions and Donations

I really want to optimize speed as much as possible up to several millions keys in a second. I would appreciate any advice or help. You can use my email to contact me:
```
viktyusk@gmail.com
```

If you want to donate me, please, use the next Bitcoin address:
```
bc1q383qqx90z3kcarpdct5fnz2flzwm3652czhp5v
```
