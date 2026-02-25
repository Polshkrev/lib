# String
This is inspired by [Tsoding's string view](https://github.com/tsoding/sv) library.

This is a structured, non-null terminated, almost radioactive sized string. Although a heap allocation of the structure is not completely unheard, due to its own nature as a view, the underlying pointer is non-owning, there is no heap allocation or destruction of the structure.
## Table of Contents
1. [Usage](#usage)
    1. [Construction](#construction)
    2. [Modification](#modification)
    3. [Access](#access)
## Usage
To amplify the simplicity of this module, as of the time of writing, none of the functions defined fail.
### Construction
To construct a new string view there are a few functions defined. Keeping with the convention of this library, to construct a new string the `string_new` function is defined. There is also a `static_string_new` macro, and a self-explanatory `string_from_literal` function.
### Modification
To modify a string view, a few functions and macros are defined. Most of the functions and macros are self-explanatory such as: `string_null` which null terminates the string, `string_trim_[left,right]` which directionally trims the string, `string_trim` which trims the string in both directions, and most usefully, `string_chop_by_delimetre` which chops a string by a given delimetre.
### Access
To access the data within the string view, there are a few functions defined. Defined are a few functions such as `string_find_[first,last]_of` which return the index in the form of a `size_t` where the given charactor respectively occurs first or last.
To print a string view, there are two functions that are defined to be used in conjuntion with each other. A `printf` example would look like this: `printf(string_format"\n", string_arg(string_from_literal("Hello World")));`
There are also some compare functions defined such as: `string_[starts,ends]_with`, and `string_equals` which each take in two string views and return a boolean.