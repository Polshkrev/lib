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
    size_t size;
    size_t capacity;
    char **files;
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
void files_append(files_t *files, char *entry);

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

#define BUFFER_IMPLEMENTATION
#include "buffer.h"

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
void files_append(files_t *files, char *entry)
{
    if (files->size >= files->capacity)
    {
        files_resize(files);
    }
    files->files[files->size++] = entry;
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
    // else if (!files->files) return;
    free(files->files);
    if (!files) return;
    free(files);
}

#ifdef _WIN32

/**
 * @brief OS-dependant path seperator.
 */
#define PATH_SEPERATOR '\\'

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#ifndef WIN32_ERR_MSG_SIZE
/**
 * @brief Size of the error message format buffer within the windows implementation.
 */
#define WIN32_ERR_MSG_SIZE (4 * 1024)
#endif // WIN32_ERR_MSG_SIZE

/**
 * @brief Format a given error code.
 * @param error Error code to format.
 * @returns A formatted string obtained from the given error code.
 */
static char *_win32_error_message(DWORD error) {
    static char win32ErrMsg[WIN32_ERR_MSG_SIZE] = {0};
    DWORD errMsgSize = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, error, LANG_USER_DEFAULT, win32ErrMsg, WIN32_ERR_MSG_SIZE, NULL);
    if (errMsgSize == 0)
    {
        if (GetLastError() != ERROR_MR_MID_NOT_FOUND)
        {
            if (sprintf(win32ErrMsg, "Could not get error message for 0x%lX", error) > 0)
            {
                return (char *)&win32ErrMsg;
            }
            else
            {
                return NULL;
            }
        }
        else
        {
            if (sprintf(win32ErrMsg, "Invalid Windows Error code (0x%lX)", error) > 0)
            {
                return (char *)&win32ErrMsg;
            }
            else
            {
                return NULL;
            }
        }
    }
    while (errMsgSize > 1 && isspace(win32ErrMsg[errMsgSize - 1]))
    {
        win32ErrMsg[--errMsgSize] = '\0';
    }
    return win32ErrMsg;
}

/**
 * @brief Open a directory of a given root.
 * @param root Root of the directory to open.
 * @param handle Handle to the directory to open.
 * @param data Pointer to the data within the directory.
 * @returns True if the directory can be opened, else false.
 */
static bool _open_windows(const char *root, HANDLE *handle, WIN32_FIND_DATA *data)
{
    size_t temp_mark = buffer_save();
    char *buffer = buffer_sprintf("%s\\*", root);
    HANDLE find = FindFirstFile(buffer, data);
    buffer_rewind(temp_mark);
    if (find == INVALID_HANDLE_VALUE) return false;
    *handle = find;
    return true;
}

/**
 * @brief Append a given root path to a given child path.
 * @param root Root to which to append.
 * @param child Child from which to append.
 * @return A formatted string containing the given root path and child path seperated by an operating system dependent seperator.
 */
static char *_append_path(const char *root, const char *child)
{
    size_t checkpoint = buffer_save();
    char *result = buffer_sprintf("%s%c%s", root, PATH_SEPERATOR, child);
    buffer_rewind(checkpoint);
    return result;
}

/**
 * @brief Obtain the entries at the given array root.
 * @param files Array of files to which to append.
 * @param handle Handle to the directory to open.
 * @param data Pointer to the data within the directory.
 * @returns True if the entries contained at the directory root can be obtained, else false.
 */
static bool _get_entries_windows(files_t *files, HANDLE handle, WIN32_FIND_DATA *data)
{
    if (!FindNextFile(handle, data)) return false;
    if (strcmp(data->cFileName, ".") == 0) return true;
    else if (strcmp(data->cFileName, "..") == 0) return true;
    files_append(files, buffer_duplicate(_append_path(files->root, data->cFileName)));
    return true;
}

/**
 * @brief Walk the directory of the given files' root.
 * @param files Array from which to walk.
 * @returns True if the directory can be walked, else false.
 */
static bool _walk_directory_windows(files_t *files)
{
    bool result = true;
    HANDLE handle;
    WIN32_FIND_DATA data;
    if (!_open_windows(files->root, &handle, &data)) return false;
    for (;;)
    {
        if (!_get_entries_windows(files, handle, &data))
        {
            if (GetLastError() == ERROR_NO_MORE_FILES) break;
            result = false;
            break;
        }
    }
    return result;
}

#else

/*
* @brief OS-dependant path seperator.
*/
#define PATH_SEPERATOR "/"

#endif // _WIN32

/**
 * @brief Extend the array of files.
 * @param files Array of files to extend.
 * @returns True if the paths stored at the root can be added, else false.
 */
bool files_fill(files_t *files)
{
#ifdef _WIN32
    bool result = _walk_directory_windows(files);
    buffer_reset();
#else
#error "NotImplementedError: The linux version of `files_fill()` has not been defined yet."
#endif // _WIN32
    return result;
}

#endif // FILES_IMPLEMENTATION