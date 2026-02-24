#ifndef ENTRY_H
#define ENTRY_H

#if defined(__cplusplus)
extern "C" {
#endif

#define PATH_IMPLEMENTATION
#include "path.h" // path_t, passtr, path_exists, path_delete

#define STRING_BUILDER_IMPLEMENTATION
#include "collections/string_builder.h" // string_builder_t, sring_builder_init, string_builder_append, string_builder_empty, string_builder_data, string_builder_delete

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
// bool entry_write_to_path(entry_t *path, string_builder_t *content);

/**
 * @brief Create a persistant file on the filesystem.
 * @param entry Entry to create.
 * @returns True if the entry's path can be created on th filesystem, else false.
 */
bool entry_touch(const entry_t *entry);

/**
 * @brief Create a persistant directory on the filesystem.
 * @param entry Entry to create.
 * @returns True if the entry's path can be created on th filesystem, else false.
 */
bool entry_make_directory(const entry_t *entry);

/**
 * @brief Generic entry creation dispatch.
 * @param entry Entry to create.
 * @returns True if the entry's path can be created on th filesystem, else false.
 */
bool entry_create(const entry_t *entry);

/**
 * @brief Generic entry removal dispatch.
 * @param entry Entry to remove.
 * @returns True if the entry's path can be removed, else false.
 */
bool entry_remove(const entry_t *entry);

/**
 * @brief Remove directory on the filesystem.
 * @param entry Entry to remove.
 * @returns True if the entry's path can be removed, else false.
 */
bool entry_remove_directory(const entry_t *entry);

/**
 * @brief Remove a file on the filesystem.
 * @param entry Entry to remove.
 * @returns True if the entry's path can be removed, else false.
 */
bool entry_remove_file(const entry_t *entry);

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
 * @brief Read a file's content into an entry.
 * @param entry Entry to which to set the read content.
 * @returns True if the entry's path can be successfully read, else false.
 */
bool entry_read(entry_t *entry)
{
    if (!path_exists(entry->path)) return false;
    FILE *file = fopen(passtr(entry->path), "r");
    if (NULL == file) return false;
    // printf("file fail.\n");
    // if (NULL != entry->content) return false;
    char character;
    while ((character = fgetc(file)) != EOF)
    {
        string_builder_append(entry->content, character);
        // printf("%c", character);
        // sprintf(entry->content, "%c", character);
    }
    // printf("fists %s\n", string_builder_data(entry->content));
    fclose(file);
    string_builder_fit(entry->content);
    // printf("%s\n", string_builder_data(entry->content));
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
    if (NULL == file) return false;
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
    if (!path_exists(source->path)) return false;
    else if (path_exists(destination->path)) return false;
    else if (source->type != FILE_TYPE) return false;
    else if (destination->type != FILE_TYPE) return false;
    else if (string_builder_empty(source->content)) if (!entry_read(source)) return false;
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
bool entry_touch(const entry_t *entry)
{
    if (path_exists(entry->path)) return false;
    else if (entry->type != FILE_TYPE) return false;
    FILE *file = fopen(passtr(entry->path), "w");
    if (NULL == file) return false;
    fclose(file);
}

/**
 * @brief Create a persistant directory on the filesystem.
 * @param entry Entry to create.
 * @returns True if the entry's path can be created on th filesystem, else false.
 */
bool entry_make_directory(const entry_t *entry)
{
    if (path_exists(entry->path)) return false;
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
bool entry_create(const entry_t *entry)
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
bool entry_remove_directory(const entry_t *entry)
{
    if (!path_exists(entry->path)) return false;
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
bool entry_remove_file(const entry_t *entry)
{
    if (!path_exists(entry->path)) return false;
    else if (entry->type != FILE_TYPE) return false;
    return remove(passtr(entry->path)) == 0;
}

/**
 * @brief Generic entry removal dispatch.
 * @param entry Entry to remove.
 * @returns True if the entry's path can be removed, else false.
 */
bool entry_remove(const entry_t *entry)
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