#ifndef STRING_BUILDER_H_
#define STRING_BUILDER_H_

#if defined (__cplusplus)
extern "C" {
#endif

#include <stddef.h> // size_t
#include <stdbool.h> // bool

/**
 * @brief Determine the minimum value between two given numbers.
 * @param a First value to check.
 * @param b Second value to check.
 * @returns `a` if `a` is less than `b`, else `b`.
 */
#define MIN(a, b) ((a) < (b) ? (a) : (b))

/**
 * @brief Determine the maximum value between two given numbers.
 * @param a First value to check.
 * @param b Second value to check.
 * @returns `a` if `a` is more than `b`, else `b`.
 */
#define MAX(a, b) ((a) > (b) ? (a) : (b))

/**
 * @brief Clamp a given value to a given minimum or maximum value.
 * @param value Value to clamp.
 * @param minimum Minimum value to check.
 * @param maximum Maximum value to check.
 * @returns The given value clamped to either the minimum or maximum.
 */
#define CLAMP(value, minimum, maximum) (MIN(MAX((value), (minimum)), (maximum)))

#define STRING_VIEW_IMPLEMENTATION
#include "./string_view.h" // string_t, string_new

/**
 * @brief A dynamic buffer of characters.
 */
typedef struct
{
    char *items;
    size_t size;
    size_t capacity;
} string_builder_t;

/**
 * @brief Construct a new dynamic buffer of characters.
 * @returns A new dynamic buffer of characters.
 * @exception If the underlying array can not be allocated, an `AllocationError` is printed to `stderr` and the programme exits.
 */
string_builder_t string_builder_init(void);

/**
 * @brief Construct a new dynamic buffer of characters with a given capacity.
 * @param capacity Initial capacity of the buffer. A value of zero is changed to one.
 * @returns A new dynamic buffer of characters with a given initial capacity.
 * @exception If the underlying array can not be allocated, an `AllocationError` is printed to `stderr` and the programme exits.
 */
string_builder_t string_builder_with_capacity(size_t capacity);

/**
 * @brief Append a character to the buffer of characters. If the buffer is full, the buffer is resized by an exponential factor of two.
 * @param builder Buffer of characters to which to append.
 * @param item Item to append to the buffer.
 * @exception If the builder can not be reallocated, an `AllocationError` is printed to `stderr` and the programme exits after the given builder is deallocated.
 */
void string_builder_append(string_builder_t *builder, char item);

/**
 * @brief Append a given number of bytes to the buffer.
 * @param builder Buffer of characters to which to append.
 * @param items Bytes to append to the buffer.
 * @param count Number of bytes to append.
 * @exception If the given items are `NULL`, an `IllegalParameterError` is printed to `stderr` and the programme exits.
 * @exception If the builder can not be reallocated, an `AllocationError` is printed to `stderr` and the programme exits after the given builder is deallocated.
 */
void string_builder_append_data(string_builder_t *builder, const char *items, size_t count);

/**
 * @brief Append a null-terminated string to the buffer of characters not including the aforementioned null byte.
 * @param builder Buffer of characters to which to append.
 * @param items Null-terminated string from which to append to the buffer.
 * @exception If the given items are `NULL`, an `IllegalParameterError` is printed to `stderr` and the programme exits after the given builder is deallocated.
 * @exception If the builder can not be reallocated, an `AllocationError` is printed to `stderr` and the programme exits after the given builder is deallocated.
 */
void string_builder_extend(string_builder_t *builder, const char *items);

/**
 * @brief Obtain a pointer to a character in the buffer at a given index.
 * @param builder Buffer from which to access.
 * @param index Index within the buffer where the data is located.
 * @returns A pointer to the data within the given buffer at the given index.
 * @exception If the given index is greater than or equal to the size of the buffer, an `IndexError` is printed to `stderr` and the programme exits.
 */
char *string_builder_at(const string_builder_t *builder, size_t index);

/**
 * @brief Find a specified character within the builder.
 * @param builder Builder within which to search.
 * @param character Character for which to search.
 * @returns The index within the builder where the given character is stored.
 * @returns If the character can not be found, negative one is returned.
 */
ptrdiff_t string_builder_find(const string_builder_t *builder, char character);

/**
 * @brief Obtain the items within the builder.
 * @param builder Builder from which to obtain the items.
 * @returns The underlying items within the builder without appending a null byte.
 */
char *string_builder_items(const string_builder_t *builder);

/**
 * @brief Obtain a null-terminated c-string consisting of the items within the buffer.
 * @param builder Buffer from which to obtain the data.
 * @returns A null-terminated c-string consisting of the data within the array.
 * @exception If the builder can not be reallocated, an `AllocationError` is printed to `stderr` and the programme exits after the builder is deallocated.
 */
const char *string_builder_data(string_builder_t *builder);

/**
 * @brief Fit the capacity of the builder to its size.
 * @param builder Builder to fit.
 * @exception If the builder can not be reallocated, an `AllocationError` is printed to `stderr` and the programme exits after the builder is deallocated.
 */
void string_builder_fit(string_builder_t *builder);

/**
 * @brief Append the contents of one string builder to another.
 * @param destination Builder to which the contents are appended.
 * @param source Builder from which the contents are obtained.
 * @exception If the destination builder can not be reallocated, an `AllocationError` is printed to `stderr` and the programme exits after the destination builder is deallocated.
 */
void string_builder_combine(string_builder_t *destination, const string_builder_t *source);

/**
 * @brief Obtain a view into the string builder.
 * @param builder Buffer from which to obtain the view.
 * @returns A view into the string builder.
 */
string_t sbtosv(const string_builder_t *builder);

/**
 * @brief Remove an element from the buffer at a given index.
 * @param builder Buffer from which to remove an element.
 * @param index Index at which the removable element is located.
 * @exception If the buffer is empty, a `ValueError` is printed to `stderr` and the programme exits after the given builder is deallocated.
 * @exception If the given index is greater than or equal to the size of the buffer, an `IndexError` is printed to `stderr` and the programme exits after the given builder is deallocated.
 */
void string_builder_remove(string_builder_t *builder, size_t index);

/**
 * @brief Resize the buffer by an exponential factor of two.
 * @param builder Buffer to resize.
 * @exception If the builder can not be reallocated, an `AllocationError` is printed to `stderr` and the programme exits after the given builder is deallocated.
 */
void string_builder_resize(string_builder_t *builder);

/**
 * @brief Resize the buffer by a given exponential factor.
 * @param builder Buffer to resize.
 * @param scaler Exponential scaler by which to resize the buffer. Values less than `two` cause no operation.
 * @exception If the resulting capacity would overflow `size_t`, an `OverflowError` is printed to `stderr` and the programme exits after the given builder is deallocated.
 * @exception If the builder can not be reallocated, an `AllocationError` is printed to `stderr` and the programme exits after the given builder is deallocated.
 */
void string_builder_resize_by(string_builder_t *builder, size_t scaler);

/**
 * @brief Determine if the buffer is empty.
 * @param builder Buffer which is evaluated.
 * @returns If the buffer is evaluated to be empty, `true` is returned, else `false`.
 */
bool string_builder_empty(const string_builder_t *builder);

/**
 * @brief Deallocate the buffer.
 * @param builder Buffer which to deallocate.
 */
void string_builder_delete(string_builder_t *builder);

#if defined(__cplusplus)
}
#endif

