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
files_t files_init(const char *root);

/**
 * @brief Construct a new dynamic array of files with a given root and initial capacity.
 * @param root Root of the directory where the files are located.
 * @param capacity Capacity with which to initialize the files.
 * @returns A new dynamic array of files on the filesystem with a given root and capacity.
 * @exception If the array can not be allocated, an `AllocationError` is printed to `stderr` and the programme exits.
 */
files_t files_init_with_capacity(const char *root, size_t capacity);

/**
 * @brief Append a new filepath to the array of files.
 * @param files Array of files to which to append.
 * @param entry Path to append.
 * @exception If the array of files can not be reallocated, an `AllocationError` is printed to `stderr` and the programme exits after the given files are deallocated.
 * @exception If the entry can not be allocated, an `AllocationError` is printed to `stderr` and the programme exits after the given files are deallocated.
 */
void files_append(files_t *files, const char *entry);

/**
 * @brief Obtain a path at a given index within the array of files.
 * @param files Array of files from which to obtain a path.
 * @param index Index of the path to obtain.
 * @returns A pointer to the path within the array of files at the given index.
 * @returns If the given index is greater than or equal to the size of the array of files, `NULL` is returned.
 */
char **files_at(const files_t *files, size_t index);

/**
 * @brief Resize the array of files by a scaling factor of two.
 * @param files Array of files to resize.
 * @exception If the array of files can not be reallocated, an `AllocationError` is printed to `stderr` and the programme exits after the given files are deallocated.
 */
void files_resize(files_t *files);

/**
 * @brief Resize the array of files by a given scaling factor.
 * @param files Array of files to resize.
 * @param scaler Scaling factor by which to resize the array of files.
 * @exception If the array of files can not be reallocated, an `AllocationError` is printed to `stderr` and the programme exits after the given files are deallocated.
 */
void files_resize_by(files_t *files, size_t scaler);

/**
 * @brief Reduce the capacity of the array to its current size.
 * @param files Array to fit.
 */
void files_fit(files_t *files);

/**
 * @brief Fill the array of files with the files contained at its root.
 * @param files Array of files to fill.
 * @returns True if the files contained at the root can be obtained, else false.
 */
bool files_fill(files_t *files);

/**
 * @brief Deallocate the array of files.
 * @param files Array to deallocate.
 */
void files_delete(files_t *files);

#if defined(__cplusplus)
}
#endif

#endif // FILES_H

#ifdef FILES_IMPLEMENTATION

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdio.h> // fprintf, stderr, snprintf
#include <stdlib.h> // malloc, realloc, free, exit, NULL
#include <string.h> // strlen, strcmp, strcpy_s
#include <stdint.h> // SIZE_MAX

#ifdef _WIN32
    #include <minwindef.h> // MAX_PATH, HANDLE
    #include <handleapi.h> // INVALID_HANDLE_VALUE, FindClose
    #include <fileapi.h> // FindFirstFile, FindNextFile, WIN32_FIND_DATA, FILE_ATTRIBUTE_DIRECTORY
    #include <strsafe.h> // StringCchCopy, StringCchCat
#elif defined(__linux__)
    #include <dirent.h> // DIR, opendir, readdir, closedir, struct dirent
    #include <limits.h> // PATH_MAX
    #include <sys/stat.h> // stat, struct stat, S_ISDIR
#else
    #error "NotImplementedError: The current platform is not supported."
#endif // _WIN32

#ifndef FILES_INITIAL_CAPACITY 
#define FILES_INITIAL_CAPACITY 256
#endif // FILES_INITIAL_CAPACITY

/**
 * @brief Construct a new dynamic array of files with a given root.
 * @param root Root of the directory where the files are located.
 * @returns A new dynamic array of files on the filesystem with a given root.
 * @exception If the array can not be allocated, an `AllocationError` is printed to `stderr` and the programme exits.
 */
