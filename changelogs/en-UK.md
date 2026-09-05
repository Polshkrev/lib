# Changelog
## v0.31.0 - 2026-09-05
`Added`
- CPP
    - `logger`
        - Added `name` and `level` getters.
        - Added `name` and `level` setters.

`Changed`
- C
    - `logger`
        - The `logger_init` constructor now returns `NULL` in the case of a failed allocation, rather than exiting with an error.
        - The `lltostr` function now returns `NONE` in the case of an invalid `level_t` parameter, rather than exiting with an error.
        - All setup methods now return a boolean, rather than exiting with an error.
        - `logger_delete` now accepts a double pointer due to ownership issues.
        - In the case that a `timestamp` can not be set, `__publish_message` now no longer publishes a message with a timestamp. All other behaviour is the same.
- CPP
    - `logger`
        - The `lltostr` function now returns `NONE` in the case of an invalid `level_t` parameter, rather than exiting with an error.
        - All setup methods now return a boolean, rather than exiting with an error.
        - `close` is now a public method rather than a friend function.
        - In the case that a `timestamp` can not be set, `__publish_message` now no longer publishes a message with a timestamp. All other behaviour is the same.
        - `output_count` is now a private member variable, instead of being global.

`Removed`
- CPP
    - `logger`
        - The default destructor has been removed. The `close` method will now have to be called explicitly.
## v0.30.0 - 2026-09-04
`Added`
- C
    - `loader`
        - Added the generic `library_t` type.
        - Added the generic `function_t` type.
        - Added the `library_error` function.
- CPP
    - `loader`
        - Added the generic `library_t` type.
        - Added the generic `function_t` type.
        - Added the `library_error` function.

`Changed`
- C
    - `loader`
        - To simplify the responsibility of the library, each of the functions no longer exit. If a failure state occurs, each of the functions return either `false` or `NULL`.
        - `library_load` now returns the more generic `library_t` instead of just a `void` pointer.
        - `library_function` now returns the more generic `function_t` instead of just a `void` pointer.
        - `library_delete` has been renamed to `library_close`.
- CPP
    - `loader`
        - To simplify the responsibility of the library, each of the functions no longer exit. If a failure state occurs, each of the functions return either `false` or a `nullptr`.
        - `library_load` now returns the more generic `library_t` instead of just a `void` pointer.
        - `library_function` now returns the more generic `function_t` instead of just a `void` pointer.
        - `library_delete` has been renamed to `library_close`.
## v0.29.0 - 2026-09-04
`Added`
- C
    - `collections`
        - `string`
            - Added basic `UTF-8` support.
            - The `string_is_valid_utf8()` function has been added.
            - The `string_utf8_length()` funtion has been added.
            - The `string_is_null()` function has been added.
            - The `STRING_NOT_FOUND` sentinel value has been added.

`Changed`
- C
    - `collections`
        - `string`
            - To more closely align with the conventions of the library, the `string_from_literal()` function has been renamed to simply `string_from()`.
            - Due to a basic typographic error, the `string_chop_by_delimetre()` function has been renamed to `string_chop_by_delimiter()`.
            - To more closely align with `unicode` support, all `search` and `comparison` functions are now explicitly `byte-oriented`.
            - `string_chop_by_delimiter()` now leaves the original string empty when the delimiter is not found.
            - `char_t` is now defined as an `unsigned char`.

`Removed`
- C
    - `collections`
        - `string`
            - Removed the `string_length` function.
## v0.28.0 - 2026-09-03
`Added`
- C
    - All include traces have been added.
- CPP
    - All include traces have been added.
    - `path`
        - The `filename` and `extension` methods have been added.

`Changed`
- C
    - Removed `windows.h` from all files in favour of the more specific includes needed within the library.
    - `logger`
        - Within the `log` function, in the unlikely case of an unknown logging level being passed into the function, the message will not be logged.
    - `collections`
        - `string_builder`
            - In the function `string_builder_fit`, if the capacity of the string builder is `less than or equal` to the current size, the function does nothing. This is to avoid an additional allocation.
- CPP
    - Removed `windows.h` from all files in favour of the more specific includes needed within the library.
    - `path`
        - The `absolute` method no longer throws an exception if the path does not exist.

`Fixed`
- C
    - `logger`
        - The previously private static variable `output_count`, has been added to the logger structure. All previous behaviour remains the same.
- CPP
    - `logger`
        - The previously private static variable `output_count`, has been added to the logger structure. All previous behaviour remains the same.
    - `path`
        - The `parent` method no longer uses the absolute path.
## v0.27.0 - 2026-07-24
`Added`
- C
    - `collections`
        - `string`
            - Added `unicode` support.
            - Added the `string_length` function.
## v0.26.0 - 2026-07-20
`Added`
- C
    - `collections`
        - Added `string_empty` function.

`Changed`
- C
    - `path`
        - The function `path_get_parent` no longer needs the absolute path.
    - `collections`
        - The `string_builder` structure is no longer heap allocated. Its underlying array is still heap allocated.
        - The content of `entry_t` is no longer a pointer. The content still needs to be deallocated.
## v0.25.0 - 2026-06-12
`Removed`
- C
    - Removed `directory_read`. The `files_t` interface will be used in its place.
## v0.24.0 - 2026-06-12
`Changed`
- C
    - All structures have been stack allocated where applicable.

`Removed`
- C
    - Removed `entry_delete`. To deallocate the entry content, use `string_builder_delete`.
    - Removed `directory_to_string`.
