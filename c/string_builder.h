#ifndef STRING_BUILDER_H_
#define STRING_BUILDER_H_

#include <stddef.h> // size_t
#include <stdio.h> // fprintf
#include <stdlib.h> // malloc, realloc, NULL, exit

#ifndef STRING_BUILDER_CAPACITY
#define STRING_BUILDER_CAPACITY 56ULL
#endif // STRING_BUILDER_CAPACITY

typedef struct
{
    size_t size;
    size_t capacity;
    char *items;
} string_builder_t;

/*
* @brief Initialize a dynamic array of characters.
* @returns A, heap allocated, dynamic array of characters.
* @throws If the structure can not be heap allocated, or the malloc call returns `NULL`, then an `AllocationError` will be thrown and the programme will exit.
* @throws If the underlying array of characters can not be allocated, or the malloc call returns `NULL`, then an `AllocationError` is thrown and the programme exits.
*/
string_builder_t *string_builder_init();

/*
* @brief Append a character to the array.
* @param builder Array of characters to which to append.
* @param item Item from which to append.
* @throws If the underlying array of characters can not be resized, or realloc returns `NULL`, then an `AllocationError` is thrown and the programme exits.
*/
void string_builder_append(string_builder_t *builder, char item);

/*
* @brief Access an element within the array at a given index.
* @param builder Array from which to access the element.
* @param index Index at which the element can be found.
* @returns The address of a character stored within the array at the given index.
* @throws If the given index is outside of the size of the underlying array, then an `IndexError` is thrown and the programme exits.
*/
char *string_builder_at(const string_builder_t *builder, size_t index);

/*
* @brief Access the underlying flattened array.
* @param builder Structure from which to access the array.
* @returns A null-terminated pointer to a character array.
*/
const char *string_builder_data(string_builder_t *builder);

/*
* @brief Append the underlying data of one string builder with that of another. Because the implementation is using the `string_builder_append` function, it has the same error signature as that function.
* @param destination Destination string builder to which the source will be concatenated.
* @param source Source string builder from which to concatenate to the destination.
* @throws If the underlying array of characters can not be resized, or realloc returns `NULL`, then an `AllocationError` is thrown and the programme exits.
*/
void string_builder_combine(string_builder_t *destination, const string_builder_t *source);

/*
* @brief Quadratically resize the string builder.
* @param builder Array of characters to resize.
* @throws If the array can not be resized, or realloc returns `NULL`, an `AllocationError` is thrown and the programme exits.
*/
void string_builder_resize(string_builder_t *builder);

/*
* @brief Resize the given array by a given scaler value.
* @param builder Array of characters to which to resize.
* @param scaler Scaler value by which to resize the array.
* @throws If the array can not be resized, or realloc returns `NULL`, an `AllocationError` is thrown and the programme exits.
*/
void string_builder_resize_by(string_builder_t *builder, size_t scaler);

/*
* @brief Destroy the memory associated with the given array. Both the underlying array, and the structure pointer will be freed and set to `NULL`.
* @param builder Array to free.
*/
void string_builder_delete(string_builder_t *builder);

#endif // STRING_BUILDER_H_

#ifdef STRING_BUILDER_IMPLEMENTATION

/*
* @brief Initialize a dynamic array of characters.
* @returns A, heap allocated, dynamic array of characters.
* @throws If the structure can not be heap allocated, or the malloc call returns `NULL`, then an `AllocationError` will be thrown and the programme will exit.
* @throws If the underlying array of characters can not be allocated, or the malloc call returns `NULL`, then an `AllocationError` is thrown and the programme exits.
*/
string_builder_t *string_builder_init()
{
    string_builder_t *builder = (string_builder_t *)malloc(sizeof(string_builder_t) + (STRING_BUILDER_CAPACITY * sizeof(char)));
    if (NULL == builder)
    {
        fprintf(stderr, "AllocationError: Can not allocate enough memory to build a string.\n");
        exit(1);
    }
    builder->capacity = STRING_BUILDER_CAPACITY;
    builder->size = 0;
    builder->items = (char *)malloc(sizeof(char) * STRING_BUILDER_CAPACITY);
    if (NULL == builder->items)
    {
        free(builder);
        fprintf(stderr, "AllocationError: Can not allocate enough memory to build a string array.\n");
        exit(1);
    }
    return builder;
}

/*
* @brief Append a character to the array.
* @param builder Array of characters to which to append.
* @param item Item from which to append.
* @throws If the underlying array of characters can not be resized, or realloc returns `NULL`, then an `AllocationError` is thrown and the programme exits.
*/
void string_builder_append(string_builder_t *builder, char item)
{
    if (builder->size >= builder->capacity)
    {
        string_builder_resize(builder);
    }
    builder->items[builder->size++] = item;
}

/*
* @brief Access an element within the array at a given index.
* @param builder Array from which to access the element.
* @param index Index at which the element can be found.
* @returns The address of a character stored within the array at the given index.
* @throws If the given index is outside of the size of the underlying array, then an `IndexError` is thrown and the programme exits.
*/
char *string_builder_at(const string_builder_t *builder, size_t index)
{
    if (index > builder->size)
    {
        fprintf(stderr, "IndexError: Can not access an element outside of array.\n");
        exit(1);
    }
    return &builder->items[index];
}

/*
* @brief Access the underlying flattened array.
* @param builder Structure from which to access the array.
* @returns A null-terminated pointer to a character array.
*/
const char *string_builder_data(string_builder_t *builder)
{
    string_builder_append(builder, '\0');
    return builder->items;
}

/*
* @brief Append the underlying data of one string builder with that of another. Because the implementation is using the `string_builder_append` function, it has the same error signature as that function.
* @param destination Destination string builder to which the source will be concatenated.
* @param source Source string builder from which to concatenate to the destination.
* @throws If the underlying array of characters can not be resized, or realloc returns `NULL`, then an `AllocationError` is thrown and the programme exits.
*/
void string_builder_combine(string_builder_t *destination, const string_builder_t *source)
{
    for (size_t i = 0; i < source->size; ++i)
    {
        string_builder_append(destination, *string_builder_at(source, i));
    }
}

/*
* @brief Quadratically resize the string builder.
* @param builder Array of characters to resize.
* @throws If the array can not be resized, or realloc returns `NULL`, an `AllocationError` is thrown and the programme exits.
*/
void string_builder_resize(string_builder_t *builder)
{
    string_builder_resize_by(builder, 2);
}

/*
* @brief Resize the given array by a given scaler value.
* @param builder Array of characters to which to resize.
* @param scaler Scaler value by which to resize the array.
* @throws If the array can not be resized, or realloc returns `NULL`, an `AllocationError` is thrown and the programme exits.
*/
void string_builder_resize_by(string_builder_t *builder, size_t scaler)
{
    builder->capacity *= scaler;
    builder->items = (char *)realloc(builder->items, builder->capacity);
    if (NULL == builder->items)
    {
        fprintf(stderr, "AllocationErorr: Can not resize array.\n");
        exit(1);
    }
}

/*
* @brief Destroy the memory associated with the given array. Both the underlying array, and the structure pointer will be freed and set to `NULL`.
* @param builder Array to free.
*/
void string_builder_delete(string_builder_t *builder)
{
    if (!(builder))
    {
        return;
    }
    builder->size = 0;
    builder->capacity = 0;
    if (!(builder->items))
    {
        return;
    }
    free(builder->items);
    builder->items = NULL;
    builder = NULL;
}

#endif // STRING_BUILDER_IMPLEMENTATION