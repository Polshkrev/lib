#ifndef ENTRY_H
#define ENTRY_H

#if defined(__cplusplus)
extern "C" {
#endif

#define PATH_IMPLEMENTATION
#include "../path.h" // path_t, passtr, path_exists, path_delete

#define STRING_BUILDER_IMPLEMENTATION
#include "string_builder.h" // string_builder_t, sring_builder_init, string_builder_append, string_builder_empty, string_builder_data, string_builder_delete

#ifdef _WIN32
#include <direct.h> // _mkdir, remove
#else
#include <unistd.h> // mkdir, rmdir
#endif // _WIN32

/**
 * @brief Representation of a type of an entry.
 */
typedef enum
{
    DIRECTORY_TYPE,
    FILE_TYPE,
} file_type_t;

/**
 * @brief Representation of an entry on the file system.
 */
typedef struct
{
    file_type_t type;
    path_t *path;
    string_builder_t *content;
} entry_t;

/**
 * @brief Construct a new entry of a given path.
 * @param path Path of the entry.
 * @returns A new entry of the given path with its type set to `FILE_TYPE` and its content an empty `string_builder_t`.
 * @exception If a new entry can not be allocated, an `AllocationError` is printed to `stderr` and the programme exists.
 */
entry_t *entry_init(path_t *path);

/**
 * @brief Read a file's content into an entry.
 * @param entry Entry to which to set the read content.
 * @returns True if the entry's path can be successfully read, else false.
 */
bool entry_read(entry_t *entry);
// bool entry_read_content_to_path(path_t *path, string_builder_t *content);

/**
 * @brief Write a given entry's content to a persistant file.
 * @param entry Entry from which to write.
 * @returns True if the entry can be written to a file, else false.
 */
bool entry_write(entry_t *entry);

/**
 * @brief Write given content to a given path.
 * @param path Path to which to write.
 * @param content Content from which to write.
 * @returns True if the given content can be written to the given path, else false.
 */
bool entry_write_content_to_path(path_t *path, string_builder_t *content);

/**
 * @brief Create a persistant file on the filesystem.
 * @param entry Entry to create.
 * @returns True if the entry's path can be created on th filesystem, else false.
 */
bool entry_touch(entry_t *entry);

/**
 * @brief Create a persistant directory on the filesystem.
 * @param entry Entry to create.
 * @returns True if the entry's path can be created on th filesystem, else false.
 */
bool entry_make_directory(entry_t *entry);

/**
 * @brief Generic entry creation dispatch.
 * @param entry Entry to create.
 * @returns True if the entry's path can be created on th filesystem, else false.
 */
bool entry_create(entry_t *entry);

/**
 * @brief Generic entry removal dispatch.
 * @param entry Entry to remove.
 * @returns True if the entry's path can be removed, else false.
 */
bool entry_remove(entry_t *entry);

/**
 * @brief Remove directory on the filesystem.
 * @param entry Entry to remove.
 * @returns True if the entry's path can be removed, else false.
 */
bool entry_remove_directory(entry_t *entry);

/**
 * @brief Remove a file on the filesystem.
 * @param entry Entry to remove.
 * @returns True if the entry's path can be removed, else false.
 */
bool entry_remove_file(entry_t *entry);

/**
 * @brief Copy a file entry from a given source to a given destination.
 * @param source Source entry from which to copy.
 * @param destination Destination entry to which to copy.
 * @return True if the source entry can be copied to the destination entry, else false.
 */
bool entry_copy(entry_t *source, entry_t *destination);

/**
 * @brief Move a file entry from a given source to a given destination.
 * @param source Source entry from which to move.
 * @param destination Destination entry to which to move.
 * @return True if the source entry can be moved to the destination entry, else false.
 */
bool entry_move(entry_t *source, entry_t *destination);

/**
 * @brief Obtain the size of the entry in bytes.
 * @param entry Entry from which to obtain the size.
 * @returns The size of the entry.
 */
size_t entry_size(entry_t *entry);

/**
 * @brief Deallocate an entry.
 * @param entry Entry to deallocate.
 */
void entry_delete(entry_t *entry);

#if defined(__cplusplus)
}
#endif

#endif // ENTRY_H

#ifdef ENTRY_IMPLEMENTATION

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Construct a new entry of a given path.
 * @param path Path of the entry.
 * @returns A new entry of the given path with its type set to `FILE_TYPE` and its content an empty `string_builder_t`.
 * @exception If a new entry can not be allocated, an `AllocationError` is printed to `stderr` and the programme exists.
 */
