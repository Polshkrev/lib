# Flag
This is a library concerned with implementing the [Flag Module from Go](https://pkg.go.dev/flag). A lot of the development is copy-pasted from a [Tsoding Video](https://www.youtube.com/watch?v=mpqaQIrVgew).
## Table Of Contents
1. [Usage](#usage)
    1. [Construction](#construction)
    2. [Access](#access)
        1. [Range](#range)
## Usage
### Construction
The only flag types that are supported at the moment are a boolean, string, and 64-bit unsigned integer. To construct a flag of any of the types, use either the `flag_string`, `flag_bool`, or `flag_uint64` functions. Each of the methods take a flag name &mdash; i.e. `o`, `help` &mdash;, a default value which can be null, and a description for the help message. These functions return a pointer to a either a string, boolean, or 64-bit unsigned integer in static memory.
### Access
Befor using a flag value, you will need to call the `flag_parse` function after you've constructed each flag. This function takes in `argc` and `argv` from the main function. Then you can use each of the set flag values. To set the name of your programme in the help message, you can use the `flag_set_programme_name`.
#### Range
You can set the minimum and maximum allowed values for a uint64 flag by calling the `flag_uint64_range`. This function will set the minimum and maximum value of the flag which will be printed in the help message. If the value passed to the flag at runtime is less than the minimum value or more then the maximum value, an error is raised and printed to `stderr`.