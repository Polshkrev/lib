# Path
A C implementation of [Python's Pathlib](https://docs.python.org/3/library/pathlib.html).

## Table Of Contents
1. [Usage](#usage)
    1. [Construction](#construction)
    2. [Modification](#modification)
    3. [Destruction](#destruction)
## Usage
### Construction
To construct a new path object, there are two functions you can use: `path_init` and `path_from`. As a rule of thumb, any function that returns a `path_t`, can assumed to return a new heap allocated path.
### Modification
To modify a path, three functions are defined: `path_append` which appends two given paths to each other, `path_append_as` which appends a c-string to a path, and `path_append_to` which appends a path to a c-string.
### Destruction
Due to the fact that a path is heap allocated, the function `path_delete` will need to be called to deallocate the path.