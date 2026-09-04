#ifndef STRING_VIEW_H
#define STRING_VIEW_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stddef.h>  // size_t, NULL
#include <stdbool.h> // bool
#include <stdint.h>  // uint8_t, SIZE_MAX

/**
 * @brief Typedef for a single character within a string.
 */
typedef unsigned char char_t;

/**
 * @brief Definition of a string. Encapsulates a char pointer marked with const along with the length — or "count" — of the data.
 */
typedef struct
{
    size_t count;
    const char *data;
} string_t;

/**
 * @brief Create a string view over a string literal.
 * @param cstring A string literal.
 * @returns A string view whose lifetime is that of the string literal.
 * @warning This macro must only be used with a string literal or character array whose size includes the terminating null byte.
 */
#define static_string_new(cstring)\
{\
    sizeof(cstring) - 1,\
    (cstring)\
}

/**
 * @brief Representation of a null string.
 */
#define string_null string_new(NULL, 0)

/**
 * @brief Representation of a character that could not be found.
 */
#define STRING_NOT_FOUND SIZE_MAX

/**
 * @brief Custom formatting for printing the string structure.
 */
#define string_format "%.*s"

/**
 * @brief Format expansion for a string.
 * @param string Sized string to expand.
 */
#define string_argument(string) (int) (string).count, (string).data

/**
 * @brief Constructor for a new string with a given length and data.
 * @param data A char pointer marked with const that is directly added to the structure.
 * @param count The provided length of the given data.
 * @returns A new string with a given count and data.
 */
string_t string_new(const char *data, size_t count);

/**
 * @brief Allocate a new string structure from a c-string literal marked with const.
 * @param cstring A char pointer marked with const to be passed to the `new_string` function that was previously defined.
 * @returns A new string with data as the given cstring and the `strlen` count.
 */
string_t string_from(const char *cstring);

/**
 * @brief Obtain the number of Unicode code points represented by the string. Valid UTF-8 sequences are counted as one code point. Invalid UTF-8 bytes are counted individually.
 * @param string String from which to obtain the length.
 * @returns The number of Unicode code points represented by the string.
 */
size_t string_utf8_length(const string_t *string);

/**
 * @brief Determine whether the given string contains valid utf-8.
 * @param string String to validate.
 * @returns True if the string contains valid utf-8.
 */
bool string_is_valid_utf8(const string_t *string);

/**
 * @brief Trim a string of its preceding spaces.
 * @param string A pointer to the string that needs to be trimmed. It is passed by pointer because it is mutable — or changeable — and shouldn't be localized or passed by value.
 * @returns A copy of the same string passed into the function without its preceding spaces.
 */
string_t string_trim_left(const string_t *string);

/**
 * @brief Trim a string of its following spaces.
 * @param string A pointer to the string that needs to be trimmed. It is passed by pointer because it is mutable — or changeable — and shouldn't be localized or passed by value.
 * @returns A copy of the same string passed into the function without its following spaces.
 */
string_t string_trim_right(const string_t *string);

/**
 * @brief Trim a string of both its preceding and following spaces. This function is a combination of both the `string_trim_left` and the `string_trim_right` functions. This function is similar to the default behaviour of Python's `string.strip()` function.
 * @param string A pointer to the string that needs to be trimmed. It is passed by pointer because it is mutable — or changeable — and shouldn't be localized or passed by value.
 * @returns A copy of the same string passed into the function without its preceding or following spaces.
 */
string_t string_trim(const string_t *string);

/**
 * @brief Chop a string by a given delimiter.
 * @param string A pointer to the string that needs to be trimmed. It is passed by pointer because it is mutable — or changeable — and shouldn't be localized or passed by value.
 * @param delimiter A character by which to split the string.
 * @returns A string where the data is set to the first chunk of string before the given delimiter.
 */
string_t string_chop_by_delimiter(string_t *string, char delimiter);

