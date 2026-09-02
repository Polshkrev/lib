#ifndef STRING_VIEW_H
#define STRING_VIEW_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stddef.h> // size_t
#include <stdbool.h> // bool

/**
 * @brief Typedef for a single charactor within a string.
 */
typedef int char_t;

/**
 * @brief Definition of a string. Encapsulates a char pointer marked with const along with the length — or "count" — of the data.
 */
typedef struct
{
    size_t count;
    const char *data;
} string_t;

/**
 * @brief Macro to allocate a string whose lifetime is that of the entire programme.
 */
#define static_string_new(cstr_lit)\
{\
    sizeof(cstr_lit) - 1,\
    (cstr_lit)\
}

/**
 * @brief Macro to represent how a null value within the string structure.
 */
#define string_null string_new(NULL, 0)

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
string_t string_from_literal(const char *cstring);

/**
 * @brief Obtain the length of the given string.
 * @param string String from which to obtain the length.
 * @returns The one-based length of the string in charactors.
 */
size_t string_length(const string_t *string);

/**
 * @brief Trim a string of its preceeding spaces.
 * @param string A pointer to the string that needs to be trimmed. It is passed by pointer because it is mutable — or changeable — and shouldn't be localized or passed by value.
 * @returns A copy of the same string passed into the function without its preceeding spaces.
 */
string_t string_trim_left(const string_t *string);

/**
 * @brief Trim a string of its following spaces.
 * @param string A pointer to the string that needs to be trimmed. It is passed by pointer because it is mutable — or changeable — and shouldn't be localized or passed by value.
 * @returns A copy of the same string passed into the function without its following spaces.
 */
string_t string_trim_right(const string_t *string);

/**
 * @brief Trim a string of both its preceeding and following spaces. This function is a combination of both the `string_trim_left` and the `string_trim_right` functions. This function is similar to the default behaviour of Python's `string.strip()` function.
 * @param string A pointer to the string that needs to be trimmed. It is passed by pointer because it is mutable — or changeable — and shouldn't be localized or passed by value.
 * @returns A copy of the same string passed into the function without its preceeding or following spaces.
 */
string_t string_trim(const string_t *string);

/**
 * @brief Chop a string by a given delimetre.
 * @param string A pointer to the string that needs to be trimmed. It is passed by pointer because it is mutable — or changeable — and shouldn't be localized or passed by value.
 * @param delimetre A charactor by which to split the string.
 * @returns A string where the data is set to the first chunk of string before the given delimetre.
 */
string_t string_chop_by_delimetre(string_t *string, char delimetre);

/**
 * @brief Find the first occurance of a given charactor within a given string.
 * @param string A pointer to the string that contains the given charactor. It is passed by pointer because it is mutable — or changeable — and shouldn't be localized or passed by value.
 * @param charactor The charactor to find within the given string.
 * @returns The index of the given charactor within the given string. Much like the `strchr` function, if the charactor is not found within the string, 0 is returned.
 */
size_t string_find_first_of(const string_t *string, char_t charactor);

/**
 * @brief Find the last occurance of a given charactor within a given string.
 * @param string A pointer to the string that contains the given charactor. It is passed by pointer because it is mutable — or changeable — and shouldn't be localized or passed by value.
 * @param charactor The charactor to find within the given string.
 * @returns The index of the given charactor within the given string. Much like the `strrchr` function, if the charactor is not found within the string, 0 is returned.
 */
size_t string_find_last_of(const string_t *string, char_t charactor);

/**
 * @brief Determine whether a given string starts with a given expected string.
 * @param string A pointer to the string that contains the given string. It is passed by pointer because it is mutable — or changeable — and shouldn't be localized or passed by value.
 * @param expected The expected prefix to check against.
 * @returns A boolean on whether `string` starts with `expected`.
 */
bool string_starts_with(const string_t *string, const string_t *expected);

/**
 * @brief Determine whether a given string ends with a given expected string.
 * @param string A pointer to the string that contains the given string. It is passed by pointer because it is mutable — or changeable — and shouldn't be localized or passed by value.
 * @param expected The expected suffix to check against.
 * @returns A boolean on whether `string` ends with `expected`.
 */
bool string_ends_with(const string_t *string, const string_t *expected);

/**
 * @brief Compare two given — case sensitive — string types.
 * @param original The string to check against.
 * @param operand The string to which a comparison will be made in parametre `original`.
 * @returns A boolean on wheather `original` and `operand` are equal.
 */
bool string_equals(const string_t *original, const string_t *operand);

/**
 * @brief Determine if the the given string is empty.
 * @param string String to evaluate.
 * @returns True if the given string has data equal to null and a size of zero.
 */
bool string_is_empty(const string_t *string);

#endif // STRING_VIEW_H

#ifdef STRING_VIEW_IMPLEMENTATION