#endif // STRING_BUILDER_H_

#ifdef STRING_BUILDER_IMPLEMENTATION

#if defined (__cplusplus)
extern "C" {
#endif

#include <stdio.h> // fprintf, stderr
#include <stdlib.h> // malloc, realloc, free, exit, NULL
#include <string.h> // strlen
#include <stdint.h> // SIZE_MAX

#ifndef STRING_BUILDER_INITIAL_CAPACITY
#define STRING_BUILDER_INITIAL_CAPACITY 256
#endif // STRING_BUILDER_INITIAL_CAPACITY

/**
 * @brief Construct a new dynamic buffer of characters.
 * @returns A new dynamic buffer of characters.
 * @exception If the underlying array can not be allocated, an `AllocationError` is printed to `stderr` and the programme exits.
 */
string_builder_t string_builder_init(void)
{
    return string_builder_with_capacity(STRING_BUILDER_INITIAL_CAPACITY);
}

/**
 * @brief Construct a new dynamic buffer of characters with a given capacity.
 * @param capacity Initial capacity of the buffer. A value of zero is changed to one.
 * @returns A new dynamic buffer of characters with a given initial capacity.
 * @exception If the underlying array can not be allocated, an `AllocationError` is printed to `stderr` and the programme exits.
 */
string_builder_t string_builder_with_capacity(size_t capacity)
{
    size_t clamped_capacity = CLAMP(capacity, 1, SIZE_MAX);
    char *items = (char *)malloc(clamped_capacity * sizeof(char));
    if (NULL == items)
    {
        fprintf(stderr, "AllocationError: Can not allocate enough memory for the array of characters.\n");
        exit(1);
    }
    return (string_builder_t)
    {
        .size = 0,
        .items = items,
        .capacity = clamped_capacity,
    };
}

/**
 * @brief Append a character to the buffer of characters. If the buffer is full, the buffer is resized by an exponential factor of two.
 * @param builder Buffer of characters to which to append.
 * @param item Item to append to the buffer.
 * @exception If the builder can not be reallocated, an `AllocationError` is printed to `stderr` and the programme exits after the given builder is deallocated.
 */
void string_builder_append(string_builder_t *builder, char item)
{
    if (builder->size >= builder->capacity)
    {
        string_builder_resize(builder);
    }
    builder->items[builder->size++] = item;
}

/**
 * @brief Append a given number of bytes to the buffer.
 * @param builder Buffer of characters to which to append.
 * @param items Bytes to append to the buffer.
 * @param count Number of bytes to append.
 * @exception If the given items are `NULL`, an `IllegalParameterError` is printed to `stderr` and the programme exits.
 * @exception If the builder can not be reallocated, an `AllocationError` is printed to `stderr` and the programme exits after the given builder is deallocated.
 */
void string_builder_append_data(string_builder_t *builder, const char *items, size_t count)
{
    if (NULL == items)
    {
        fprintf(stderr, "IllegalParameterError: items can not be NULL.\n");
        exit(1);
    }

    for (size_t i = 0; i < count; ++i)
    {
        string_builder_append(builder, items[i]);
    }
}

/**
 * @brief Append a null-terminated string to the buffer of characters not including the aforementioned null byte.
 * @param builder Buffer of characters to which to append.
 * @param items Null-terminated string from which to append to the buffer.
 * @exception If the given items are `NULL`, an `IllegalParameterError` is printed to `stderr` and the programme exits after the given builder is deallocated.
 * @exception If the builder can not be reallocated, an `AllocationError` is printed to `stderr` and the programme exits after the given builder is deallocated.
 */
void string_builder_extend(string_builder_t *builder, const char *items)
{
    if (NULL == items)
    {
        fprintf(stderr, "IllegalParameterError: items can not be NULL.\n");
        exit(1);
    }
    string_builder_append_data(builder, items, strlen(items));
}

/**
 * @brief Obtain a pointer to a character in the buffer at a given index.
 * @param builder Buffer from which to access.
 * @param index Index within the buffer where the data is located.
 * @returns A pointer to the data within the given buffer at the given index.
 * @exception If the given index is greater than or equal to the size of the buffer, an `IndexError` is printed to `stderr` and the programme exits.
 */
char *string_builder_at(const string_builder_t *builder, size_t index)
{
    if (index >= builder->size)
    {
        fprintf(stderr, "IndexError: Can not access array of size %zu at index %zu.\n", builder->size, index);
        exit(1);
    }
    return &builder->items[index];
}

/**
 * @brief Find a specified character within the builder.
 * @param builder Builder within which to search.
 * @param character Character for which to search.
 * @returns The index within the builder where the given character is stored.
 * @returns If the character can not be found, negative one is returned.
 */
ptrdiff_t string_builder_find(const string_builder_t *builder, char character)
{
    for (size_t i = 0; i < builder->size; ++i)
    {
        if (builder->items[i] != character) continue;
        return (ptrdiff_t)i;
    }
    return -1;
}

/**
 * @brief Obtain the items within the builder.
 * @param builder Builder from which to obtain the items.
 * @returns The underlying items within the builder without appending a null byte.
 */
char *string_builder_items(const string_builder_t *builder)
{
    return builder->items;
}

/**
 * @brief Obtain a null-terminated c-string consisting of the items within the buffer.
 * @param builder Buffer from which to obtain the data.
 * @returns A null-terminated c-string consisting of the data within the array.
 * @exception If the builder can not be reallocated, an `AllocationError` is printed to `stderr` and the programme exits after the builder is deallocated.
 */
const char *string_builder_data(string_builder_t *builder)
{
    if (builder->size >= builder->capacity)
    {
        string_builder_resize(builder);
    }
    builder->items[builder->size] = '\0';
    return builder->items;
}

/**
 * @brief Fit the capacity of the builder to its size.
 * @param builder Builder to fit.
 * @exception If the builder can not be reallocated, an `AllocationError` is printed to `stderr` and the programme exits after the builder is deallocated.
 */
void string_builder_fit(string_builder_t *builder)
{
    if (builder->capacity == builder->size) return;

    char *items = (char *)realloc(builder->items, builder->size * sizeof(char));

    if (NULL == items && builder->size > 0)
    {
        fprintf(stderr, "AllocationError: Can not reallocate the buffer.\n");
        string_builder_delete(builder);
        exit(1);
    }

    builder->items = items;
    builder->capacity = builder->size;
}

/**
 * @brief Append the contents of one string builder to another.
 * @param destination Builder to which the contents are appended.
 * @param source Builder from which the contents are obtained.
 * @exception If the destination builder can not be reallocated, an `AllocationError` is printed to `stderr` and the programme exits after the destination builder is deallocated.
 */
void string_builder_combine(string_builder_t *destination, const string_builder_t *source)
{
    for (size_t i = 0; i < source->size; ++i)
    {
        string_builder_append(destination, string_builder_items(source)[i]);
    }
}

/**
 * @brief Obtain a view into the string builder.
 * @param builder Buffer from which to obtain the view.
 * @returns A view into the string builder.
 */
string_t sbtosv(const string_builder_t *builder)
{
    return string_new(builder->items, builder->size);
}

/**
 * @brief Remove an element from the buffer at a given index.
 * @param builder Buffer from which to remove an element.
 * @param index Index at which the removable element is located.
 * @exception If the buffer is empty, a `ValueError` is printed to `stderr` and the programme exits after the given builder is deallocated.
 * @exception If the given index is greater than or equal to the size of the buffer, an `IndexError` is printed to `stderr` and the programme exits after the given builder is deallocated.
 */
void string_builder_remove(string_builder_t *builder, size_t index)
{
    if (string_builder_empty(builder))
    {
        fprintf(stderr, "ValueError: Can not remove from an empty buffer.\n");
        string_builder_delete(builder);
        exit(1);
    }
    else if (index >= builder->size)
    {
        fprintf(stderr, "IndexError: Can not remove from buffer of size %zu at index %zu.\n", builder->size, index);
        string_builder_delete(builder);
        exit(1);
    }

    for (size_t i = index; i < builder->size - 1; ++i)
    {
        builder->items[i] = builder->items[i + 1];
    }

    --builder->size;
}

/**
 * @brief Resize the buffer by an exponential factor of two.
 * @param builder Buffer to resize.
 * @exception If the builder can not be reallocated, an `AllocationError` is printed to `stderr` and the programme exits after the given builder is deallocated.
 */
void string_builder_resize(string_builder_t *builder)
{
    string_builder_resize_by(builder, 2);
}

/**
 * @brief Safely divide a given dividend size by a given divisor size.
 * @param dividend Dividend to which to divide.
 * @param divisor Divisor from which to divide.
 * @returns The quotient of the given sizes.
 * @exception If the given divisor is zero, a `ZeroDivisionError` is printed to `stderr` and the programme exits.
 */
static size_t __safe_builder_size_divide(size_t dividend, size_t divisor)
{
    if (divisor == 0)
    {
        fprintf(stderr, "ZeroDivisionError: Can not divide by zero.\n");
        exit(1);
    }
    return (dividend / divisor);
}

/**
 * @brief Resize the buffer by a given exponential factor.
 * @param builder Buffer to resize.
 * @param scaler Exponential scaler by which to resize the buffer. Values less than `two` cause no operation.
 * @exception If the resulting capacity would overflow `size_t`, an `OverflowError` is printed to `stderr` and the programme exits after the given builder is deallocated.
 * @exception If the builder can not be reallocated, an `AllocationError` is printed to `stderr` and the programme exits after the given builder is deallocated.
 */
void string_builder_resize_by(string_builder_t *builder, size_t scaler)
{
    if (scaler < 2) return;
    else if (builder->capacity > __safe_builder_size_divide(SIZE_MAX, scaler))
    {
        fprintf(stderr, "OverflowError: The capacity has overflown its type.\n");
        string_builder_delete(builder);
        exit(1);
    }
    builder->capacity *= scaler;
    builder->items = (char *)realloc(builder->items, builder->capacity * sizeof(char));
    if (NULL == builder->items)
    {
        fprintf(stderr, "AllocationError: Can not reallocate the buffer.\n");
        string_builder_delete(builder);
        exit(1);
    }
}

/**
 * @brief Determine if the buffer is empty.
 * @param builder Buffer which is evaluated.
 * @returns If the buffer is evaluated to be empty, `true` is returned, else `false`.
 */
bool string_builder_empty(const string_builder_t *builder)
{
    return builder->size == 0;
}

/**
 * @brief Deallocate the buffer.
 * @param builder Buffer which to deallocate.
 */
void string_builder_delete(string_builder_t *builder)
{
    if (!builder->items) return;
    free(builder->items);
    builder->items = NULL;
    builder->size = 0;
    builder->capacity = 0;
}

#if defined(__cplusplus)
}
#endif

#endif // STRING_BUILDER_IMPLEMENTATION