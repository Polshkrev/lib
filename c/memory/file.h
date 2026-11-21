#ifndef FILE_H_
#define FILE_H_

#include <stdbool.h> // bool
#include <stddef.h> // size_t, NULL

#ifdef _WIN32
    #include <handleapi.h> // HANDLE

    /**
     * @brief Representation of a file descriptor.
     */
    typedef HANDLE descriptor_t;

    /**
     * @brief Representation of an invalid file descriptor.
     */
    #define INVALID_DESCRIPTOR INVALID_HANDLE_VALUE
#else
    /**
     * @brief Representation of a file descriptor.
     */
    typedef int descriptor_t;

    /**
     * @brief Representation of an invalid file descriptor.
     */
    #define INVALID_DESCRIPTOR ((descriptor_t)-1)
#endif // _WIN32

#include "protection.h" // memory_protection_t, memory_protect, protection_to_native, MEMORY_PROTECTION_READ, MEMORY_PROTECTION_WRITE, MEMORY_PROTECTION_EXECUTE

#include "../attributes.h" // NODISCARD, UNUSED

/**
 * @brief Map a given file descriptor into memory.
 * @param file Descriptor to the targeted file.
 * @param protection Protection mask of the desired access.
 * @returns A `virtually-allocateed` `page-aligned` mapping of the given file descriptor.
 * @retval If the given file descriptor can `not` be mapped, `NULL` is returned.
 * @note The caller of this function is responsible for closing the given descriptor.
 */
NODISCARD void *file_allocate(descriptor_t file, memory_protection_t protection);

/**
 * @brief Free a given region of `file-backed` memory.
 * @param memory Region of memory to free.
 * @param size Size of the region of memory to free.
 * @returns True if the given memory is able to be freed, else false.
 */
bool file_free(void *memory, size_t size);

/**
 * @brief Obtain a file `file` from a given filename.
 * @param filename The name of the targeted file.
 * @param protection Targeted protection of the file.
 * @returns A `descriptor` to the file with the given filename.
 */
NODISCARD descriptor_t file_handle(const char *filename, memory_protection_t protection);

/**
 * @brief Obtain the size of the given file descriptor.
 * @param file Descriptor of the file whose size will be determined.
 * @returns The size of the given file descriptor.
 */
NODISCARD size_t file_size(descriptor_t file);

/**
 * @brief Close a given file descriptor.
 * @param file Descriptor to close.
 * @returns True if the given file descriptor can be closed, else false.
 */
bool file_close(descriptor_t file);

#endif // FILE_H_

#ifdef FILE_IMPLEMENTATION

#include <stdint.h> // uint64_t

#ifdef _WIN32
    #include <windef.h> // HWND
    #include <winbase.h> // CreateFileMapping
    #include <minwindef.h> // DWORD
    #include <handleapi.h> // CloseHandle
    #include <fileapi.h> // CreateFile, FILE_ATTRIBUTE_NORMAL
    #include <memoryapi.h> // MapViewOfFile, FILE_MAP_READ
    #include <winnt.h> // HANDLE, GENERIC_READ, GENERIC_WRITE, FILE_SHARE_READ, FILE_SHARE_WRITE
#else
    #include <fcntl.h> // open
    #include <unistd.h> // fstat, close
    #include <sys/mman.h> // mmap, munmap
    #include <sys/stat.h> // struct stat, stat
#endif // _WIN32

#ifdef _WIN32
/**
 * @brief Obtain a native handle of access protections based on the given protection mask.
 * @param protection Protection mask of the desired access.
 * @returns A native handle of access protections based on the given protection mask.
 */
NODISCARD static DWORD __handle_access_to_native(memory_protection_t protection)
{
    bool read = protection & MEMORY_PROTECTION_READ;
    bool write = protection & MEMORY_PROTECTION_WRITE;
    bool execute = protection & MEMORY_PROTECTION_EXECUTE;
    return (read ? GENERIC_READ : 0) | (write ? GENERIC_WRITE : 0) | (execute ? GENERIC_EXECUTE : 0);
}

/**
 * @brief Obtain a native handle of file mapping access protections based on the given protection mask.
 * @param protection Protection mask of the desired access.
 * @returns A native handle of file mapping access protections based on the given protection mask.
 */
