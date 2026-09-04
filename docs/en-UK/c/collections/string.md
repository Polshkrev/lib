# String
This is inspired by [Tsoding's string view](https://github.com/tsoding/sv) library.

This is a structured, non-null terminated, almost radioactive sized string. Although a heap allocation of the structure is not completely unheard, due to its own nature as a view, the underlying pointer is non-owning, there is no heap allocation or destruction of the structure.

The library also provides basic UTF-8 support.

## Table of Contents
1. [Usage](#usage)
   1. [Construction](#construction)
   2. [Modification](#modification)
   3. [Access](#access)
   4. [UTF-8](#utf-8)

## Usage
To amplify the simplicity of this module, as of the time of writing, none of the functions defined fail.
### Construction
There are several ways to construct a string view.
`string_new()` constructs a string view from a pointer and an explicit byte count:
```c
string_t string = string_new("Hello World", 11);
```
`static_string_new()` can determine the size of a string literal or character array at compile time.
It must not be passed a pointer variable because `sizeof()` would return the size of the pointer rather than the string.
For null-terminated C strings, use `string_from()`.
Unlike `static_string_new()`, `string_from()` accepts any null-terminated C string.
A null string can be represented using `string_null`.
An empty string and a null string are distinct concepts. An empty string has a count of zero, while a null string has both a count of zero and a `NULL` data pointer.

### Modification
The string view itself can be modified without modifying the underlying character data.
`string_trim_left()`, `string_trim_right()`, and `string_trim()` remove leading, trailing, or surrounding whitespace.
These functions only adjust the pointer and count stored in the `string_t`.
`string_chop_by_delimiter()` splits a string view at its first delimiter. The returned view contains everything before the delimiter, while the original view is advanced past it.
If the delimiter is not found, the returned view contains the entire string and the original view becomes empty.

### Access
The underlying data can be accessed through `data`, and its size through `count`. To print a string, first the macro `string_format` will need to be defined, to pass the formatted string, the macro `string_argument` will need to be called:
```c
printf(string_format"\n", string_argument(string));
```
The functions `string_find_first_of()` and `string_find_last_of()` search for a byte and return its byte index.
If the byte cannot be found, `STRING_NOT_FOUND` is returned.
These functions are byte-oriented and do not search for Unicode code points.

The library also provides:
- `string_starts_with()` — checks for a prefix.
- `string_ends_with()` — checks for a suffix.
- `string_equals()` — performs a case-sensitive comparison.
- `string_is_empty()` — checks whether the count is zero.
- `string_is_null()` — checks for `NULL` data with a zero count.

These operations are all byte-oriented.

### UTF-8
The library provides UTF-8 validation and code-point length calculation.
`string_is_valid_utf8()` determines whether the string contains valid UTF-8.
`string_utf8_length()` returns the number of Unicode code points represented by the string.
The important distinction is:
```text
string.count          -> number of bytes
string_utf8_length()  -> number of Unicode code points
```
For example, `Привет` contains 12 bytes but only 6 code points.

UTF-8 validation rejects:
- Invalid continuation bytes.
- Overlong encodings.
- UTF-16 surrogate code points.
- Code points above `U+10FFFF`.
- Truncated sequences.

UTF-8 length counts code points, not user-perceived characters. Combining marks can therefore count as separate code points.
The normal searching and comparison functions remain byte-oriented. This is intentional.
Unicode can be checked when needed, without making every string operation unnecessarily complicated.