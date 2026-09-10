#ifndef DIRECTORY_H
#define DIRECTORY_H

#ifndef DIRECTORY_CAPACITY
#define DIRECTORY_CAPACITY 256
#endif // DIRECTORY_CAPACITY

#define ENTRY_IMPLEMENTATION
#include "entry.h" // entry_t

#include <stdbool.h> // bool

/**
 * @brief Representation of a directory on the filesystem.
 */
typedef struct
{
    size_t size;
    size_t capacity;
    path_t root;
    entry_t *entries;
} directory_t;

/**
 * @brief Construct a new directory at a given root.
 * @param root Root of the directory.
 * @returns A directory at the given root.
 * @exception If the underlying entry array can not be allocated, an `AllocationError` is printed to standard error and the programme exits.
 */
directory_t directory_init(path_t root);

/**
 * @brief Construct a new directory at a given root with a given initial capacity.
 * @param root Root of the directory.
 * @param capacity Initial capacity of the directory.
 * @returns A directory at the given root with the given capacity.
 * @exception If the underlying entry array can not be allocated, an `AllocationError` is printed to standard error and the programme exits.
 */
directory_t directory_init_with_capacity(path_t root, size_t capacity);

/**
 * @brief Append an entry to the directory.
 * @param directory Directory to which to append.
 * @param entry Entry from which to append.
 * @exception If the directory can not be reallocated, an `AllocationError` is printed to standard error and the programme exits.
 */
void directory_append(directory_t *directory, entry_t entry);

/**
 * @brief Obtain a pointer to an `entry_t` at a given index.
 * @param directory Directory from which to access.
 * @param index Index at which the directory is to be accessed.
 * @exception If the index is outside of the bounds of the directory, an `IndexError` is printed to standard error and the programme exits.
 */
entry_t *directory_at(directory_t *directory, size_t index);

/**
 * @brief Resize a given directory by a factor of two.
 * @param directory Directory to resize.
 * @exception If the directory can not be reallocated, an `AllocationError` is printed to standard error and the programme exits.
 */
void directory_resize(directory_t *directory);

/**
 * @brief Resize a given directory by an exponential factor.
 * @param directory Directory to resize.
 * @param scaler Exponential scaler by which to resize the directory. Values less than `2` cause no operation.
 * @exception If the resulting capacity would overflow `size_t`, an `OverflowError` is printed to `stderr` and the programme exits after the given directory is deallocated.
 * @exception If the directory can not be reallocated, an `AllocationError` is printed to `stderr` and the programme exits after the given directory is deallocated.
 */
void directory_resize_by(directory_t *directory, size_t scaler);

/**
 * @brief Remove from a given directory at a given index.
 * @param directory Directory from which to remove.
 * @param index Index at which to remove an `entry_t`.
 * @exception If the given index is outside of the bounds of the directory, an `IndexError` is printed to standard error and the programme exits.
 */
void directory_remove(directory_t *directory, size_t index);

/**
 * @brief Determine if the given directory is empty.
 * @param directory Directory to evaluate.
 * @returns True if the given directory is evaluated to be empty, else false.
 */
bool directory_empty(const directory_t *directory);

/**
 * @brief Deallocate a directory.
 * @param directory Directory to deallocate.
 */
void directory_delete(directory_t *directory);

#endif // DIRECTORY_H

#ifdef DIRECTORY_IMPLEMENTATION

#include <stdio.h> // fprintf, stderr
#include <stdlib.h> // malloc, realloc, free, exit, NULL

/**
 * @brief Construct a new directory at a given root.
 * @param root Root of the directory.
 * @returns A directory at the given root.
 * @exception If the underlying entry array can not be allocated, an `AllocationError` is printed to standard error and the programme exits.
 */
directory_t directory_init(path_t root)
{
    return directory_init_with_capacity(root, DIRECTORY_CAPACITY);
}

/**
 * @brief Construct a new directory at a given root with a given initial capacity.
 * @param root Root of the directory.
 * @param capacity Initial capacity of the directory.
 * @returns A directory at the given root with the given capacity.
 * @exception If the underlying entry array can not be allocated, an `AllocationError` is printed to standard error and the programme exits.
 */