/**
 * @brief Find the first occurrence of a given character within a given string.
 * @param string A pointer to the string that contains the given character. It is passed by pointer because it is mutable — or changeable — and shouldn't be localized or passed by value.
 * @param character The character to find within the string.
 * @returns The index of the given character within the string. If the character is not found within the string, `STRING_NOT_FOUND` is returned.
 */
size_t string_find_first_of(const string_t *string, char_t character);

/**
 * @brief Find the last occurrence of a given character within a given string.
 * @param string A pointer to the string that contains the given character. It is passed by pointer because it is mutable — or changeable — and shouldn't be localized or passed by value.
 * @param character The character to find within the string.
 * @returns The index of the given character within the string. If the character is not found within the string, `STRING_NOT_FOUND` is returned.
 */
size_t string_find_last_of(const string_t *string, char_t character);

/**
 * @brief Determine whether a given string starts with a given expected string.
 * @param string A pointer to the string that contains the given string.
 * @param expected The expected prefix to check against.
 * @returns A boolean on whether `string` starts with `expected`.
 */
bool string_starts_with(const string_t *string, const string_t *expected);

/**
 * @brief Determine whether a given string ends with a given expected string.
 * @param string A pointer to the string that contains the given string.
 * @param expected The expected suffix to check against.
 * @returns A boolean on whether `string` ends with `expected`.
 */
bool string_ends_with(const string_t *string, const string_t *expected);

/**
 * @brief Compare two given — case sensitive — string types.
 * @param original The string to check against.
 * @param operand The string to which a comparison will be made in parameter `original`.
 * @returns A boolean on whether `original` and `operand` are equal.
 */
bool string_equals(const string_t *original, const string_t *operand);

/**
 * @brief Determine if the the given string is empty.
 * @param string String to evaluate.
 * @returns True if the given string has a count of zero.
 */
bool string_is_empty(const string_t *string);

/**
 * @brief Determine if the given string represents a null string.
 * @param string String to evaluate.
 * @returns True if the given string has null data and a count of zero.
 */
bool string_is_null(const string_t *string);

#if defined(__cplusplus)
}
#endif

#endif // STRING_VIEW_H

#ifdef STRING_VIEW_IMPLEMENTATION

