The target device of this project is [Raspberry Pi](https://www.raspberrypi.org/) (specifically, I use Raspberry Pi 3B+, but I think it is no problem to run the program on any of models). The important thing that you need to use 64-bit operating system which is not common but it can be found as a beta-version [here](https://downloads.raspberrypi.org/raspios_lite_arm64/images/).

The project is written in C++. In order to optimize speed, for some operations [GCC Inline Assembly](https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html) with some specific intructions of ARM64 processors is used. To cross-compile this code from my laptop to Raspberry Pi, I use [CLion](https://www.jetbrains.com/clion/).

## Bitcoin Puzzle

A brief introduction to Bitcoin Puzzle, also known as BTC32, all necessary references, and some nice statistical analysis can be found is [this repository](https://github.com/HomelessPhD/BTC32). The whole Bitcoin Puzzle is a set of 160 puzzles with increasing difficulty, some of them are already solved (using brute force), and others are not. The easiest non-solved yet puzzle is # 64, and this project is specifically made to brute force this puzzle.

## Code Overview

### Key

`Key` represents a [Bitcoin](https://bitcoin.org/) private key which is a 256-bit number. In some cases, it also may represent just 256-bit numbers which are not valid Bitcoin pirvate keys or even 512-bit numbers. In the project, it is stored as an array of four or eight `unsigned long long` i. e. 64-bit digits. To work with such data, multiprecision arithmetic is implemented.

A key can be contructed either from four 64-bit digits, or eight 64-bit digits (for some futher algorithms it is sometimes necessary to consider 512-bit keys).

There are some static constant fields such as `Key::ZERO`, `Key::ONE`, etc. for some important key constants.

The next principal methods for keys are implemented:
* `add`, `subtract`, `multiply`, `divide` for common arithmetic operations with 256-bit keys (for division, [Knuth's D-Algorithm](https://skanthak.homepage.t-online.de/division.html) is used); `addExtended` adds 512-bit keys; `multiplyReduced` and `multiplyByR2` are optimized multiplication methods, the first of them calculates only lower 256-bit product, the second of them multiplies a key by `Key::R2` which is a rather small key
* `reduce` does [Montgomery reduction](https://en.wikipedia.org/wiki/Montgomery_modular_multiplication#The_REDC_algorithm)
* `operator+=`, `operator-=`, `operator*=`, `invert` for modular addition, subtraction, multiplication, and inversion modulo `Key::P`, respectively; `invertGroup` effectively inverts a group of keys as described [here](https://en.wikipedia.org/wiki/Modular_multiplicative_inverse#Multiple_inverses).

### Point

`Point` represents a Bitcoin public point which is a point on [SECP256K1 Curve](https://en.bitcoin.it/wiki/Secp256k1) which is a. k. a. *Bitcoin Curve*. It consists of two 256-bit keys `x` and `y`. `Point::G` is the generator point.

The next principal methods for points are implemented:
> TODO...

### sha256 and ripemd160

`sha256` and `ripemd160` are hashes which are used for converting a public point into a Bitcoin address. Addresses in this projects are not common addresses of Bitcoin like `16jY7qLJnxb7CHZyqBP8qca9d51gAjyXQN` but Base58Check-decoded ones like `3EE4133D991F52FDF6A25C9834E0745AC74248A4` (by the way, the address in previous examples is the address of the puzzle). In the project, these hash-functions are highly optimized such that they break its specification rules; for example, SHA256 does not do packing and unpacking to optimize speed.

### test

All the methods from `Key`, `Point`, and functions from `sha256`, and `ripemd160` are tested in `test`. Some of them, which are directly used in brute-forcing keys, are speed-tested. There are time consumptions for the principal operations using 1 thread:
Operation | Time for 1 operation | Usages for 1 key | Time for 1 key | % of total time
--------- | -------------------- | ---------------- | -------------- | ---------------
`sha256` | 1174 ns | 1.0000 | 1174 ns | 24 %
`Point::add` | 2100 ns | 0.5000 | 1050 ns | 22 %
`Key::invertGroup` | 4 100 000 ns | 0.0002 | 1001 ns | 21 %
`Point::subtract` | 2000 ns | 0.5000 | 1000 ns | 21 %
`ripemd160` | 549 ns | 1.0000 | 15 ns | 11 %
`Point::compress` | 15 ns | 1.0000 | 15 ns | 0 %
`Key::operator-=` | 25 ns | 0.5002 | 13 ns | 0 %

For now, it is about 4800 ns needed to check 1 private key using 1 thread. Therefore the total speed is about 800 000 keys per second.

### main

The sought private key consists of 64 bits. In the project, the bits are divided into groups, where Bit # 1 is a higher one:
Bit # 1 | Bits # 2—27 | Bits # 28—29 | Bits # 30—41 | Bits # 42—52 | Bits # 53—64
------- | ----------- | ------------ | ------------ | ------------ | ------------
`1` | block bits | thread bits | progress bits | subblock bits | group bits

The total number of private keys to brute force is 2^63. The whole work is divided into 2^26 = 67 108 864 so called *blocks*, each of them corresponds to *block bits*. Since Raspberry Pi 3B+ has 4 cores, I use 2 bits as *thread bits*, i. e. for the first thread thread bits are `00`, for the second thread thread bits are `01`, etc. When *progress bits* group changes, there prints progress message in Debug configuration. Other bit groups are just being brute-forced.

The compiled program `bitcoin_puzzle` for Linux need an argument with block number in decimal to process. For example, run
```
bitcoin_puzzle 14920854
```

in order to process block # 14920854. With current speed of calculations, it takes about 2 days to process a block, and after this you will see the next output:

```
9C7592C05DC70C7D 3EE4133DC0048754A7B8EB3A085032FADD54E184
9C7592C88B0EE91F 3EE4133D4499172BCD0AD8E6D5DBA9EB316E16DE
9C7592C0FFB209D8 3EE4133D6739DB5AEBF55730BBBB3B1CDB68788A
9C7592C11DA59D3D 3EE4133DBA8E2045A105467C2CD04DDB05E574A0
9C7592D13D39CD7A 3EE4133D426604E0B158D1DE936EF68E55A80D4F
9C7592D944483217 3EE4133D415DEE4E98D12786B20C5BE022178C36
9C7592D1518F88A6 3EE4133DBF268C1B6F5DE23194A0E1E68DF1F4EB
9C7592DA0F75E300 3EE4133D52B968BF92BA11B2EE117CDEBBDB48AD
9C7592D2566158E4 3EE4133D3618F155FFBAD7265330FB1751D6345B
9C7592C2633A61C7 3EE4133D5CA424B8EC9546FC2F56A11CA82E0F48
9C7592D2F15C577C 3EE4133D21AA4AB10B68415578CCB5C822BF182F
9C7592DB4103F871 3EE4133D600E7DAED65DFC20E84806727AC80310
9C7592D346F4F338 3EE4133DEAE3A0A849EBC474919284B1368CFB37
9C7592D3608A7772 3EE4133DEAE1E61B3DD210D2E6B0C4A69F6C59B5
9C7592DBA1C60A00 3EE4133D80144F372A36537A9C3348140A210737
9C7592C3CA63B617 3EE4133DCDF6BB1DF5024C5235A98F85141C0F32
9C7592C3D365C3D3 3EE4133DA18C9E9B34062E3685538D56173B71FB
9C7592DC5F1762E1 3EE4133D02BD487602C9E0DA01D67AA1F5299CF5
9C7592DD205DDAB9 3EE4133D838503E06E8FED9B8CA90E0BD4D9A595
9C7592DD7B363167 3EE4133D2367A74A3D046F14F19FCE197EE56A2F
9C7592C58DD943CF 3EE4133D8D1906F80CF78BB9D9375FF363750B56
9C7592D592804387 3EE4133D9289A366383241A068B535A350AB2885
9C7592CE22C27C33 3EE4133D257CD4E860D907FEDEF03BF594498249
9C7592D67433755C 3EE4133DB73CDD4B3AFD293B610585642519D594
9C7592D679E0D3F1 3EE4133D0AE646058A7236672DF69495FDF0B456
9C7592C6DE2B1726 3EE4133DB8CF52D91908BACB89793FB799EB4B9B
9C7592D6ECB2C5B2 3EE4133D0C01E0C480E845CF25E1F44E699A1E52
9C7592DF05329C3B 3EE4133D7DA19267A7AC733FC31D2EA0CB0779E5
9C7592CF911F3295 3EE4133DDEFF851847B33250BA8303D813422505
9C7592D7F3436F0D 3EE4133DFAB903A9A059EF8E265B5C6AE056CD99
```

The output is a report which contains partially coinciding addresses with private keys (20%-coincidence is enough to get into a report). Each line is a pare of a key and the corresponding address. The program does not check the exact coincidence. Statistically, each block should have about 32 lines in its report.

## Contributions and Donations

I really want to optimize speed as much as possible up to several millions keys in a second. I would appreciate any advice or help. You can use my email to contact me:
```
viktyusk@gmail.com
```

If you want to donate me, please, use the next Bitcoin address:
```
bc1q383qqx90z3kcarpdct5fnz2flzwm3652czhp5v
```
