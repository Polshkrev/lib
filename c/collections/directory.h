#ifndef DIRECTORY_H
#define DIRECTORY_H

#ifndef DIRECTORY_CAPACITY
#define DIRECTORY_CAPACITY 256
#endif // DIRECTORY_CAPACITY

#define ENTRY_IMPLEMENTATION
#include "entry.h"

/**
 * @brief Representation of a directory on the filesystem.
 */
typedef struct
{
    size_t size;
    size_t capacity;
    path_t *root;
    entry_t **entries;
} directory_t;

/**
 * @brief Construct a new directory at a given root.
 * @param root Root of the directory.
 * @returns A pointer to a directory at a given root.
 * @exception If the directory can not be allocated, an `AllocationError` is printed to standard error and the programme exits.
 * @exception If the underlying entry array can not be allocated, an `AllocationError` is printed to standard error and the programme exits.
 */
directory_t *directory_init(path_t *root);

/**
 * @brief Construct a new directory at a given root with a given initial capacity.
 * @param root Root of the directory.
 * @returns A pointer to a directory at a given root with a given capacity.
 * @exception If the directory can not be allocated, an `AllocationError` is printed to standard error and the programme exits.
 * @exception If the underlying entry array can not be allocated, an `AllocationError` is printed to standard error and the programme exits.
 */
directory_t *directory_init_with_capacity(path_t *root, size_t capacity);

/**
 * @brief Append an `entry_t` to the directory.
 * @param directory Directory to which to append.
 * @param entry Entry from which to append.
 * @exception If the directory can not be reallocated, an `AllocationError` is printed to standard error and the programme exits.
 */
void directory_append(directory_t *directory, entry_t *entry);

/**
 * @brief Obtain a pointer to an `entry_t` at a given index.
 * @param directory Directory from which to access.
 * @param index Index at which the directory is to be accessed.
 * @exception If the index is outside of the bounds of the directory, an `IndexError` is printed to standard error and the programme exits.
 */
entry_t **directory_at(directory_t *directory, size_t index);

/**
 * @brief Read a given directory.
 * @param directory Directory to read. Each `entry_t` that is scanned from the filesystem is appended to the directory.
 * @returns True if the directory can be written, else false.
 */
bool directory_read(directory_t *directory);

/**
 * @brief Represent a directory to as a string.
 * @param directory Directory to represent.
 * @param buffer Buffer to which to render the resulting string.
 * @exception If the given items are null, an `IllegalParametreError` is printed to standard error and the programme exits.
 * @exception If the builder can not be reallocated, an `AllocationError` is printed to standard error and the programme exits.
 */
void directory_to_string(directory_t *directory, string_builder_t *buffer);

/**
 * @brief Resize a given directory by a factor of two.
 * @param directory Directory to resize.
 * @exception If the directory can not be reallocated, an `AllocationError` is printed to standard error and the programme exits.
 */
void directory_resize(directory_t *directory);

/**
 * @brief Resize a given directory by a given scaler.
 * @param directory Directory to resize.
 * @param scaler Scaler by which to resize the directory.
 * @exception If the directory can not be reallocated, an `AllocationError` is printed to standard error and the programme exits.
 */
void directory_resize_by(directory_t *directory, size_t scaler);

/**
 * @brief Remove an `entry_t` from a given directory at a given index.
 * @param directory Directory from which to remove.
 * @param index Index at which to remove an `entry_t` within the directory.
 * @exception If the given index is outside of the bounds of the directory, an `IndexError` is printed to standard error and the programme exits.
 * @exception If the directory is empty, a `ValueError` is printed to standard error and the programme exits.
 * @exception If the directory can not be reallocated, an `AllocationError` is printed to standard error and the programme exits.
 */
void directory_remove(directory_t *directory, size_t index);

/**
 * @brief Deallocate a directory.
 * @param directory Directory to deallocate.
 */
void directory_delete(directory_t *directory);

#endif // DIRECTORY_H

#ifdef DIRECTORY_IMPLEMENTATION

#define FILES_IMPLEMENTATION
#include "files.h"

/**
 * @brief Construct a new directory at a given root.
 * @param root Root of the directory.
 * @returns A pointer to a directory at a given root.
 * @exception If the directory can not be allocated, an `AllocationError` is printed to standard error and the programme exits.
 * @exception If the underlying entry array can not be allocated, an `AllocationError` is printed to standard error and the programme exits.
 */
directory_t *directory_init(path_t *root)
{
    return directory_init_with_capacity(root, DIRECTORY_CAPACITY);
}

/**
 * @brief Construct a new directory at a given root with a given initial capacity.
 * @param root Root of the directory.
 * @returns A pointer to a directory at a given root with a given capacity.
 * @exception If the directory can not be allocated, an `AllocationError` is printed to standard error and the programme exits.
 * @exception If the underlying entry array can not be allocated, an `AllocationError` is printed to standard error and the programme exits.
 */
directory_t *directory_init_with_capacity(path_t *root, size_t capacity)
{
    directory_t *directory = (directory_t *)calloc(1, sizeof(directory_t));
    if (NULL == directory)
    {
        fprintf(stderr, "AllocationError: Can not allocate a new directory.\n");
        exit(1);
    }
    directory->size = 0;
    directory->capacity = capacity;
    directory->root = root;
    directory->entries = (entry_t **)malloc(sizeof(entry_t *) * directory->capacity);
    if (NULL == directory->entries)
    {
        fprintf(stderr, "AllocationError: Can not allocate a new directory.\n");
        path_delete(root);
        if (directory) free(directory);
        exit(1);
    }
    return directory;
}

