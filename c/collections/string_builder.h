#ifndef STRING_BUILDER_H
#define STRING_BUILDER_H

#if defined (__cplusplus)
extern "C" {
#endif

#include <stddef.h> // size_t
// #include <stdbool.h> // bool

#define STRING_VIEW_IMPLEMENTATION
#include "./string_view.h"

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
 */
string_builder_t *string_builder_init(void);

/**
 * @brief Construct a new dynamic buffer of characters with a given capacity.
 * @param capacity capacity to set for the buffer.
 * @returns A new dynamic buffer of characters with a given initial capacity.
 */
string_builder_t *string_builder_with_capacity(size_t capacity);

/**
 * @brief Append a character to the buffer of characters. If the buffer is full, the buffer is resized by an exponential factor of two.
 * @param builder Buffer of characters to which to append.
 * @param item Item to append to the buffer.
 */
void string_builder_append(string_builder_t *builder, char item);

/**
 * @brief Append a null-terminated string to the buffer of characters not including the aforementioned null byte.
 * @param builder Buffer of characters to which to append.
 * @param items Null-terminated string from which to append to the buffer.
 */
void string_builder_extend(string_builder_t *builder, const char *items);

/**
 * @brief Obtain a pointer to a character in the buffer at a given index.
 * @param builder Buffer from which to access.
 * @param index Index within the buffer where the data is located.
 * @returns A pointer to the data within the given buffer at the given index.
 * @exception If the given index is greater than the size of the buffer, an `IndexError` to `stderr` is printed and the programme exits.
 */
char *string_builder_at(const string_builder_t *builder, size_t index);

/**
 * @brief Obtain a null-terminated c-string consisting of the items within the buffer.
 * @param builder Buffer from which to obtain the data.
 * @returns A null-terminated c-string consisting of the data within the array.
 */
const char *string_builder_data(string_builder_t *builder);

/**
 * @brief Fit the capacity of the builder to its size.
 * @param builder Builder to fit.
 */
void string_builder_fit(string_builder_t *builder);

/**
 * @brief Append the contents of a given buffer into another.
 * @param destination Buffer to which to append.
 * @param source Buffer from which to append.
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
 * @param index Index at which the removeable element is located.
 * @exception If the given index is greater than the size of the buffer, an `IndexError` to `stderr` is printed and the programme exits.
 * @exception If the buffer can not be reallocated, an `AllocationError` is printed to `stderr` and the programme exits.
 * @exception If the buffer is evaluated to be empty, a `ValueError` is printed to `stderr` and the programme exits.
 */
void string_builder_remove(string_builder_t *builder, size_t index);

/**
 * @brief Resize the buffer by an exponentional factor of two.
 * @param builder Buffer to resize.
 */
void string_builder_resize(string_builder_t *builder);

/**
 * @brief Resize the buffer by a given exponentional factor.
 * @param builder Buffer to resize.
 * @param scaler Exponential scaler by which to resize the buffer.
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

#endif // STRING_BUILDER_H

#ifdef STRING_BUILDER_IMPLEMENTATION

#if defined (__cplusplus)
extern "C" {
#endif

#include <stdio.h> // fprintf, stderr
#include <stdlib.h> // malloc, realloc, free, exit, NULL

#ifndef STRING_BUILDER_INITIAL_CAPACITY
#define STRING_BUILDER_INITIAL_CAPACITY 256
#endif // STRING_BUILDER_INITIAL_CAPACITY

/**
 * @brief Construct a new dynamic buffer of characters.
 * @returns A new dynamic buffer of characters.
 */
string_builder_t *string_builder_init(void)
{
    return string_builder_with_capacity(STRING_BUILDER_INITIAL_CAPACITY);
}

/**
 * @brief Construct a new dynamic buffer of characters with a given capacity.
 * @param capacity capacity to set for the buffer.
 * @returns A new dynamic buffer of characters with a given initial capacity.
 */
string_builder_t *string_builder_with_capacity(size_t capacity)
{
    string_builder_t *builder = (string_builder_t *)malloc(sizeof(string_builder_t));
    if (NULL == builder)
    {
        fprintf(stderr, "AllocationError: Can not allocate buffer.\n");
        exit(1);
    }
    builder->capacity = capacity;
    builder->size = 0;
    builder->items = (char *)calloc(builder->capacity, sizeof(char));
    if (NULL == builder->items)
    {
        fprintf(stderr, "AllocationError: Can not allocate buffer.\n");
        exit(1);
    }
    return builder;
}

/**
 * @brief Append a character to the buffer of characters. If the buffer is full, the buffer is resized by an exponential factor of two.
 * @param builder Buffer of characters to which to append.
 * @param item Item to append to the buffer.
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
 * @brief Append a null-terminated string to the buffer of characters not including the aforementioned null byte.
 * @param builder Buffer of characters to which to append.
 * @param items Null-terminated string from which to append to the buffer.
 */
void string_builder_extend(string_builder_t *builder, const char *items)
{
    if (NULL == items)
    {
        fprintf(stderr, "IllegalParametreError: items can not be NULL.\n");
        exit(1);
    }
    size_t length = strlen(items);
    for (size_t i = 0; i < length; ++i)
    {
        string_builder_append(builder, items[i]);
    }
}

/**
 * @brief Obtain a pointer to a character in the buffer at a given index.
 * @param builder Buffer from which to access.
 * @param index Index within the buffer where the data is located.
 * @returns A pointer to the data within the given buffer at the given index.
 * @exception If the given index is greater than the size of the buffer, an `IndexError` to `stderr` is printed and the programme exits.
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
 * @brief Obtain a null-terminated c-string consisting of the items within the buffer.
 * @param builder Buffer from which to obtain the data.
 * @returns A null-terminated c-string consisting of the data within the array.
 */
const char *string_builder_data(string_builder_t *builder)
{
    string_builder_append(builder, '\0');
    return builder->items;
}

/**
 * @brief Fit the capacity of the builder to its size.
 * @param builder Builder to fit.
 */
void string_builder_fit(string_builder_t *builder)
{
    builder->capacity = builder->size;
}

/**
 * @brief Fit the capacity of the builder to its size.
 * @param builder Builder to fit.
 */
void string_builder_combine(string_builder_t *destination, const string_builder_t *source)
{
    for (size_t i = 0; i < source->size; ++i)
    {
        string_builder_append(destination, (*string_builder_at(source, i)));
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
 * @param index Index at which the removeable element is located.
 * @exception If the given index is greater than the size of the buffer, an `IndexError` to `stderr` is printed and the programme exits.
 * @exception If the buffer can not be reallocated, an `AllocationError` is printed to `stderr` and the programme exits.
 * @exception If the buffer is evaluated to be empty, a `ValueError` is printed to `stderr` and the programme exits.
 */
void string_builder_remove(string_builder_t *builder, size_t index)
{
    if (index >= builder->size)
    {
        fprintf(stderr, "IndexError: Can not remove from buffer of size %zu at index %zu.\n", builder->size, index);
        exit(1);
    }
    else if (string_builder_empty(builder))
    {
        fprintf(stderr, "ValueError: Can not remove from an empty buffer.\n");
        exit(1);
    }
    for (size_t i = index; i < builder->size - 1; i++)
    {
        builder->items[i] = builder->items[i + 1];
    }
    builder->size--;
    char *temp = (char *)realloc(builder->items, builder->size * sizeof(char));
    if (NULL == temp)
    {
        fprintf(stderr, "AllocationError: Can not reallocate the buffer.\n");
        exit(1);
    }
    builder->items = temp;
}

/**
 * @brief Resize the buffer by an exponentional factor of two.
 * @param builder Buffer to resize.
 */
void string_builder_resize(string_builder_t *builder)
{
    string_builder_resize_by(builder, 2);
}

/**
 * @brief Resize the buffer by a given exponentional factor.
 * @param builder Buffer to resize.
 * @param scaler Exponential scaler by which to resize the buffer.
 */
void string_builder_resize_by(string_builder_t *builder, size_t scaler)
{
    builder->capacity *= scaler;
    builder->items = (char *)realloc(builder->items, builder->capacity * sizeof(char *));
    if (NULL == builder->items)
    {
        fprintf(stderr, "AllocationError: Can not reallocate the buffer.\n");
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
    if (!builder) return;
    free(builder);
    builder = NULL;
}

#if defined(__cplusplus)
}
#endif

#endif // STRING_BUILDER_IMPLEMENTATION