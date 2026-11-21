#ifndef HEAP_H_
#define HEAP_H_

#include <stdbool.h> // bool
#include <stddef.h> // size_t, NULL

#include "protection.h" // memory_protection_t, memory_protect, protection_to_native

/**
 * @brief Allocate a given size of memory on the heap.
 * @param size Size of the memory to allocate.
 * @param write Sentinel value of writability for the targeted file.
 * @returns A `virtually-allocateed` `page-aligned` region of memory of the given size.
 * @retval If the memory region can `not` be allocated, `NULL` is returned.
 */
void *heap_allocate(size_t size, memory_protection_t protection);

/**
 * @brief Reallocate a given region of heap memory.
 * @param memory Memory to reallocate.
 * @param old_size Original size of the region of memory.
 * @param new_size Desired size of the new region of memory.
 * @param protection Protection mask of the desired access for the given region of memory.
 * @returns A pointer to the `new` region of memory of the given `new` size.
 * @retval If the given `memory` is `NULL`, it is the `same` as a `new` allocation.
 * @retval If the given `old` size is `equal` to the `new` size, the `original` region of memory is returned.
 * @retval If the given `new` size is `zero`, `NULL` is returned.
 * @retval If the `original` memory can not be reallocated, `NULL` is returned.
 * @note If `NULL` is returned, the `caller` is responsible for the `original` region of memory.
 */
void *heap_reallocate(void *memory, size_t old_size, size_t new_size, memory_protection_t protection);

#ifdef _WIN32
/**
 * @brief Free a given region of `heap-backed` memory.
 * @param memory Region of memory to free.
 * @param size Size of the region of memory to free.
 * @returns True if the given memory is able to be freed, else false.
 * @note Size `can` be `zero` as — on windows — it is `unused`.
 */
bool heap_free(void *memory, size_t size);
#else
/**
 * @brief Free a given region of `heap-backed` memory.
 * @param memory Region of memory to free.
 * @param size Size of the region of memory to free.
 * @returns True if the given memory is able to be freed, else false.
 */
bool heap_free(void *memory, size_t size);
#endif // _WIN32

#endif // HEAP_H_

#ifdef HEAP_IMPLEMENTATION

#include <string.h> // memcpy

#ifdef _WIN32
    #include <windef.h> // HWND
    #include <minwindef.h> // DWORD
    #include <memoryapi.h> // VirtualAlloc, VirtualFree,
    #include <winnt.h> //  MEM_COMMIT, MEM_RESERVE, MEM_RELEASE
#else
    #include <sys/mman.h> // mmap, mremap, munmap, MAP_PRIVATE, MAP_ANONYMOUS
#endif // _WIN32

#include "../attributes.h" // NODISCARD, UNUSED

/**
 * @brief Allocate a given size of memory on the heap.
 * @param size Size of the memory to allocate.
 * @param protection Protection mask of the desired access.
 * @returns A `virtually-allocateed` `page-aligned` region of memory of the given size.
 * @retval If the memory region can `not` be allocated, `NULL` is returned.
 */
NODISCARD void *heap_allocate(size_t size, memory_protection_t protection)
{
    if (size == 0) return NULL;
#ifdef _WIN32
    DWORD permision = protection_to_native(protection);
    return VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, permision);
#else
    int permission = protection_to_native(protection);
    void *memory = mmap(NULL, size, permision, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (MAP_FAILED == memory) return NULL;
    return memory;
#endif // _WIN32
}

/**
 * @brief Reallocate a given region of heap memory.
 * @param memory Memory to reallocate.
 * @param old_size Original size of the region of memory.
 * @param new_size Desired size of the new region of memory.
 * @param protection Protection mask of the desired access for the given region of memory.
 * @returns A pointer to the `new` region of memory of the given `new` size.
 * @retval If the given `memory` is `NULL`, it is the `same` as a `new` allocation.
 * @retval If the given `old` size is `equal` to the `new` size, the `original` region of memory is returned.
 * @retval If the given `new` size is `zero`, `NULL` is returned.
 * @retval If the `original` memory can not be reallocated, `NULL` is returned.
 * @note If `NULL` is returned, the `caller` is responsible for the `original` region of memory.
 */
NODISCARD void *heap_reallocate(void *memory, size_t old_size, size_t new_size, memory_protection_t protection)
{
#ifdef _WIN32
    if (0 == new_size) return NULL;
    else if (NULL == memory) return heap_allocate(new_size, protection);
    else if (old_size == new_size) return memory;
    void *replacement = heap_allocate(new_size, MEMORY_PROTECTION_FULL);
    if (NULL == replacement) return NULL;
    size_t copy_size = old_size < new_size ? old_size : new_size;
    memcpy(replacement, memory, copy_size);
    if (!memory_protect(replacement, new_size, protection))
    {
        heap_free(replacement, new_size);
        return NULL;
    }
    else if (!heap_free(memory, old_size))
    {
        heap_free(replacement, new_size);
        return NULL;
    }
#else
    void *replacement = mremap(memory->memory, memory->size, new_size, MREMAP_MAYMOVE);
    if (MAP_FAILED == replacement) return NULL;
#endif // _WIN32
    return replacement;
}

#ifdef _WIN32
/**
 * @brief Free a given region of `heap-backed` memory.
 * @param memory Region of memory to free.
 * @param size Size of the region of memory to free.
 * @returns True if the given memory is able to be freed, else false.
 * @note Size `can` be `zero` as — on windows — it is `unused`.
 */
bool heap_free(void *memory, size_t size)
#else
/**
 * @brief Free a given region of `heap-backed` memory.
 * @param memory Region of memory to free.
 * @param size Size of the region of memory to free.
 * @returns True if the given memory is able to be freed, else false.
 */
bool heap_free(void *memory, size_t size)
#endif // _WIN32
{
    if (NULL == memory) return false;
#ifdef _WIN32
    UNUSED(size);
    return VirtualFree(memory, 0, MEM_RELEASE) != 0;
#else
    if (size == 0) return false;
    return munmap(memory, size) == 0;
#endif // _WIN32
}

#endif // HEAP_IMPLEMENTATION