## v0.23.0 - 2026-04-04
`Changed`
- C
    - Renamed `logger_new` to `logger_init` to better fit within conventions.
    - `pasb` no longer checks if the given path exists.

`Fixed`
- C
    - `logger_new` now allocates the correct amount of memory for the logger.
## v0.22.0 - 2026-03-30
`Changed`
- Implementations of some methods and functions have changed. Hopefully, all previous behaviours should still be the same.
## v0.21.0 - 2026-03-20
`Changed`
- Removed buffer implementations.
## v0.20.0 - 2026-03-16
`Added`
- C
    - Added `path_filename` function.
    - Added `path_extension` function.
## v0.19.0 - 2026-03-16
`Added`
- CPP
    - Added `entry` module.
## v0.18.0 - 2026-03-16
`Added`
- CPP
    - Added `loader` module.
    - Added `FileExistsError` exception.
## v0.17.0 - 2026-03-16
`Added`
- CPP
    - Added `printable_t` interface.
    - Added both the `parent` and `root` methods for `path_t`.
    - Added all getters and setters for private properties for `version_t`.

`Changed`
- CPP
    - All public properties are now private in `version_t`.
## v0.16.0 - 2026-03-14
`Added`
- C
    - `collections`
        - Added `string_builder_find`.
        - Added `string_builder_items`.
    - Added `path_get_root`.
## v0.15.0 - 2026-03-13
`Added`
- C
    - `collections`
        - Added `directory_to_string`.
        - Added `entry_to_string`.
- CPP
    - `collections`
        - Added the `is_full` methos for `sized_t`.
        - Added the `is_full` method for `array_t`.

`Changed`
- CPP
    - `collections`
        - `length` has been renamed to `size` for `sized_t`.
## v0.14.0 - 2026-03-11
`Added`
- C
    - `collections`
        - Added `directory` module.
        - Added `NONE_TYPE` for `file_type_t` enum.
        - Added `file_type_to_string` function.
## v0.13.0 - 2026-02-25
`Added`
- C
    - `collections`
        - Added `files` module.
        - `entry`
            - Added `entry_size` function.
        - `string_builder`
            - Added `string_builder_fit` function.

`Changed`
- C
    - The `entry` module has been moved to `collections`.
    - The `buffer` module has been moved to `collections`
    - `collections`
        - Each creation function for `entry_t` have been marked as non-const.
        - Each removal function for `entry_t` have been marked as non-const.
        - Each function for `entry_t` has been properly error handled, i.e. crash states have been added where applicable.
## v0.12.0 - 2026-02-24
`Added`
- C
    - Added `entry_t` type.
    - Added `file_type_t` enum.
    - `Docs`
        - Added documentation for `entry_t`.

`Changed`
- C
    - `loader_load` no longer depends on a `path_t` to ne passed as an argument. Instead a regular c-string will suffice.The previous behaviour is the same.
## v0.11.0 - 2026-01-30
`Added`
- C
    - Added `library_load` function.
    - Added `library_function` function.
    - Added `library_delete` function.
## v0.10.0 - 2026-01-08
`Added`
- C
    - Added `path` module.
- CPP
    - Added `path` module.
    - `Exceptions`
        - `IOError` has been added.
## v0.9.0 - 2026-01-08
`Added`
- C
    - Added `buffer` module.
    - `Docs`
        - Added `buffer` documentation.
## v0.8.0 - 2026-01-06
`Added`
- CPP
    - Added the `dynamic array` module.
    - `Array`
        - Added the `length` method.
        - Added the `is_empty` method.
    - `Docs`
        - Added the `dynamic array` documentation.

`Changed`
- CPP
    - `Array`
        - All the private member variables are now protected due to implementation changes.
## v0.7.0 - 2026-01-06
`Added`
- CPP
    - Added the `sized` interface.
    - `Docs`
        - Added the `sized` documentation.
## v0.6.0 - 2026-01-03
`Added`
- C
    - Added the `string builder` module.
    - `Docs`
        - Added `string builder` documentation.
## v0.5.0 - 2026-01-02
`Added`
- CPP
    - Added the `array` module.
    - Added the `collection` module.
    - An `AllocationError` has been added.
    - An `IndexError` has been added.
    - An `OutOfRangeError` has been added.
    - `Docs`
        - Added the `array` module documentation.
        - Added the `collection` module documentation.
## v0.4.0 - 2025-12-23
`Added`
- C
    - Added the `string view` module.
    - `Docs`
        - Added the `string view` module documentation.

## v0.3.0 - 2025-12-14
`Added`
- C
    - Added the `flag` module.
    - `Docs`
        - Added `flag` module documentation.

`Changed`
- CPP
    - `ValueError`, `FileNotFoundError`, and `UnreachableError` have been moved to a seperate file: `exceptions.hpp`.
    - `Version`
        - `to_string` now returns a `std::string`.

## v0.2.0 - 2025-12-09
`Added`
- C
    - Added the `logger` module.
    - `Docs`
        - Added the `logger` module documentation.
- CPP
    - Added the `logger` module.
    - `Exception`
        - `UnreachableError` has been added.
        - `FileNotFoundError` has been added.
    - `Docs`
        - Added the `logger` module documentation.

`Changed`
- C
    - `version_init` has been renamed to `version_new` to more closely fit with this package's nomenclature.

## v0.1.0 - 2025-12-06
`Added`
- C
    - Added the `version` module.
    - `Docs`
        - Added documentation for the `version` module.
- CPP
    - Added the `exception` module.
    - Added the `version` module.
    - `Docs`
        - Added documentation for the `version` module.
        - Added documentation for the `exception` module.