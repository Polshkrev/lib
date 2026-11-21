#ifndef PROTECTION_H_
#define PROTECTION_H_

#include <stdbool.h> // bool
#include <stddef.h>  // size_t, NULL

/**
 * @brief Representation of a finite list of memory protections.
 */
typedef enum
{
    MEMORY_PROTECTION_READ = (1 << 0), /** @brief Read only memory protection. */
    MEMORY_PROTECTION_WRITE = (1 << 1), /** @brief Write only memory protection. */
    MEMORY_PROTECTION_EXECUTE = (1 << 2), /** @brief Execution only memory protection. */
    MEMORY_PROTECTION_FULL = MEMORY_PROTECTION_READ | MEMORY_PROTECTION_WRITE | MEMORY_PROTECTION_EXECUTE /** @brief Read, write, and execute memory protection. */
} memory_protection_t;

#ifdef _WIN32
#include <minwindef.h> // DWORD
/**
 * @brief Represent a given memory protection as its native counterpart.
 * @param protection Protection to represent.
 * @returns A given memory protection as its native counterpart.
 */
DWORD protection_to_native(memory_protection_t protection);
#else
/**
 * @brief Represent a given memory protection as its native counterpart.
 * @param protection Protection to represent.
 * @returns A given memory protection as its native counterpart.
 */
int protection_to_native(memory_protection_t protection);
#endif // _WIN32

/**
 * @brief Change — or assign — the protection of a given region of memory.
 * @param memory Region of memory to protect.
 * @param size Size of the given region of memory.
 * @param protection New memory protection to change — or assign — to the given region of memory.
 * @returns True if the given region of memory can be changed or assigned, else false.
 */
bool memory_protect(void *memory, size_t size, memory_protection_t protection);

#endif // PROTECTION_H_

#ifdef PROTECTION_IMPLEMENTATION

#ifdef _WIN32
    #include <winnt.h> // PAGE_NOACCESS, PAGE_READONLY, PAGE_READWRITE, PAGE_EXECUTE_READ, PAGE_EXECUTE_WRITE, PAGE_EXECUTE_READWRITE
    #include <memoryapi.h> // VirtualProtect
#else
    #include <sys/mman.h> // mprotect, PROT_NONE, PROT_READ, PROT_WRITE, PROT_EXEC
#endif // _WIN32

/**
 * @brief Change — or assign — the protection of a given region of memory.
 * @param memory Region of memory to protect.
 * @param size Size of the given region of memory.
 * @param protection New memory protection to change — or assign — to the given region of memory.
 * @returns True if the given region of memory can be changed or assigned, else false.
 */
bool memory_protect(void *memory, size_t size, memory_protection_t protection)
{
    if (memory == NULL || size == 0) return false;
#ifdef _WIN32
    DWORD old;
    DWORD native = protection_to_native(protection);
    return VirtualProtect(memory, size, native, &old) != 0;
#else
    int native = protection_to_native(protection);
    return mprotect(memory, size, native) == 0;
#endif // _WIN32
}

#ifdef _WIN32
/**
 * @brief Represent a given memory protection as its native counterpart.
 * @param protection Protection to represent.
 * @returns A given memory protection as its native counterpart.
 */
DWORD protection_to_native(memory_protection_t protection)
{
    bool write = protection & MEMORY_PROTECTION_WRITE;
    bool read = protection & MEMORY_PROTECTION_READ;
    bool execute = protection & MEMORY_PROTECTION_EXECUTE;
    if (execute)
    {
        return write ? PAGE_EXECUTE_READWRITE : read ? PAGE_EXECUTE_READ : PAGE_EXECUTE;
    }
    return write ? PAGE_READWRITE : read ? PAGE_READONLY : PAGE_NOACCESS;
}
#else
/**
 * @brief Represent a given memory protection as its native counterpart.
 * @param protection Protection to represent.
 * @returns A given memory protection as its native counterpart.
 */
int protection_to_native(memory_protection_t protection)
{
    bool write = protection & MEMORY_PROTECTION_WRITE;
    bool read = protection & MEMORY_PROTECTION_READ;
    bool execute = protection & MEMORY_PROTECTION_EXECUTE;
    return PROT_NONE | (read ? PROT_READ : 0) | (write ? PROT_WRITE : 0) | (execute ? PROT_EXEC : 0);
}
#endif // _WIN32

#endif // PROTECTION_IMPLEMENTATION