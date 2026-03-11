#ifndef FILES_H
#define FILES_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stddef.h> // size_t
#include <stdbool.h> // bool

/**
 * @brief Dynamic array of files on the file system.
 */
typedef struct
{
    const char *root;
    char **files;
    size_t size;
    size_t capacity;
} files_t;

/**
 * @brief Construct a new dynamic array of files with a given root.
 * @param root Root of the directory where the files are located.
 * @returns A new dynamic array of files on the filesystem with a given root.
 * @exception If the array can not be allocated, an `AllocationError` is printed to `stderr` and the programme exits.
 */
files_t *files_init(const char *root);

/**
 * @brief Construct a new dynamic array of files with a given root and initial capacity.
 * @param root Root of the directory where the files are located.
 * @param capacity Capacity with which to initialize the files.
 * @returns A new dynamic array of files on the filesystem with a given root and capacity.
 * @exception If the array can not be allocated, an `AllocationError` is printed to `stderr` and the programme exits.
 */
files_t *files_init_with_capacity(const char *root, size_t capacity);

/**
 * @brief Append a new filepath to the array of files.
 * @param files Array of files to which to append.
 * @param entry Path from which to append.
 */
void files_append(files_t *files, const char *entry);

/**
 * @brief Obtain a path at a given index within the array of files.
 * @param files Array of files from which to search.
 * @param index Index from which to search.
 * @returns A pointer to a path within the array of files at the given index. If the given index is greater than or equal to the size of the array of files, `NULL` is returned.
 */
char **files_at(const files_t *files, size_t index);

/**
 * @brief Resize the array of files by a scaler value of two.
 * @param files Array of files to resize.
 * @exception If the array of files can not be reallocated, an `AllocationError` is printed to `stderr` and the programme exits.
 */
void files_resize(files_t *files);

/**
 * @brief Resize the array of files by a given scaler value.
 * @param files Array of files to resize.
 * @param scaler Scaler value from which to resize the array of files.
 * @exception If the array of files can not be reallocated, an `AllocationError` is printed to `stderr` and the programme exits.
 */
void files_resize_by(files_t *files, size_t scaler);

/**
 * @brief Assign the capacity of the array to the size.
 * @param files Array to fit.
 */
void files_fit(files_t *files);

/**
 * @brief Extend the array of files.
 * @param files Array of files to extend.
 * @returns True if the paths stored at the root can be added, else false.
 */
bool files_fill(files_t *files);

/**
 * @brief Deallocate the array of files.
 * @param files Array to deallocate.
 */
void files_delete(files_t *files);

#endif // FILES_H

#ifdef FILES_IMPLEMENTATION

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#endif // _WIN32

#ifndef FILES_CAPACITY
#define FILES_CAPACITY 256
#endif // FILES_CAPACITY

/**
 * @brief Construct a new dynamic array of files with a given root.
 * @param root Root of the directory where the files are located.
 * @returns A new dynamic array of files on the filesystem with a given root.
 * @exception If the array can not be allocated, an `AllocationError` is printed to `stderr` and the programme exits.
 */
files_t *files_init(const char *root)
{
    return files_init_with_capacity(root, FILES_CAPACITY);
}

/**
 * @brief Construct a new dynamic array of files with a given root and initial capacity.
 * @param root Root of the directory where the files are located.
 * @param capacity Capacity with which to initialize the files.
 * @returns A new dynamic array of files on the filesystem with a given root and capacity.
 * @exception If the array can not be allocated, an `AllocationError` is printed to `stderr` and the programme exits.
 */
files_t *files_init_with_capacity(const char *root, size_t capacity)
{
    files_t *files = malloc(sizeof(files_t));
    if (NULL == files)
    {
        fprintf(stderr, "AllocationError: Can not allocate files.\n");
        exit(1);
    }
    files->size = 0;
    files->capacity = capacity;
    files->files = (char **)malloc(sizeof(char *) * capacity);
    if (NULL == files->files)
    {
        fprintf(stderr, "AllocationError: Can not allocate files array.\n");
        if (files) free(files);
        exit(1);
    }
    files->root = root;
    return files;
}

/**
 * @brief Append a new filepath to the array of files.
 * @param files Array of files to which to append.
 * @param entry Path from which to append.
 * @exception If the array of files can not be reallocated, an `AllocationError` is printed to `stderr` and the programme exits.
 */