entry_t *entry_init(path_t *path)
{
    entry_t *entry = (entry_t *)malloc(sizeof(entry_t));
    if (NULL == entry)
    {
        fprintf(stderr, "AllocationError: Can not allocate a new entry.\n");
        exit(1);
    }
    entry->path = path;
    entry->content = string_builder_init();
    entry->type = FILE_TYPE;
    return entry;
}

/**
 * @brief Safely convert a given long value to a size.
 * @param value Long value to convert.
 * @param result Out result parametre.
 * @returns True if the value can be converted, else false.
 */
static bool _safe_long_to_size(long value, size_t *result)
{
    if (value < 0) return false;
    *result = (size_t)value;
    return true;
}

/**
 * @brief Obtain the size of a given file pointer.
 * @param file Opened file pointer of which to determine the size.
 * @param result Out result parametre.
 * @returns True if the size can be obtained, else false.
 */
static bool _get_file_size(FILE *file, size_t *result) {
    long current_pos = ftell(file);
    if (fseek(file, 0L, SEEK_END) != 0) return false;
    long size = ftell(file);
    if (size == -1) return false;
    else if (fseek(file, current_pos, SEEK_SET) != 0) return false;
    return _safe_long_to_size(size, result);
}

/**
 * @brief Read a file's content into an entry.
 * @param entry Entry to which to set the read content.
 * @returns True if the entry's path can be successfully read, else false.
 */
bool entry_read(entry_t *entry)
{
    if (!path_exists(entry->path))
    {
        fprintf(stderr, "FileNotFoundError: Can not find file %s.\n", passtr(entry->path));
        entry_delete(entry);
        exit(1);
    }
    FILE *file = fopen(passtr(entry->path), "r");
    if (NULL == file)
    {
        fprintf(stderr, "IOError: Can not readfile: %s.\n", passtr(entry->path));
        entry_delete(entry);
        exit(1);
    };
    char character;
    while ((character = fgetc(file)) != EOF)
    {
        string_builder_append(entry->content, character);
    }
    fclose(file);
    string_builder_fit(entry->content);
    return true;
}

/**
 * @brief Write given content to a given path.
 * @param path Path to which to write.
 * @param content Content from which to write.
 * @returns True if the given content can be written to the given path, else false.
 */
bool entry_write_content_to_path(path_t *path, string_builder_t *content)
{
    if (string_builder_empty(content)) return false;
    FILE *file = fopen(passtr(path), "w");
    if (NULL == file)
    {
        fprintf(stderr, "IOError: Can not open file: %s.\n", passtr(path));
        path_delete(path);
        string_builder_delete(content);
        exit(1);
    }
    fprintf(file, "%s\n", string_builder_data(content));
    fclose(file);
    return true;
}

/**
 * @brief Write a given entry's content to a persistant file.
 * @param entry Entry from which to write.
 * @returns True if the entry can be written to a file, else false.
 */
bool entry_write(entry_t *entry)
{
    return entry_write_content_to_path(entry->path, entry->content);
}

/**
 * @brief Copy a file entry from a given source to a given destination.
 * @param source Source entry from which to copy.
 * @param destination Destination entry to which to copy.
 * @return True if the source entry can be copied to the destination entry, else false.
 */
bool entry_copy(entry_t *source, entry_t *destination)
{
    if (!path_exists(source->path))
    {
        fprintf(stderr, "FileNotFoundError: Can not find file %s.\n", passtr(source->path));
        entry_delete(source);
        entry_delete(destination);
        exit(1);
    }
    else if (path_exists(destination->path))
    {
        fprintf(stderr, "FileExistsError: File '%s' already exists.\n", passtr(destination->path));
        entry_delete(source);
        entry_delete(destination);
        exit(1);
    }
    else if (source->type != FILE_TYPE) return false;
    else if (destination->type != FILE_TYPE) return false;
    else if (string_builder_empty(source->content))
    {
        if (!entry_read(source)) return false;
    }
    else if (!entry_write_content_to_path(destination->path, source->content)) return false;
    return true;
}

/**
 * @brief Move a file entry from a given source to a given destination.
 * @param source Source entry from which to move.
 * @param destination Destination entry to which to move.
 * @return True if the source entry can be moved to the destination entry, else false.
 */
bool entry_move(entry_t *source, entry_t *destination)
{
    if (!entry_copy(source, destination)) return false;
    return entry_remove(source);
}

/**
 * @brief Create a persistant file on the filesystem.
 * @param entry Entry to create.
 * @returns True if the entry's path can be created on th filesystem, else false.
 */
