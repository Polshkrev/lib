# Path
A C++ implementation of [Python's Pathlib](https://docs.python.org/3/library/pathlib.html).
## Table Of Contents
1. [Usage](#usage)
    1. [Construction](#construction)
    2. [Modification](#modification)
## Usage
### Construction
To construct a new path object, there are two constructor overloads you can use: A default constructor, and a constructor that takes a raw path as a parametre.
### Modification
To modify a path, the `/` operator overload is defined in two ways. One that takes another path, and one that takes a string.