void files_append(files_t *files, const char *entry)
{
    if (files->size >= files->capacity)
    {
        files_resize(files);
    }
    files->files[files->size] = (char*)malloc((strlen(entry) + 1) * sizeof(char));
    if (files->files[files->size] == NULL)
    {
        fprintf(stderr, "ValueError: Can not allocate enough memory for entry: '%s'.\n", entry);
        files_delete(files);
        exit(1);
    }
#ifdef _WIN32
    StringCbCopy(files->files[files->size], (strlen(entry) + 1) * sizeof(char), entry);
#else
#error "NotImplementedError: The linux version of `files_append()` has not been defined yet."
#endif
    files->size++;
}

/**
 * @brief Obtain a path at a given index within the array of files.
 * @param files Array of files from which to search.
 * @param index Index from which to search.
 * @returns A pointer to a path within the array of files at the given index. If the given index is greater than or equal to the size of the array of files, `NULL` is returned.
 */
char **files_at(const files_t *files, size_t index)
{
    if (index >= files->size) return NULL;
    return &files->files[index];
}

/**
 * @brief Resize the array of files by a scaler value of two.
 * @param files Array of files to resize.
 * @exception If the array of files can not be reallocated, an `AllocationError` is printed to `stderr` and the programme exits.
 */
void files_resize(files_t *files)
{
    files_resize_by(files, 2);
}

/**
 * @brief Resize the array of files by a given scaler value.
 * @param files Array of files to resize.
 * @param scaler Scaler value from which to resize the array of files.
 * @exception If the array of files can not be reallocated, an `AllocationError` is printed to `stderr` and the programme exits.
 */
void files_resize_by(files_t *files, size_t scaler)
{
    files->capacity *= scaler;
    files->files = (char **)realloc(files->files, files->capacity);
    if (NULL == files)
    {
        fprintf(stderr, "AllocationError: Can not allocate files array.\n");
        files_delete(files);
        exit(1);
    }
}

/**
 * @brief Assign the capacity of the array to the size.
 * @param files Array to fit.
 */
void files_fit(files_t *files)
{
    files->capacity = files->size;
}

/**
 * @brief Deallocate the array of files.
 * @param files Array to deallocate.
 */
void files_delete(files_t *files)
{
    if (!files->files) return;
    for (size_t i = 0; i < files->size; i++) {
        free(files->files[i]);
    }
    free(files->files);
    files->files = NULL;
    files->size = 0;
    files->capacity = 0;
    if (!files) return;
    free(files);
}

#ifdef _WIN32
#ifndef WIN32_ERR_MSG_SIZE
/**
 * @brief Size of the error message format buffer within the windows implementation.
 */
#define WIN32_ERR_MSG_SIZE (4 * 1024)
#endif // WIN32_ERR_MSG_SIZE

/**
 * @brief Obtain the entries at the given array root.
 * @param files Array of files to which to append.
 * @param handle Handle to the directory to open.
 * @param data Pointer to the data within the directory.
 * @returns True if the entries contained at the directory root can be obtained, else false.
 */
static bool _get_entries_windows(files_t *files, const char *path)
{
    char buffer[MAX_PATH];
    WIN32_FIND_DATA data;
    HANDLE find = INVALID_HANDLE_VALUE;
    StringCchCopy(buffer, MAX_PATH, path);
    StringCchCat(buffer, MAX_PATH, "\\*");
    find = FindFirstFile(buffer, &data);
    if (INVALID_HANDLE_VALUE == find) return false;
    bool result = true;
    do {
        if (strcmp(data.cFileName, ".") == 0 || strcmp(data.cFileName, "..") == 0) continue;
        char full_path[MAX_PATH];
        StringCchCopy(full_path, MAX_PATH, path);
        StringCchCat(full_path, MAX_PATH, "\\");
        StringCchCat(full_path, MAX_PATH, data.cFileName);
        // printf("Full Path: %s\n", full_path);
        files_append(files, full_path);
        if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            if (!_get_entries_windows(files, full_path))
            {
                result = false;
            }
        }
    } while (FindNextFile(find, &data) != 0);
    FindClose(find);
    return result;
}
#else
#endif // _WIN32

/**
 * @brief Extend the array of files.
 * @param files Array of files to extend.
 * @returns True if the paths stored at the root can be added, else false.
 */
bool files_fill(files_t *files)
{
#ifdef _WIN32
    bool result = _get_entries_windows(files, files->root);
#else
#error "NotImplementedError: The linux version of `files_fill()` has not been defined yet."
#endif // _WIN32
    return result;
}

#endif // FILES_IMPLEMENTATION