files_t files_init(const char *root)
{
    return files_init_with_capacity(root, FILES_INITIAL_CAPACITY);
}

/**
 * @brief Construct a new dynamic array of files with a given root and initial capacity.
 * @param root Root of the directory where the files are located.
 * @param capacity Capacity with which to initialize the files.
 * @returns A new dynamic array of files on the filesystem with a given root and capacity.
 * @exception If the array can not be allocated, an `AllocationError` is printed to `stderr` and the programme exits.
 */
files_t files_init_with_capacity(const char *root, size_t capacity)
{
    char **files = (char **)malloc(capacity * sizeof(char *));
    if (NULL == files)
    {
        fprintf(stderr, "AllocationError: Can not allocate enough memory for the array of files.\n");
        exit(1);
    }
    return (files_t)
    {
        .root = root,
        .capacity = capacity,
        .size = 0,
        .files = files
    };
}

/**
 * @brief Append a new filepath to the array of files.
 * @param files Array of files to which to append.
 * @param entry Path to append.
 * @exception If the array of files can not be reallocated, an `AllocationError` is printed to `stderr` and the programme exits after the given files are deallocated.
 * @exception If the entry can not be allocated, an `AllocationError` is printed to `stderr` and the programme exits after the given files are deallocated.
 */
void files_append(files_t *files, const char *entry)
{
    if (files->size >= files->capacity) files_resize(files);

    size_t entry_size = strlen(entry) + 1;

    files->files[files->size] = (char *)malloc(entry_size);

    if (NULL == files->files[files->size])
    {
        fprintf(stderr, "AllocationError: Can not allocate enough memory for entry: '%s'.\n", entry);
        files_delete(files);
        exit(1);
    }
    else if (strcpy_s(files->files[files->size], entry_size, entry) != 0)
    {
        fprintf(stderr, "RuntimeError: Can not copy entry: '%s'.\n", entry);
        files_delete(files);
        exit(1);
    }

    files->size++;
}

/**
 * @brief Obtain a path at a given index within the array of files.
 * @param files Array of files from which to obtain a path.
 * @param index Index of the path to obtain.
 * @returns A pointer to the path within the array of files at the given index.
 * @returns If the given index is greater than or equal to the size of the array of files, `NULL` is returned.
 */
char **files_at(const files_t *files, size_t index)
{
    if (index >= files->size) return NULL;
    return &files->files[index];
}

/**
 * @brief Resize the array of files by a scaling factor of two.
 * @param files Array of files to resize.
 * @exception If the array of files can not be reallocated, an `AllocationError` is printed to `stderr` and the programme exits after the given files are deallocated.
 */
void files_resize(files_t *files)
{
    files_resize_by(files, 2);
}

/**
 * @brief Safely divide a given dividend size by a given divisor size.
 * @param dividend Dividend to which to divide.
 * @param divisor Divisor from which to divide.
 * @returns The quotient of the given sizes.
 * @exception If the given divisor is zero, a `ZeroDivisionError` is printed to `stderr` and the programme exits.
 */
static size_t __safe_file_size_divide(size_t dividend, size_t divisor)
{
    if (divisor == 0)
    {
        fprintf(stderr, "ZeroDivisionError: Can not divide by zero.\n");
        exit(1);
    }
    return (dividend / divisor);
}

/**
 * @brief Resize the array of files by a given scaling factor.
 * @param files Array of files to resize.
 * @param scaler Scaling factor by which to resize the array of files.
 * @exception If the array of files can not be reallocated, an `AllocationError` is printed to `stderr` and the programme exits after the given files are deallocated.
 */
void files_resize_by(files_t *files, size_t scaler)
{
    if (scaler < 2) return;
    else if (files->capacity > __safe_file_size_divide(SIZE_MAX, scaler))
    {
        fprintf(stderr, "OverflowError: The capacity has overflown its type.\n");
        files_delete(files);
        exit(1);
    }
    files->capacity *= scaler;
    files->files = (char **)realloc(files->files, files->capacity * sizeof(char *));
    if (NULL == files->files)
    {
        fprintf(stderr, "AllocationError: Can not allocate files array.\n");
        files_delete(files);
        exit(1);
    }
}

