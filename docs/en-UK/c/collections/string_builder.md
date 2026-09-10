# String Builder
A classical dynamic array of charactors inspired by both [Tsoding's String Builder](https://github.com/tsoding/nob.h/blob/main/nob.h#L295) and [Golang's String Builder](https://pkg.go.dev/strings#Builder).
## Table Of Contents
1. [Usage](#usage)
    1. [Construction](#construction)
    2. [Append](#append)
    3. [Access](#access)
    4. [Copying](#copying)
    5. [Remove](#remove)
    6. [Destruction](#destruction)
## Usage
### Construction
To construct a new string builder object, you will need to call the `sring_builder_init` function. This will return a heap allocated string builder object.
### Append
To append to the string builder, there are two methods you can call: `string_builder_append` or `string_builder_extend`. `string_builder_append` takes in the object and a charactor to append. `string_builder_extend` takes in the object and a null-terminated c-string to append &mdash; charactor by charactor &mdash; to the object. Like any other classical dynamic array, if the capacity is reached, the array is reallocated. If the reallocation fails, and error is raised.
### Access
There are two ways to access data within the array: `string_builder_at` and `string_builder_data`. `string_builder_at` will take in an index and returns a pointer to a charactor with the array at said index. If either, the index is greater than the size of the array, or there is no data at the index and error is raised. `string_builder_data` returns the full array. This function appends a null byte to the array before returning. It internally calls `string_builder_append` function; so all errors pertaining to that function still apply.
### Copying
To copy the array to another string builder object, you will need to call the `string_builder_combine` function. This will take in a view to the original string builder and a destination string builder to which to append.
### Remove
To remove an element from the builder, the function `string_builder_remove` is defined. This function takes in the index where the removed data is located. If the given index is greater than the size of the builder, an `IndexError` is printed to `stderr` and the programme exits.
### Destruction
To deallocate the string builder object, you will need to call the `string_builder_delete` function.