/**
 * @brief Append an `entry_t` to the directory.
 * @param directory Directory to which to append.
 * @param entry Entry from which to append.
 * @exception If the directory can not be reallocated, an `AllocationError` is printed to standard error and the programme exits.
 */
void directory_append(directory_t *directory, entry_t *entry)
{
    if (directory->size >= directory->capacity)
    {
        directory_resize(directory);
    }
    directory->entries[directory->size++] = entry;
}

/**
 * @brief Obtain a pointer to an `entry_t` at a given index.
 * @param directory Directory from which to access.
 * @param index Index at which the directory is to be accessed.
 * @exception If the index is outside of the bounds of the directory, an `IndexError` is printed to standard error and the programme exits.
 */
entry_t **directory_at(directory_t *directory, size_t index)
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
 * @brief Read a given directory.
 * @param directory Directory to read. Each `entry_t` that is scanned from the filesystem is appended to the directory.
 * @returns True if the directory can be written, else false.
 */
bool directory_read(directory_t *directory)
{
    files_t *files = files_init(passtr(directory->root));
    if (!files_fill(files))
    {
        files_delete(files);
        return false;
    }
    bool result = true;
    for (size_t i = 0; i < files->size; ++i)
    {
        path_t *main_path = path_from(files->files[i]);
        entry_t *entry = entry_init(main_path);
        switch (entry->type)
        {
            case NONE_TYPE:
            {
                result = false && result;
            } break;
            case DIRECTORY_TYPE:
            {
                directory_append(directory, entry);
                files_t *directory_files = files_init(files->files[i]);
                for (size_t i = 0; i < directory_files->size; ++i)
                {
                    path_t *path = path_from(directory_files->files[i]);
                    entry_t *entry = entry_init(path);
                    directory_append(directory, entry);
                }
            } break;
            default:
            {
                directory_append(directory, entry);
            } break;
        }
    }
    return result;
}

/**
 * @brief Represent a given directory as a string.
 * @param directory Directory to represent as a string.
 * @param result Buffer to which to formulate the string.
 * @exception If the given items are null, an `IllegalParametreError` is printed to standard error and the programme exits.
 * @exception If the builder can not be reallocated, an `AllocationError` is printed to standard error and the programme exits.
 */
void directory_to_string(directory_t *directory, string_builder_t *result)
{
    for (size_t i = 0; i < directory->size; ++i)
    {
        string_builder_extend(result, entry_to_string(directory->entries[i]));
        string_builder_append(result, '\n');
    }
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
 * @brief Resize a given directory by a given scaler.
 * @param directory Directory to resize.
 * @param scaler Scaler by which to resize the directory.
 * @exception If the directory can not be reallocated, an `AllocationError` is printed to standard error and the programme exits.
 */
void directory_resize_by(directory_t *directory, size_t scaler)
{
    directory->capacity *= scaler;
    directory->entries = (entry_t **)realloc(directory->entries, directory->capacity);
    if (NULL == directory->entries)
    {
        fprintf(stderr, "AllocationError: Can not reallocate entries.\n");
        directory_delete(directory);
        exit(1);
    }
}

/**
 * @brief Remove an `entry_t` from a given directory at a given index.
 * @param directory Directory from which to remove.
 * @param index Index at which to remove an `entry_t` within the directory.
 * @exception If the given index is outside of the bounds of the directory, an `IndexError` is printed to standard error and the programme exits.
 * @exception If the directory is empty, a `ValueError` is printed to standard error and the programme exits.
 * @exception If the directory can not be reallocated, an `AllocationError` is printed to standard error and the programme exits.
 */
void directory_remove(directory_t *directory, size_t index)
{
    if (index >= directory->size)
    {
        fprintf(stderr, "IndexError: Can not remove from directory of size %zu at index %zu.\n", directory->size, index);
        directory_delete(directory);
        exit(1);
    }
    else if (directory->size == 0 || NULL == directory->entries)
    {
        fprintf(stderr, "ValueError: Can not remove from an empty directory '%s'.\n", passtr(directory->root));
        directory_delete(directory);
        exit(1);
    }
    for (size_t i = index; i < directory->size - 1; i++)
    {
        directory->entries[i] = directory->entries[i + 1];
    }
    directory->size--;
    entry_t **temporary = (entry_t **)realloc(directory->entries, directory->size * sizeof(entry_t *));
    if (NULL == temporary)
    {
        fprintf(stderr, "AllocationError: Can not reallocate the directory.\n");
        directory_delete(directory);
        exit(1);
    }
    directory->entries = temporary;
}

/**
 * @brief Deallocate a directory.
 * @param directory Directory to deallocate.
 */
void directory_delete(directory_t *directory)
{
    if (!directory) return;
    else if (!directory->entries) return;
    for (size_t i = 0; i < directory->size; ++i)
    {
        entry_delete(directory->entries[i]);
    }
    free(directory->entries);
    directory->entries = NULL;
    free(directory);
    directory = NULL;
}

#endif // DIRECTORY_IMPLEMENTATION