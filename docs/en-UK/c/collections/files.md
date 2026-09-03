# Files
Temu file walker.

In other words, a small dynamic array for storing file paths discovered within a directory on the file system.

The array owns the memory allocated for each stored path, while the `root` itself is non-owning. The implementation only currently supports `Windows` and recursively walks the directory tree starting from the given root.

By default, the array is initialized with a capacity of 256 entries. This can be changed by defining `FILES_CAPACITY` before including the implementation.

## Table of Contents
1. [Usage](#usage)
   1. [Construction](#construction)
   2. [Modification](#modification)
   3. [Access](#access)
   4. [Filling](#filling)
   5. [Memory](#memory)

## Usage
The intended use is to initialize an array with a root directory, fill it with the entries contained within that directory.

### Construction
To construct a new array of files, the `files_init` function is defined. It initializes the array with the default capacity specified by `FILES_CAPACITY`.

For more control over the initial allocation, `files_init_with_capacity` allows a specific capacity to be provided:

```c
files_t files = files_init_with_capacity("C:\\Projects", 512);
```

The `root` passed to either function is stored directly and is not copied. The caller is therefore responsible for ensuring that the string remains valid for the lifetime of the array.

### Modification
To append a path to the array, the `files_append` function is defined.
As in a standard dynamic array, if the array becomes full, the `files_resize` function doubles the capacity of the array, while `files_resize_by` allows the capacity to be increased by a given scaler:

```c
void files_append(files_t *files, const char *entry)
{
    if (files->size >= files->capacity)
    {
        files_resize(files); // files_resize_by(&files, 2);
    }
    /* ... */
}
```

To ensure the capacity match the current number of stored paths, the `files_fit` function is defined.

### Access
To access a path at a given index, the `files_at` function is defined. It returns a pointer to the stored path when the index is valid, and `NULL` when the index is outside the current size of the array.

For example:

```c
char **path = files_at(&files, 0);
```

### Filling
To populate the array with the contents of its root directory, the `files_fill` function is defined.

On Windows, the function recursively traverses the directory beginning at `root`, appending every discovered entry to the array. Directories are traversed recursively, meaning that paths contained within nested directories are also added.

The function returns `true` when the directory tree can be successfully traversed and `false` when an entry cannot be obtained.

### Memory
The array owns the memory allocated for its stored paths. To release this memory, the `files_delete` function is defined.

`files_delete` frees every path stored in the array before freeing the underlying array itself.

The `root` pointer is not freed because the array does not own the memory it points to.
