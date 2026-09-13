# Directory
A small, dynamically sized collection for representing the entries contained within a directory on the file system.

## Table Of Contents
1. [Usage](#usage)
    1. [Construction](#construction)
    2. [Append](#append)
        1. [Ownership](#ownership)
    3. [Access](#access)
    4. [Remove](#remove)
    5. [Delete](#delete)

### Construction
To construct a new directory there are two functions defined: `directory_init` and `directory_init_with_capacity`. These functions are mainly self-explanatory. The difference between the two is that `directory_init` constructs a new directory with a pre-defined capacity.
If any of these construction functions fail, the library panics with an `AllocationError`.

### Append
To append an `entry_t` to a directory, the self-explanatory function `directory_append` is defined.
This function can fail with an `AllocationError`.

#### Ownership
A note on the ownership of the internal directory array, all entries appended to the directory should be owned by the directory.
Any entry appended to the directory &mdash; espsecially a removed entry &mdash; will have all of its fields destroyed when the `directory_delete` function is called.

### Access
To access an entry within a directory, the function `directory_at` is defined. This function takes in an index and returns either the `entry_t` stored in the directory at that index, or if the given index is outside of the bounds of the directory, the programme panics with an `IndexError`.

### Remove
To remove an entry within a directory, the `directory_remove` function is defined. This function takes in an index and either removes the `entry_t` stored in the directory at that index, or if the given index is outside of the bounds of the directory, the programme panics with an `IndexError`.
This function does not change the capacity.
The removed `entry_t` previously stored at the given index will have all of its fields destroyed when it is removed from the directory.

### Delete
Due to this being a classical-dynamic array, the directory will need to be deallocated. To deallocate the directory, the function `directory_delete` function is defined. This function will deallocate, both the directory's internal array, and any of the stored or allocated properties of each of the stored entries.