#if defined(__cplusplus)
extern "C" {
#endif

#include <string.h> // strlen, strchr, strrchr, memcmp, NULL
#include <stdint.h> // uint8_t
#include <ctype.h> // isspace

#ifdef _WIN32
    #include <windef.h> // ! NEEDED FOR CONSOLEAPI2.H
    #include <consoleapi2.h> // SetConsoleOutputCP
    #include <winnls.h> // CP_UTF8
#endif // _WIN32

static const uint8_t __bytes_for_utf8[] = {
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3, 4,4,4,4,4,4,4,4,5,5,5,5,6,6,6,6,
};

size_t __string_utf8_length(const string_t *string, size_t *bytes_overrun)
{
    size_t i = 0;
    size_t n = 0;
    while (true)
    {
        if (i >= string->count)
        {
            if (bytes_overrun) *bytes_overrun = i - string->count;
            return n;
        }
        i += __bytes_for_utf8[(uint8_t)string->data[i]];
        n += 1;
    }
    return 0;
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
 * @brief Obtain the length of the given string.
 * @param string String from which to obtain the length.
 * @returns The one-based length of the string in charactors.
 */
size_t string_length(const string_t *string)
{
    return __string_utf8_length(string, NULL);
}

/**
 * @brief Allocate a new string structure from a c-string literal marked with const.
 * @param cstring A char pointer marked with const to be passed to the `new_string` function that was previously defined.
 * @returns A new string with data as the given cstr and the `strlen` count.
 */
string_t string_from_literal(const char *cstring)
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif // _WIN32
    return string_new(cstring, strlen(cstring));
}

/**
 * @brief Trim a string of its preceeding spaces.
 * @param string A pointer to the string that needs to be trimmed. It is passed by pointer because it is mutable — or changeable — and shouldn't be localized or passed by value.
 * @returns A copy of the same string passed into the function without its preceeding spaces.
 */
string_t string_trim_left(const string_t *string)
{
    size_t index = 0;
    while (index < string->count && isspace(string->data[index]))
    {
        index++;
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
    while (index < string->count && isspace(string->data[string->count - 1 - index]))
    {
        index++;
    }
    return string_new(string->data, string->count - index);
}

/**
 * @brief Trim a string of both its preceeding and following spaces. This function is a combination of both the `string_trim_left` and the `string_trim_right` functions. This function is similar to the default behaviour of Python's `string.strip()` function.
 * @param string A pointer to the string that needs to be trimmed. It is passed by pointer because it is mutable — or changeable — and shouldn't be localized or passed by value.
 * @returns A copy of the same string passed into the function without its preceeding or following spaces.
 */
string_t string_trim(const string_t *string)
{
    string_t left_trim = string_trim_left(string);
    return string_trim_right(&left_trim);
}

/**
 * @brief Chop a string by a given delimetre.
 * @param string A pointer to the string that needs to be trimmed. It is passed by pointer because it is mutable — or changeable — and shouldn't be localized or passed by value.
 * @param delimetre A charactor by which to split the string.
 * @returns A string where the data is set to the first chunk of string before the given delimetre.
 */
string_t string_chop_by_delimetre(string_t *string, char delimetre)
{
    size_t index = 0;
    while (index < string->count && string->data[index] != delimetre)
    {
        index++;
    }
    string_t result = string_new(string->data, index);
    if (index < string->count)
    {
        string->count -= index + 1;
        string->data += index + 1;
    }
    else
    {
        string->count -= index;
        string->data += index;
    }
    return result;
}

/**
 * @brief Find the first occurance of a given charactor within a given string.
 * @param string A pointer to the string that contains the given charactor. It is passed by pointer because it is mutable — or changeable — and shouldn't be localized or passed by value.
 * @param charactor The charactor to find within the given string.
 * @returns The index of the given charactor within the given string. Much like the `strchr` function, if the charactor is not found within the string, 0 is returned.
 */
size_t string_find_first_of(const string_t *string, char_t charactor)
{
    const char *result = strchr(string->data, charactor);
    size_t full_length = string_length(string) - 1;
    if (!result)
    {
        return 0;
    }
    return full_length - strlen(result);
}

/**
 * @brief Find the last occurance of a given charactor within a given string.
 * @param string A pointer to the string that contains the given charactor. It is passed by pointer because it is mutable — or changeable — and shouldn't be localized or passed by value.
 * @param charactor The charactor to find within the given string.
 * @returns The index of the given charactor within the given string. Much like the `strrchr` function, if the charactor is not found within the string, 0 is returned.
 */
size_t string_find_last_of(const string_t *string, char_t charactor)
{
    const char *result = strrchr(string->data, charactor);
    size_t full_length = string_length(string) - 1;
    if (!result)
    {
        return 0;
    }
    return full_length - strlen(result);
}

/**
 * @brief Determine whether a given string starts with a given expected string.
 * @param string A pointer to the string that contains the given string. It is passed by pointer because it is mutable — or changeable — and shouldn't be localized or passed by value.
 * @param expected The expected prefix to check against.
 * @returns A boolean on whether `string` starts with `expected`.
 */
bool string_starts_with(const string_t *string, const string_t *expected)
{
    if (expected->count > string->count)
    {
        return false;
    }
    string_t actual = string_new(string->data, expected->count);
    return string_equals(expected, &actual);
}

/**
 * @brief Determine whether a given string ends with a given expected string.
 * @param string A pointer to the string that contains the given string. It is passed by pointer because it is mutable — or changeable — and shouldn't be localized or passed by value.
 * @param expected The expected suffix to check against.
 * @returns A boolean on whether `string` ends with `expected`.
 */
bool string_ends_with(const string_t *string, const string_t *expected)
{
    if (expected->count > string->count)
    {
        return false;
    }
    string_t actual = string_new(string->data + string->count - expected->count, expected->count);
    return string_equals(expected, &actual);
}

/**
 * @brief Compare two given — case sensitive — string types.
 * @param original The string to check against.
 * @param operand The string to which a comparison will be made in parametre `original`.
 * @returns A boolean on wheather `original` and `operand` are equal.
 */
bool string_equals(const string_t *original, const string_t *operand)
{
    if (original->count != operand->count)
    {
        return false;
    }
    return memcmp(original->data, operand->data, original->count) == 0;
}

/**
 * @brief Determine if the the given string is empty.
 * @param string String to evaluate.
 * @returns True if the given string has data equal to null and a size of zero.
 */
bool string_is_empty(const string_t *string)
{
    string_t null = string_null;
    return string_equals(string, &null);
}

#if defined(__cplusplus)
}
#endif

#endif // STRING_VIEW_IMPLEMENTATION