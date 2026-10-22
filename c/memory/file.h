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

/**
 * @brief Map a given file descriptor into memory.
 * @param file Descriptor to the targeted file.
 * @param write Sentinel value of writability for the targeted file.
 * @returns A `virtually-allocateed` `page-aligned` mapping of the given file descriptor.
 * @retval If the given file descriptor can `not` be mapped, `NULL` is returned.
 * @note The caller of this function is responsible for closing the given descriptor.
 */
void *file_allocate(descriptor_t file, bool write);

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
 * @param write Sentinel value of writability for the targeted file.
 * @returns A `descriptor` to the file with the given filename.
 */
descriptor_t file_handle(const char *filename, bool write);

/**
 * @brief Obtain the size of the given file descriptor.
 * @param file Descriptor of the file whose size will be determined.
 * @returns The size of the given file descriptor.
 */
size_t file_size(descriptor_t file);

#endif // FILE_H_

#ifdef FILE_IMPLEMENTATION

#include <string.h> // memcpy

#ifdef _WIN32
    #include <windef.h> // HWND
    #include <winbase.h> // CreateFileMapping
    #include <minwindef.h> // DWORD
    #include <handleapi.h> // CloseHandle
    #include <fileapi.h> // CreateFile, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL
    #include <memoryapi.h> // MapViewOfFile, FILE_MAP_ALL_ACCESS, FILE_MAP_READ
    #include <winnt.h> // HANDLE, GENERIC_READ, GENERIC_WRITE, FILE_SHARE_READ, FILE_SHARE_WRITE, PAGE_READWRITE, PAGE_READONLY
#else
    #include <fcntl.h> // open
    #include <sys/mman.h> // mmap, munmap
    #include <sys/stat.h> // struct stat, stat
#endif // _WIN32

/**
 * @brief Obtain a file `file` from a given filename.
 * @param filename The name of the targeted file.
 * @param write Sentinel value of writability for the targeted file.
 * @returns A `descriptor` to the file with the given filename.
 */
descriptor_t file_handle(const char *filename, bool write)
{
#ifdef _WIN32
    DWORD access = write ? GENERIC_READ | GENERIC_WRITE : GENERIC_READ;
    DWORD permission = write ? FILE_SHARE_READ | FILE_SHARE_WRITE : FILE_SHARE_READ;
    return CreateFile(filename, access, permission, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
#else
    int permission = write ? O_RDWR : O_RDONLY;
    return open(filename, permission);
#endif // _WIN32
}

/**
 * @brief Obtain the size of the given file descriptor.
 * @param file Descriptor of the file whose size will be determined.
 * @returns The size of the given file descriptor.
 */
size_t file_size(descriptor_t file)
{
#ifdef _WIN32
    DWORD size = GetFileSize(file, 0);
    if (size == INVALID_FILE_SIZE) return SIZE_MAX;
    return (size_t)size;
#else
    struct stat statbuf;
    int result = stat(file, &statbuf);
    if (result != 0) return SIZE_MAX;
    return statbuf.st_size;
#endif // _WIN32
}

/**
 * @brief Map a given file descriptor into memory.
 * @param file Descriptor to the targeted file.
 * @param write Sentinel value of writability for the targeted file.
 * @returns A `virtually-allocateed` `page-aligned` mapping of the given file descriptor.
 * @retval If the given file descriptor can `not` be mapped, `NULL` is returned.
 * @note The caller of this function is responsible for closing the given descriptor.
 */
void *file_allocate(descriptor_t file, bool write)
{
    if (INVALID_DESCRIPTOR == file) return NULL;
#ifdef _WIN32
    DWORD permision = write ? PAGE_READWRITE : PAGE_READONLY;
    HANDLE mapping = CreateFileMapping(file, NULL, permision, 0, 0, NULL);
    if (NULL == mapping) return NULL;
    DWORD access = write ? FILE_MAP_ALL_ACCESS : FILE_MAP_READ;
    void *view = MapViewOfFile(mapping, access, 0, 0, 0);
    if (NULL == view)
    {
        CloseHandle(mapping);
        return NULL;
    }
    CloseHandle(mapping);
    return view;
#else
    int permission = write ? PROT_READ | PROT_WRITE : PROT_READ;
    void *pointer = mmap(NULL, size, permision, MAP_PRIVATE | MAP_ANONYMOUS, file, 0);
    if (MAP_FAILED == pointer) return NULL;
    return pointer;
#endif // _WIN32
}

/**
 * @brief Free a given region of `file-backed` memory.
 * @param memory Region of memory to free.
 * @param size Size of the region of memory to free.
 * @returns True if the given memory is able to be freed, else false.
 */
bool file_free(void *memory, size_t size)
{
    if (NULL == memory || NULL == memory) return true;
#ifdef _WIN32
    (void)size;
    return UnmapViewOfFile(memory) != 0;
#else
    return mumap(memory, size) == 0;
#endif // _WIN32
}

#endif // FILE_IMPLEMENTATION