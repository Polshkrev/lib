#ifndef HEAP_H_
#define HEAP_H_

#include <stdbool.h> // bool
#include <stddef.h> // size_t, NULL

#include "../attributes.h" // NODISCARD

/**
 * @brief Allocate a given size of memory on the heap.
 * @param size Size of the memory to allocate.
 * @param write Sentinel value of writability for the targeted file.
 * @returns A `virtually-allocateed` `page-aligned` region of memory of the given size.
 * @retval If the memory region can `not` be allocated, `NULL` is returned.
 */
NODISCARD void *heap_allocate(size_t size, bool write);

/**
 * @brief Reallocate a given region of `heap-backed` memory.
 * @param memory Memory to reallocate.
 * @param old_size Original size of the region of memory.
 * @param new_size Desired size of the new region of memory.
 * @param write Sentinel value of writability for the targeted region of memory.
 * @returns A pointer to the new region of memory of the given `new size`.
 * @retval If the given `memory` is `NULL`, it is the same as a `new allocation`.
 * @retval If the given `old size` is `equal to` the `new size`, the `original` region of memory is returned.
 * @retval If the given `new size` is `zero`, the given region of memory is `deallocated`, and `NULL` is returned.
 * @retval If the `original` memory can not be reallocated, `NULL` is returned.
 */
NODISCARD bool heap_reallocate(void *memory, size_t old_size, size_t new_size, bool write);

/**
 * @brief Free a given region of `heap-backed` memory.
 * @param memory Region of memory to free.
 * @param size Size of the region of memory to free.
 * @returns True if the given memory is able to be freed, else false.
 */
bool heap_free(void *memory, size_t size);

#endif // HEAP_H_

#ifdef HEAP_IMPLEMENTATION

#include <string.h> // memcpy

#ifdef _WIN32
    #include <windef.h> // HWND
    #include <winbase.h> // CreateFileMapping
    #include <minwindef.h> // DWORD
    #include <handleapi.h> // CloseHandle
    #include <fileapi.h> // CreateFile, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL
    #include <memoryapi.h> // VirtualAlloc, VirtualFree, MapViewOfFile, FILE_MAP_ALL_ACCESS, FILE_MAP_READ
    #include <winnt.h> // HANDLE, GENERIC_READ, GENERIC_WRITE, FILE_SHARE_READ, FILE_SHARE_WRITE, MEM_COMMIT, MEM_RESERVE, PAGE_READWRITE, PAGE_READONLY
#else
    #define _GNU_SOURCE
    #include <sys/mman.h> // mmap, munmap
#endif // _WIN32

/**
 * @brief Allocate a given size of memory on the heap.
 * @param size Size of the memory to allocate.
 * @param write Sentinel value of writability for the targeted file.
 * @returns A `virtually-allocateed` `page-aligned` region of memory of the given size.
 * @retval If the memory region can `not` be allocated, `NULL` is returned.
 */
NODISCARD void *heap_allocate(size_t size, bool write)
{
    if (size == 0) return NULL;
#ifdef _WIN32
    DWORD permision = write ? PAGE_READWRITE : PAGE_READONLY;
    return VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, permision);
#else
    int permission = write ? PROT_READ | PROT_WRITE : PROT_READ;
    void *pointer = mmap(NULL, size, permision, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (MAP_FAILED == pointer) return NULL;
    return pointer;
#endif // _WIN32
}

/**
 * @brief Reallocate a given region of heap memory.
 * @param memory Memory to reallocate.
 * @param old_size Original size of the region of memory.
 * @param new_size Desired size of the new region of memory.
 * @param write Sentinel value of writability for the targeted region of memory.
 * @returns A pointer to the new region of memory of the given `new size`.
 * @retval If the given `memory` is `NULL`, it is the same as a `new allocation`.
 * @retval If the given `old size` is `equal to` the `new size`, the `original` region of memory is returned.
 * @retval If the given `new size` is `zero`, the given region of memory is `deallocated`, and `NULL` is returned.
 * @retval If the `original` memory can not be reallocated, `NULL` is returned.
 */
NODISCARD bool heap_reallocate(void *memory, size_t old_size, size_t new_size, bool write)
{
#ifdef _WIN32
    if (0 == new_size)
    {
        if (NULL != memory) heap_free(memory, old_size);
        return false;
    }
    else if (NULL == memory) return false;
    else if (old_size == new_size) return false;
    
    void *replacement = heap_allocate(new_size, write);
    if (NULL == replacement) return false;

    size_t copy_size = old_size < new_size ? old_size : new_size;

    memcpy(replacement, memory, copy_size);

    if (!heap_free(memory, old_size))
    {
        heap_free(replacement, new_size);
        return false;
    }
#else
    void *replacement = mremap(memory->memory, memory->size, new_size, MREMAP_MAYMOVE);
    if (MAP_FAILED == replacement) return false;
#endif // _WIN32
    memory = replacement;
    return true;
}

/**
 * @brief Free a given region of `heap-backed` memory.
 * @param memory Region of memory to free.
 * @param size Size of the region of memory to free.
 * @returns True if the given memory is able to be freed, else false.
 */
bool heap_free(void *memory, size_t size)
{
    if (NULL == memory || NULL == memory) return false;
#ifdef _WIN32
    (void)size;
    return VirtualFree(memory, 0, MEM_RELEASE | MEM_DECOMMIT) != 0;
#else
    return mumap(memory, size) == 0;
#endif // _WIN32
}

#endif // HEAP_IMPLEMENTATION