directory_t directory_init_with_capacity(path_t root, size_t capacity)
{
    size_t clamped_capacity = CLAMP(capacity, 1, SIZE_MAX);
    entry_t *entries = (entry_t *)malloc(clamped_capacity * sizeof(entry_t));
    if (NULL == entries)
    {
        fprintf(stderr, "AllocationError: Can not allocate enough memory for the array of entries.\n");
        exit(1);
    }
    return (directory_t)
    {
        .root = root,
        .capacity = clamped_capacity,
        .size = 0,
        .entries = entries
    };
}

/**
 * @brief Append an entry to the directory.
 * @param directory Directory to which to append.
 * @param entry Entry from which to append.
 * @exception If the directory can not be reallocated, an `AllocationError` is printed to standard error and the programme exits.
 */
void directory_append(directory_t *directory, entry_t entry)
{
    if (directory->size >= directory->capacity)
    {
        directory_resize(directory);
    }
    directory->entries[directory->size++] = entry;
}

/**
 * @brief Obtain a pointer to an entry at a given index.
 * @param directory Directory from which to access.
 * @param index Index at which the directory is to be accessed.
 * @exception If the index is outside of the bounds of the directory, an `IndexError` is printed to standard error and the programme exits.
 */
entry_t *directory_at(directory_t *directory, size_t index)
{
    if (index >= directory->size)
    {
        fprintf(stderr, "IndexError: Can not access directory of size %zu at index %zu.\n", directory->size, index);
        directory_delete(directory);
        exit(1);
    }
    return &directory->entries[index];
}

/**
 * @brief Resize a given directory by a factor of two.
 * @param directory Directory to resize.
 * @exception If the directory can not be reallocated, an `AllocationError` is printed to standard error and the programme exits.
 */
void directory_resize(directory_t *directory)
{
    directory_resize_by(directory, 2);
}

/**
 * @brief Resize a given directory by an exponential factor.
 * @param directory Directory to resize.
 * @param scaler Exponential scaler by which to resize the directory. Values less than `2` cause no operation.
 * @exception If the resulting capacity would overflow `size_t`, an `OverflowError` is printed to `stderr` and the programme exits after the given directory is deallocated.
 * @exception If the directory can not be reallocated, an `AllocationError` is printed to `stderr` and the programme exits after the given directory is deallocated.
 */
void directory_resize_by(directory_t *directory, size_t scaler)
{
    if (scaler < 2) return;
    else if (directory->capacity > __safe_builder_size_divide(SIZE_MAX, scaler))
    {
        fprintf(stderr, "OverflowError: The capacity has overflown its type.\n");
        directory_delete(directory);
        exit(1);
    }
    directory->capacity *= scaler;
    if (directory->capacity > __safe_builder_size_divide(SIZE_MAX, sizeof(entry_t)))
    {
        fprintf(stderr, "OverflowError: The directory allocation size has overflown its type.\n");
        directory_delete(directory);
        exit(1);
    }
    directory->entries = (entry_t *)realloc(directory->entries, directory->capacity * sizeof(entry_t));
    if (NULL == directory->entries)
    {
        fprintf(stderr, "AllocationError: Can not reallocate the directory.\n");
        directory_delete(directory);
        exit(1);
    }
}

/**
 * @brief Remove an entry from a given directory at a given index.
 * @param directory Directory from which to remove.
 * @param index Index at which to remove an entry.
 * @exception If the given index is outside of the bounds of the directory, an `IndexError` is printed to standard error and the programme exits.
 */
void directory_remove(directory_t *directory, size_t index)
{
    if (index >= directory->size)
    {
        fprintf(stderr, "IndexError: Can not remove from directory of size %zu at index %zu.\n", directory->size, index);
        directory_delete(directory);
        exit(1);
    }

    string_builder_delete(&directory->entries[index].content);

    for (size_t current = index; current + 1 < directory->size; current++)
    {
        directory->entries[current] = directory->entries[current + 1];
    }

    directory->size--;
}

/**
 * @brief Determine if the given directory is empty.
 * @param directory Directory to evaluate.
 * @returns True if the given directory is evaluated to be empty, else false.
 */
bool directory_empty(const directory_t *directory)
{
    return directory->size == 0 || directory->entries == NULL;
}

/**
 * @brief Deallocate a directory.
 * @param directory Directory to deallocate.
 */
void directory_delete(directory_t *directory)
{
    if (!directory->entries) return;
    for (size_t i = 0; i < directory->size; ++i)
    {
        string_builder_delete(&directory->entries[i].content);
    }
    free(directory->entries);
    directory->entries = NULL;
}

#endif // DIRECTORY_IMPLEMENTATION