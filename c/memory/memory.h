#ifndef MEMORY_H_
#define MEMORY_H_

#define HEAP_IMPLEMENTATION
#include "heap.h"

#define FILE_IMPLEMENTATION
#include "file.h"

#include "../attributes.h" // NODISCARD

/**
 * @brief Finite list of possible backings of the allocated memory.
 */
typedef enum
{
    HEAP_BACKING,
    FILE_BACKING,
} backing_t;

/**
 * @brief Allocate a region of memory.
 * @param size `Desired` size of the allocated region of memory. If the given `backing` is `FILE_BACKING`, this parameter is `ignored`.
 * @param backing Backing of the region of memory.
 * @param file Handle to the desired file that will be mapped into memory. If the `backing` parameter is `HEAP_BACKING`, this parameter can be `NULL`.
 * @param write Sentinel value of writability for the targeted region of memory.
 * @returns A pointer to the newly allocated region of memory of the given size.
 * @retval If the given `size` is zero, `NULL` is returned.
 * @retval If the given `backing` is `HEAP_BACKING`, a pointer to a `virtually-allocated` region of memory is returned.
 * @retval If the given `backing` is `FILE_BACKING`, a pointer to a `mapping` of the given `file` is returned.
 * @note If the given `backing` is `FILE_BACKING`, the `caller` of this function is responsible for `closing` the given `file`.
 */
NODISCARD void *memory_allocate(size_t size, backing_t backing, descriptor_t file, bool write);

/**
 * @brief Free a given region of memory.
 * @param memory Region of memory to free.
 * @param backing Backing of the region of memory.
 * @returns True if the given memory is able to be freed, else false.
 */
bool memory_free(void *memory, size_t size, backing_t backing);

#endif // MEMORY_H_

#ifdef MEMORY_IMPLEMENTATION

/**
 * @brief Allocate a region of memory.
 * @param size `Desired` size of the allocated region of memory. If the given `backing` is `FILE_BACKING`, this parameter is `ignored`.
 * @param backing Backing of the region of memory.
 * @param file Handle to the desired file that will be mapped into memory. If the `backing` parameter is `HEAP_BACKING`, this parameter can be `NULL`.
 * @param write Sentinel value of writability for the targeted region of memory.
 * @returns A pointer to the newly allocated region of memory of the given size.
 * @retval If the given `size` is zero, `NULL` is returned.
 * @retval If the given `backing` is `HEAP_BACKING`, a pointer to a `virtually-allocated` region of memory is returned.
 * @retval If the given `backing` is `FILE_BACKING`, a pointer to a `mapping` of the given `file` is returned.
 * @note If the given `backing` is `FILE_BACKING`, the `caller` of this function is responsible for `closing` the given `file`.
 */
NODISCARD void *memory_allocate(size_t size, backing_t backing, descriptor_t file, bool write)
{
    switch (backing)
    {
        case HEAP_BACKING:
        {
            UNUSED(file);
            return heap_allocate(size, write);
        } break;
        case FILE_BACKING:
        {
            UNUSED(size);
            return file_allocate(file, write);
        } break;
        default:
        {
            return false;
        } break;
    }
}

/**
 * @brief Free a given region of memory.
 * @param memory Region of memory to free.
 * @param backing Backing of the region of memory.
 * @returns True if the given memory is able to be freed, else false.
 */
bool memory_free(void *memory, size_t size, backing_t backing)
{
    switch (backing)
    {
        case FILE_BACKING:
        {
            return file_free(memory, size);
        } break;
        case HEAP_BACKING:
        {
            return heap_free(memory, size);
        } break;
        default: return false;
    }
}

#endif // MEMORY_IMPLEMENTATION