# Buffer
Temporary allocator inspired by [Tsoding's temporary allocator](https://github.com/tsoding/nob.h/blob/main/nob.h#L604).

The buffer provides a simple way of performing temporary allocations without repeatedly allocating and freeing memory from the heap. All allocations are made from a fixed-size static buffer and are invalidated when the buffer is reset or rewound.

By default, the buffer has a capacity of 5 KiB. This can be changed by defining `TEMP_CAPACITY` before the implementation is included.

## Table of Contents
1. [Usage](#usage)
   1. [Allocation](#allocation)
   2. [Strings](#strings)
   3. [Checkpoints](#checkpoints)
   4. [Reset](#reset)

## Usage
The buffer is intended primarily for temporary allocations whose lifetime can be managed collectively rather than individually. Unlike normal heap allocation, there is no corresponding free operation for individual allocations.

When the buffer runs out of space, `buffer_allocate` returns `NULL`, while the string functions print an `AllocationError` to `stderr` and terminate the programme.

### Allocation
To allocate memory from the buffer, the `buffer_allocate` function is defined. It takes the number of bytes to allocate and returns a pointer to the newly allocated region.

The returned memory belongs to the buffer and should not be freed manually. Once the buffer is rewound or reset, previously allocated memory should be considered invalid.

### Strings
For working with strings, two convenience functions are provided.

The `buffer_duplicate` function copies a null-terminated string into the buffer and returns a pointer to the new copy. The resulting string is null terminated and can therefore be used anywhere a normal C string is expected.

The `buffer_sprintf` function provides a `printf`-style interface for constructing a formatted string directly inside the buffer. For example:

```c
char *message = buffer_sprintf("Hello, %s!", "World");
```

Both functions terminate the programme if the buffer does not have enough space for the requested string.

### Checkpoints
The buffer supports checkpoints through the `buffer_save` and `buffer_rewind` functions.

`buffer_save` returns the current position of the buffer's cursor. This value can later be passed to `buffer_rewind` to discard everything allocated after the checkpoint.

This makes it possible to treat a section of the buffer as a temporary allocation scope:

```c
size_t checkpoint = buffer_save();

char *message = buffer_sprintf("Hello, World!");

buffer_rewind(checkpoint);
```

After rewinding, the memory occupied by `message` is available for subsequent allocations.

### Reset
To completely clear the buffer, the `buffer_reset` function is defined. This moves the buffer's cursor back to the beginning, effectively making the entire buffer available for allocation again.

The contents of the buffer are not explicitly erased; they are simply considered unused.