NODISCARD static DWORD __map_access_to_native(memory_protection_t protection)
{
    bool read = protection & MEMORY_PROTECTION_READ;
    bool write = protection & MEMORY_PROTECTION_WRITE;
    bool execute = protection & MEMORY_PROTECTION_EXECUTE;
    return (read ? FILE_MAP_READ : 0) | (write ? FILE_MAP_WRITE : 0) | (execute ? FILE_MAP_EXECUTE : 0);
}
#endif // _WIN32

/**
 * @brief Obtain a file `file` from a given filename.
 * @param filename The name of the targeted file.
 * @param protection Targeted protection of the file.
 * @returns A `descriptor` to the file with the given filename.
 */
NODISCARD descriptor_t file_handle(const char *filename, memory_protection_t protection)
{
    bool write = protection & MEMORY_PROTECTION_WRITE;
#ifdef _WIN32
    DWORD access = __handle_access_to_native(protection);
    DWORD permission = write ? FILE_SHARE_READ | FILE_SHARE_WRITE : FILE_SHARE_READ;
    return CreateFile(filename, access, permission, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
#else
    int flags = write ? O_RDWR : O_RDONLY;
    return open(filename, flags);
#endif
}

/**
 * @brief Close a given file descriptor.
 * @param file Descriptor to close.
 * @returns True if the given file descriptor can be closed, else false.
 */
bool file_close(descriptor_t file)
{
#ifdef _WIN32
    return CloseHandle(file) != 0;
#else
    return close(file) == 0;
#endif // _WIN32
}

/**
 * @brief Obtain the size of the given file descriptor.
 * @param file Descriptor of the file whose size will be determined.
 * @returns The size of the given file descriptor.
 */
NODISCARD size_t file_size(descriptor_t file)
{
    if (INVALID_DESCRIPTOR == file) return SIZE_MAX;
#ifdef _WIN32
    LARGE_INTEGER size;
    if (!GetFileSizeEx(file, &size)) return SIZE_MAX;
    if (size.QuadPart < 0 || (uint64_t)size.QuadPart > SIZE_MAX) return SIZE_MAX;
    return (size_t)size.QuadPart;
#else
    struct stat status;
    if (fstat(file, &status) != 0) return SIZE_MAX;
    if (status.st_size < 0 || (uintmax_t)status.st_size > SIZE_MAX) return SIZE_MAX;
    return (size_t)status.st_size;
#endif
}

/**
 * @brief Map a given file descriptor into memory.
 * @param file Descriptor to the targeted file.
 * @param protection Protection mask of the desired access.
 * @returns A `virtually-allocateed` `page-aligned` mapping of the given file descriptor.
 * @retval If the given file descriptor can `not` be mapped, `NULL` is returned.
 * @note The caller of this function is responsible for closing the given descriptor.
 */
NODISCARD void *file_allocate(descriptor_t file, memory_protection_t protection)
{
    if (INVALID_DESCRIPTOR == file) return NULL;
#ifdef _WIN32
    DWORD access = __map_access_to_native(protection);
    DWORD permission = protection_to_native(protection);
    HANDLE mapping = CreateFileMapping(file, NULL, permission, 0, 0, NULL);
    if (NULL == mapping) return NULL;
    void *view = MapViewOfFile(mapping, access, 0, 0, 0);
    if (NULL == view)
    {
        CloseHandle(mapping);
        return NULL;
    }
    CloseHandle(mapping);
#else
    int permission = protection_to_native(protection);
    void *view = mmap(NULL, file_size(file), permission, MAP_SHARED | MAP_ANONOMOUS, file, 0);
    if (MAP_FAILED == view) return NULL;
#endif
    return view;
}

/**
 * @brief Free a given region of `file-backed` memory.
 * @param memory Region of memory to free.
 * @param size Size of the region of memory to free.
 * @returns True if the given memory is able to be freed, else false.
 */
bool file_free(void *memory, size_t size)
{
    if (NULL == memory) return true;
#ifdef _WIN32
    UNUSED(size);
    return UnmapViewOfFile(memory) != 0;
#else
    if (size == 0) return false;
    return munmap(memory, size) == 0;
#endif // _WIN32
}

#endif // FILE_IMPLEMENTATION