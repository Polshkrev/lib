# lib
Library of single-file [stb](https://github.com/nothings/stb)-like header files.

If you would like to revise on how to use a single-file header-only library, then please read the [stb how-to](https://github.com/nothings/stb/blob/master/docs/stb_howto.txt).

## Table of Contents
1. [Usage](#usage)
2. C
    1. IO
        1. [Logger](/docs/en-UK/c/io/logger.md)
    2. Runtime
        1. [Flag](/docs/en-UK/c/runtime/flag.md)
    3. [Path](/docs/en-UK/c/path.md)
    4. [String Builder](/docs/en-UK/c/string-builder.md)
    5. [String](/docs/en-UK/c/string.md)
    6. [Version](/docs/en-UK/c/version.md)
3. CPP
    1. IO
        1. [Logger](/docs/en-UK/cpp/io/logger.md)
    2. Structures
        1. [Version](/docs/en-UK/cpp/structures/version.md)
    3. [Exception](/docs/en-UK/cpp/exception.md)
    4. [Path](/docs/en-UK/cpp/path.md)
## Usage
A simple &mdash; non-specific &mdash; usage of this library is to, first, copy each of the header files into your project. Then, you can include `lib.h` or `lib.hpp` anywhere in your c or c++ project respectively.
To include a specfic library file in your project, use the include and provide the ```#define <LIBRARY_NAME>_IMPLEMENTATION``` definition.