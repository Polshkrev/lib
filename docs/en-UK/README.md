# lib
Library of single-file stb-like header files for c and c++.

If you would like to revise on how to use a single-file header-only library, then please read the [stb how-to](https://github.com/nothings/stb/blob/master/docs/stb_howto.txt).
## Table of Contents
1. [Usage](#usage)
2. [Changelog](/changelogs/en-UK.md)
3. C
    1. [Version](/docs/en-UK/c/version.md)
    2. [Logger](/docs/en-UK/c/logger.md)
    3. [Flag](/docs/en-UK/c/flag.md)
    4. [Loader](/docs/en-UK/c/loader.md)
    5. Collections
        1. [String Builder](/docs/en-UK/c/collections/string_builder.md)
        2. [String](/docs/en-UK/c/collections/string.md)
        3. [Buffer](/docs/en-UK/c/collections/buffer.md)
    6. Fayl
        1. [Path](/docs/en-UK/c/fayl/path.md)
        2. [Entry](/docs/en-UK/c/fayl/entry.md)
        3. [Files](/docs/en-UK/c/fayl/files.h)
        4. [Directory](/docs/en-UK/c/fayl/directory.md)
4. CPP
    1. [Version](/docs/en-UK/cpp/version.md)
    2. [Exception](/docs/en-UK/cpp/exception.md)
    3. [Logger](/docs/en-UK/cpp/logger.md)
    4. [Loader](/docs/en-UK/c/loader.md)
    5. [Collections](/docs/en-UK/cpp/collections/collections.md)
        1. Interfaces
            1. [Collection](/docs/en-UK/cpp/collections/collection.md)
            2. [Sized](/docs/en-UK/cpp/collections/sized.md)
        2. Structures
            1. [Array](/docs/en-UK/cpp/collections/array.md)
            2. [Dynamic Array](/docs/en-UK/cpp/collections/dynamic_array.md)
    6. Fayl
        1. [Path](/docs/en-UK/cpp/fayl/path.md)
        2. [Entry](/docs/en-UK/cpp/fayl/entry.md)
        3. [Directory](/docs/en-UK/c/fayl/directory.md)
## Usage
A simple &mdash; non-specific &mdash; usage of this library is to, first, copy each of the header files into your project. Then, you can include `lib.h` or `lib.hpp` anywhere in your c or c++ project respectively.
To include a specfic library file in your project, use the include and provide the ```#define <LIBRARY_NAME>_IMPLEMENTATION``` definition.