#if defined(__cplusplus)
extern "C" {
#endif

#include <string.h> // strlen, memcmp
#include <ctype.h>  // isspace

static const uint8_t __bytes_for_utf8[] =
{
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,

    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,

    0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,

    3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,

    4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0
};

/**
 * @brief Determine whether a byte is a utf-8 continuation byte.
 * @param character The byte to evaluate.
 * @returns True if the byte is a utf-8 continuation byte.
 */
static bool __string_utf8_is_continuation(uint8_t character)
{
    return character >= 0x80 && character <= 0xBF;
}

/**
 * @brief Determine whether a utf-8 sequence is valid.
 * @param data The beginning of the utf-8 sequence.
 * @param count The number of bytes available.
 * @param sequence_length The expected length of the sequence.
 * @returns True if the utf-8 sequence is valid.
 */
static bool __string_utf8_sequence_is_valid(const char *data, size_t count, size_t sequence_length)
{
    if ((sequence_length == 0 || sequence_length > 4) || (count < sequence_length))
    {
        return false;
    }

    uint8_t first = (uint8_t) data[0];

    if ((sequence_length == 1) || (sequence_length == 2))
    {
        return true;
    }
    else if (!__string_utf8_is_continuation((uint8_t) data[1]) ||
             (first == 0xE0 && (uint8_t) data[1] < 0xA0) ||
             (first == 0xED && (uint8_t) data[1] > 0x9F) ||
             (first == 0xF0 && (uint8_t) data[1] < 0x90) ||
             (first == 0xF4 && (uint8_t) data[1] > 0x8F))
    {
        return false;
    }

    for (size_t index = 2; index < sequence_length; index++)
    {
        if (__string_utf8_is_continuation((uint8_t) data[index]))
        {
            continue;
        }
        return false;
    }

    return true;
}

/**
 * @brief Constructor for a new string with a given length and data.
 * @param data A char pointer marked with const that is directly added to the structure.
 * @param count The provided length of the given data.
 * @returns A new string with a given count and data.
 */
string_t string_new(const char *data, size_t count)
{
    string_t string = {0};
    string.count = count;
    string.data = data;
    return string;
}

/**
 * @brief Obtain the number of Unicode code points represented by the string. Valid UTF-8 sequences are counted as one code point. Invalid UTF-8 bytes are counted individually.
 * @param string String from which to obtain the length.
 * @returns The number of Unicode code points represented by the string.
 */
size_t string_utf8_length(const string_t *string)
{
    size_t index = 0;
    size_t count = 0;

    while (index < string->count)
    {
        uint8_t character = (uint8_t) string->data[index];
        size_t sequence_length = __bytes_for_utf8[character];

        if ((sequence_length == 0) || (!__string_utf8_sequence_is_valid(string->data + index, string->count - index, sequence_length)))
        {
            index++;
            count++;
            continue;
        }
        index += sequence_length;
        count++;
    }
    return count;
}

/**
 * @brief Determine whether the given string contains valid utf-8.
 * @param string String to validate.
 * @returns True if the string contains valid utf-8.
 */
bool string_is_valid_utf8(const string_t *string)
{
    size_t index = 0;

    while (index < string->count)
    {
        uint8_t character = (uint8_t) string->data[index];
        size_t sequence_length = __bytes_for_utf8[character];

        if ((sequence_length == 0) || (!__string_utf8_sequence_is_valid(string->data + index, string->count - index, sequence_length)))
        {
            return false;
        }
        index += sequence_length;
    }
    return true;
}

/**
 * @brief Allocate a new string structure from a c-string literal marked with const.
 * @param cstring A char pointer marked with const to be passed to the `new_string` function that was previously defined.
 * @returns A new string with data as the given cstring and the `strlen` count.
 */
string_t string_from(const char *cstring)
{
    if (!cstring)
    {
        return string_null;
    }
    return string_new(cstring, strlen(cstring));
}

/**
 * @brief Trim a string of its preceding spaces.
 * @param string A pointer to the string that needs to be trimmed. It is passed by pointer because it is mutable — or changeable — and shouldn't be localized or passed by value.
 * @returns A copy of the same string passed into the function without its preceding spaces.
 */
string_t string_trim_left(const string_t *string)
{
    size_t index = 0;
    while (index < string->count && isspace((unsigned char) string->data[index]))
    {
        index++;
    }
    if (index == 0)
    {
        return *string;
    }
    return string_new(string->data + index, string->count - index);
}

/**
 * @brief Trim a string of its following spaces.
 * @param string A pointer to the string that needs to be trimmed. It is passed by pointer because it is mutable — or changeable — and shouldn't be localized or passed by value.
 * @returns A copy of the same string passed into the function without its following spaces.
 */
string_t string_trim_right(const string_t *string)
{
    size_t index = 0;
    while (index < string->count && isspace((unsigned char) string->data[string->count - 1 - index]))
    {
        index++;
    }
    return string_new(string->data, string->count - index);
}

/**
 * @brief Trim a string of both its preceding and following spaces. This function is a combination of both the `string_trim_left` and the `string_trim_right` functions. This function is similar to the default behaviour of Python's `string.strip()` function.
 * @param string A pointer to the string that needs to be trimmed. It is passed by pointer because it is mutable — or changeable — and shouldn't be localized or passed by value.
 * @returns A copy of the same string passed into the function without its preceding or following spaces.
 */
string_t string_trim(const string_t *string)
{
    string_t left_trim = string_trim_left(string);
    return string_trim_right(&left_trim);
}

/**
 * @brief Chop a string by a given delimiter.
 * @param string A pointer to the string that needs to be trimmed. It is passed by pointer because it is mutable — or changeable — and shouldn't be localized or passed by value.
 * @param delimiter A character by which to split the string.
 * @returns A string where the data is set to the first chunk of string before the given delimiter.
 */
string_t string_chop_by_delimiter(string_t *string, char delimiter)
{
    size_t index = 0;
    while (index < string->count && string->data[index] != delimiter)
    {
        index++;
    }
    string_t result = string_new(string->data, index);
    if (index < string->count)
    {
        string->data += index + 1;
        string->count -= index + 1;
    }
    else
    {
        string->count = 0;
    }
    return result;
}

/**
 * @brief Find the first occurrence of a given character within a given string.
 * @param string A pointer to the string that contains the given character. It is passed by pointer because it is mutable — or changeable — and shouldn't be localized or passed by value.
 * @param character The character to find within the string.
 * @returns The index of the given character within the string. If the character is not found within the string, `STRING_NOT_FOUND` is returned.
 */
size_t string_find_first_of(const string_t *string, char_t character)
{
    for (size_t index = 0; index < string->count; index++)
    {
        if ((unsigned char) string->data[index] == (unsigned char) character)
        {
            return index;
        }
    }
    return STRING_NOT_FOUND;
}

/**
 * @brief Find the last occurrence of a given character within a given string.
 * @param string A pointer to the string that contains the given character. It is passed by pointer because it is mutable — or changeable — and shouldn't be localized or passed by value.
 * @param character The character to find within the string.
 * @returns The index of the given character within the string. If the character is not found within the string, `STRING_NOT_FOUND` is returned.
 */
size_t string_find_last_of(const string_t *string, char_t character)
{
    if (string->count == 0)
    {
        return STRING_NOT_FOUND;
    }

    for (size_t index = string->count; index > 0; index--)
    {
        if ((unsigned char) string->data[index - 1] != (unsigned char) character)
        {
            continue;
        }
        return index - 1;
    }

    return STRING_NOT_FOUND;
}

/**
 * @brief Determine whether a given string starts with a given expected string.
 * @param string A pointer to the string that contains the given string.
 * @param expected The expected prefix to check against.
 * @returns A boolean on whether `string` starts with `expected`.
 */
bool string_starts_with(const string_t *string, const string_t *expected)
{
    if (expected->count > string->count)
    {
        return false;
    }
    else if (string_is_empty(expected))
    {
        return true;
    }
    return memcmp(string->data, expected->data, expected->count) == 0;
}

/**
 * @brief Determine whether a given string ends with a given expected string.
 * @param string A pointer to the string that contains the given string.
 * @param expected The expected suffix to check against.
 * @returns A boolean on whether `string` ends with `expected`.
 */
bool string_ends_with(const string_t *string, const string_t *expected)
{
    if (expected->count > string->count)
    {
        return false;
    }
    else if (string_is_empty(expected))
    {
        return true;
    }
    return memcmp(string->data + string->count - expected->count, expected->data, expected->count) == 0;
}

/**
 * @brief Compare two given — case sensitive — string types.
 * @param original The string to check against.
 * @param operand The string to which a comparison will be made in parameter `original`.
 * @returns A boolean on whether `original` and `operand` are equal.
 */
bool string_equals(const string_t *original, const string_t *operand)
{
    if (original->count != operand->count)
    {
        return false;
    }
    else if (string_is_empty(original))
    {
        return true;
    }
    return memcmp(original->data, operand->data, original->count) == 0;
}

/**
 * @brief Determine if the the given string is empty.
 * @param string String to evaluate.
 * @returns True if the given string has a count of zero.
 */
bool string_is_empty(const string_t *string)
{
    return string->count == 0;
}

/**
 * @brief Determine if the given string represents a null string.
 * @param string String to evaluate.
 * @returns True if the given string has null data and a count of zero.
 */
bool string_is_null(const string_t *string)
{
    return string->data == NULL && string->count == 0;
}

#if defined(__cplusplus)
}
#endif

#endif // STRING_VIEW_IMPLEMENTATION