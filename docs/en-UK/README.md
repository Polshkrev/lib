# lib
Library of single-file stb-like header files for c and c++.

If you would like to revise on how to use a single-file header-only library, then please read the [stb how-to](https://github.com/nothings/stb/blob/master/docs/stb_howto.txt).

## Table of Contents
1. [Usage](#usage)
2. C
    1. [Version](/docs/en-UK/c/version.md)
3. CPP
    1. [Version](/docs/en-UK/cpp/version.md)
    2. [Exception](/docs/en-UK/cpp/exception.md)

## Usage
A simple &mdash; non-specific &mdash; usage of this library is to, first, copy each of the header files into your project. Then, you can include `lib.h` or `lib.hpp` anywhere in your c or c++ project respectively.
To include a specfic library file in your project, use the include and provide the ```#define <LIBRARY_NAME>_IMPLEMENTATION``` definition.