bool entry_touch(entry_t *entry)
{
    if (path_exists(entry->path))
    {
        fprintf(stderr, "FileExistsError: File '%s' already exists.\n", passtr(entry->path));
        entry_delete(entry);
        exit(1);
    }
    else if (entry->type != FILE_TYPE) return false;
    FILE *file = fopen(passtr(entry->path), "w");
    if (NULL == file)
    {
        fprintf(stderr, "IOError: Can not open file: %s.\n", passtr(entry->path));
        entry_delete(entry);
        exit(1);
    }
    fclose(file);
    return true;
}

/**
 * @brief Create a persistant directory on the filesystem.
 * @param entry Entry to create.
 * @returns True if the entry's path can be created on th filesystem, else false.
 */
bool entry_make_directory(entry_t *entry)
{
    if (path_exists(entry->path))
    {
        fprintf(stderr, "FileExistsError: File '%s' already exists.\n", passtr(entry->path));
        entry_delete(entry);
        exit(1);
    }
    else if (entry->type != DIRECTORY_TYPE) return false;
#ifdef _WIN32
    int result = _mkdir(passtr(entry->path));
#else
    int result = mkdir(passtr(entry->path), 0755);
#endif
    return result == 0;
}

/**
 * @brief Generic entry creation dispatch.
 * @param entry Entry to create.
 * @returns True if the entry's path can be created on th filesystem, else false.
 */
bool entry_create(entry_t *entry)
{
    switch (entry->type)
    {
        case DIRECTORY_TYPE:
        {
            return entry_make_directory(entry);
        } break;
        default:
        {
            return entry_touch(entry);
        } break;
    }
}

/**
 * @brief Remove directory on the filesystem.
 * @param entry Entry to remove.
 * @returns True if the entry's path can be removed, else false.
 */
bool entry_remove_directory(entry_t *entry)
{
    if (!path_exists(entry->path))
    {
        fprintf(stderr, "FileNotFoundError: Can not find file %s.\n", passtr(entry->path));
        entry_delete(entry);
        exit(1);
    }
    else if (entry->type != DIRECTORY_TYPE) return false;
#ifdef _WIN32
    int result = remove(passtr(entry->path));
#else
    int result = rmdir(passtr(entry->path));
#endif // _WIN32
    return result == 0;
}

/**
 * @brief Remove a file on the filesystem.
 * @param entry Entry to remove.
 * @returns True if the entry's path can be removed, else false.
 */
bool entry_remove_file(entry_t *entry)
{
    if (!path_exists(entry->path))
    {
        fprintf(stderr, "FileNotFoundError: Can not find file %s.\n", passtr(entry->path));
        entry_delete(entry);
        exit(1);
    }
    else if (entry->type != FILE_TYPE) return false;
    return remove(passtr(entry->path)) == 0;
}

/**
 * @brief Generic entry removal dispatch.
 * @param entry Entry to remove.
 * @returns True if the entry's path can be removed, else false.
 */
bool entry_remove(entry_t *entry)
{
    switch (entry->type)
    {
        case DIRECTORY_TYPE:
        {
            return entry_remove_directory(entry);
        } break;
        default:
        {
            return entry_remove_file(entry);
        } break;
    }
}

/**
 * @brief Obtain the size of the entry in bytes.
 * @param entry Entry from which to obtain the size.
 * @returns The size of the entry.
 */
size_t entry_size(entry_t *entry)
{
    if (!path_exists(entry->path))
    {
        if (!entry_create(entry))
        {
            fprintf(stderr, "IOError: Can not create file %s\n", passtr(entry->path));
            entry_delete(entry);
            exit(1);
        }
    }
    FILE *file = fopen(passtr(entry->path), "r");
    if (NULL == file)
    {
        fprintf(stderr, "IOError: Can not open file: %s\n", passtr(entry->path));
        entry_delete(entry);
        exit(1);
    }
    size_t result = 0; // Size of zero is valid.z
    if (!_get_file_size(file, &result))
    {
        fprintf(stderr, "IOError: Can not get size of file: %s.\n", passtr(entry->path));
        entry_delete(entry);
        exit(1);
    }
    return result;
}

/**
 * @brief Deallocate an entry.
 * @param entry Entry to deallocate.
 */
void entry_delete(entry_t *entry)
{
    if (!entry) return;
    path_delete(entry->path);
    string_builder_delete(entry->content);
    free(entry);
    entry = NULL;
}

#if defined(__cplusplus)
}
#endif

#endif // ENTRY_IMPLEMENTATION