/**
 * @brief Reduce the capacity of the array to its current size.
 * @param files Array to fit.
 */
void files_fit(files_t *files)
{
    if (files->size == files->capacity) return;

    if (files->size == 0)
    {
        free(files->files);
        files->files = NULL;
        files->capacity = 0;
        return;
    }

    files->files = realloc(files->files, files->size * sizeof(char *));
    if (NULL == files->files)
    {
        fprintf(stderr, "AllocationError: Can not fit files array.\n");
        files_delete(files);
        exit(1);
    }

    files->capacity = files->size;
}

/**
 * @brief Deallocate the array of files.
 * @param files Array to deallocate.
 */
void files_delete(files_t *files)
{
    if (!files->files) return;
    for (size_t i = 0; i < files->size; ++i)
    {
        free(files->files[i]);
        files->files[i] = NULL;
    }
    free(files->files);
    files->files = NULL;
    files->root = NULL;
    files->size = 0;
    files->capacity = 0;
}

#ifdef _WIN32

/**
 * @brief Obtain the files contained at the given directory path.
 * @param files Array of files to which to append.
 * @param path Directory path from which to obtain files.
 * @returns True if the files contained at the given path can be obtained, else false.
 */
static bool _get_entries_windows(files_t *files, const char *path)
{
    char search_path[MAX_PATH];
    WIN32_FIND_DATA data;

    StringCchCopy(search_path, MAX_PATH, path);
    StringCchCat(search_path, MAX_PATH, "\\*");

    HANDLE find = FindFirstFile(search_path, &data);

    if (INVALID_HANDLE_VALUE == find) return false;

    do
    {
        if (strcmp(data.cFileName, ".") == 0 || strcmp(data.cFileName, "..") == 0) continue;

        char full_path[MAX_PATH];

        StringCchCopy(full_path, MAX_PATH, path);
        StringCchCat(full_path, MAX_PATH, "\\");
        StringCchCat(full_path, MAX_PATH, data.cFileName);

        if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            if (!_get_entries_windows(files, full_path))
            {
                FindClose(find);
                return false;
            }
        }
        else
        {
            files_append(files, full_path);
        }

    } while (FindNextFile(find, &data) != 0);

    FindClose(find);

    return true;
}

#else

/**
 * @brief Obtain the files contained at the given directory path.
 * @param files Array of files to which to append.
 * @param path Directory path from which to obtain files.
 * @returns True if the files contained at the given path can be obtained, else false.
 */
static bool _get_entries_linux(files_t *files, const char *path)
{
    DIR *directory = opendir(path);

    if (NULL == directory) return false;

    struct dirent *entry;

    while (NULL != (entry = readdir(directory)))
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        char full_path[PATH_MAX];

        int result = snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        if (result < 0 || (size_t)result >= sizeof(full_path))
        {
            closedir(directory);
            return false;
        }

        struct stat information;

        if (stat(full_path, &information) != 0)
        {
            closedir(directory);
            return false;
        }
        else if (entry->d_type == DT_DIR)
        {
            if (!_get_entries_linux(files, full_path))
            {
                closedir(directory);
                return false;
            }
        }
        else
        {
            files_append(files, full_path);
        }
    }

    closedir(directory);

    return true;
}
#endif // _WIN32

/**
 * @brief Fill the array of files with the files contained at its root.
 * @param files Array of files to fill.
 * @returns True if the files contained at the root can be obtained, else false.
 */
bool files_fill(files_t *files)
{
#ifdef _WIN32
    bool result = _get_entries_windows(files, files->root);
#else
    bool result = _get_entries_linux(files, files->root);
#endif // _WIN32
    return result;
}

#endif // FILES_IMPLEMENTATION