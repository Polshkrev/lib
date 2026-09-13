#ifndef FLAG_H_
#define FLAG_H_

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdio.h> // FILE
#include <stdbool.h> // bool
#include <stdint.h> // uintptr_t, uint64_t
#include <stddef.h> // size_t

/**
 * @brief Type representation of the "pseudo-union" type defined in the Flag struct.
 */
typedef enum
{
    FLAG_BOOL,
    FLAG_UINT64,
    FLAG_STR
} Flag_Type;

/**
 * @brief Enum to represent the index in a "pseudo-union" array.
 */
typedef enum
{
    DATA_VAL,
    DATA_DEF,
    DATA_MIN,
    DATA_MAX,
    __data_count,
} Flag_Data;

/**
 * @brief Struct to represent a runtime flag given to a programme.
 */
typedef struct
{
    Flag_Type type;
    const char *name;
    const char *desc;
    uintptr_t data[__data_count];
} Flag;

/**
 * @brief Internal way to create a new flag.
 * @param type An enum representation of a type of flag.
 * @param name Name of the flag to be printed in a help message.
 * @param desc Description of the flag to be printed in a help message.
 * @returns A pointer to the newly created flag.
 * @exception If the number of registered flags has exceeded the capacity, a `ValueError` is printed to `stderr` and the programme exits.
 */
Flag *flag_new(Flag_Type type, const char *name, const char *desc);

/**
 * @brief Construct a boolean flag.
 * @param name Name of the flag to be printed in a help message.
 * @param def Default boolean value of the flag.
 * @param desc Description of the flag to be printed in the help message.
 * @returns A pointer to the boolean value of the provided flag.
 */
bool *flag_bool(const char *name, bool def, const char *desc);

/**
 * @brief Construct an unsigned 64-bit integer flag.
 * @param name Name of the flag to be printed in a help message.
 * @param def Default unsigned 64-bit integer value of the flag.
 * @param desc Description of the flag to be printed in the help message.
 * @returns A pointer to the unsigned 64-bit integer value of the provided flag.
 */
uint64_t *flag_uint64(const char *name, uint64_t def, const char *desc);

/**
 * @brief Set the minimum and maximum values for an integer flag.
 * @param flag A mutable pointer to the unsigned 64-bit flag to modify.
 * @param min The minimum value accepted by the flag.
 * @param max The maximum value accepted by the flag.
 * @exception If the supplied minimum is greater than the given maximum, a `ValueError` is printed to `stderr` and the programme exits.
 * @exception If the supplied minimum is equal to the current minimum, a `ValueError` is printed to `stderr` and the programme exits.
 * @exception If the supplied maximum is equal to the current maximum, a `ValueError` is printed to `stderr` and the programme exits.
 */
void flag_uint64_range(uint64_t *flag, uint64_t min, uint64_t max);

/**
 * @brief Construct a string flag.
 * @param name Name of the flag to be printed in a help message.
 * @param def Default string value of the flag.
 * @param desc Description of the flag to be printed in the help message.
 * @returns A pointer to the c-string value of the provided flag.
 */
const char **flag_string(const char *name, const char *def, const char *desc);

/**
 * @brief Print a help message to a given stream.
 * @param stream A pointer to an output stream where the output message will be printed.
 * @param print_default A boolean flag to customise the printing of the default values of each flag. By default the value should be false.
 */
void flag_print_help(FILE *stream, bool print_default);

/**
 * @brief Set the name of the programme that takes in the flags. The name of the programme will be displayed in the help menu. A global `PROGRAMME_NAME` variable will be set to be used in the `flag_parse` function. If this function is not called, then the first argument supplied to `argv` will be used.
 * @param name String to set as the name of the programme marked with const.
 */
void flag_set_programme_name(const char *name);

/**
 * @brief Parse the flags provided to the programme at runtime. This function must be called for any of the flags to be parsed.
 * @param argc Argument count provided in `main`.
 * @param argv String array of runtime arguments provided in `main`
 */
void flag_parse(int argc, char **argv);

#if defined(__cplusplus)
}
#endif

#endif // FLAG_H_

#ifdef FLAG_IMPLEMENTATION

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdlib.h>  // NULL, exit, strtoull
#include <string.h>  // strcmp
#include <assert.h>  // static_assert
#include <errno.h>   // errno, ERANGE
#include <limits.h>  // ULLONG_MAX
#include <inttypes.h> // PRIu64

#ifndef FLAG_CAPACITY
#define FLAG_CAPACITY 256
#endif // FLAG_CAPACITY

static const char *_PROGRAMME_NAME = NULL;

static Flag flags[FLAG_CAPACITY];
static size_t flag_count = 0;

/**
 * @brief Internal way to create a new flag.
 * @param type An enum representation of a type of flag.
 * @param name Name of the flag to be printed in a help message.
 * @param desc Description of the flag to be printed in a help message.
 * @returns A pointer to the newly created flag.
 * @exception If the number of registered flags has exceeded the capacity, a `ValueError` is printed to `stderr` and the programme exits.
 */
Flag *flag_new(Flag_Type type, const char *name, const char *desc)
{
    // make sure the count doesn't exceed the capacity
    if (flag_count >= FLAG_CAPACITY)
    {
        fprintf(stderr, "ValueError: Amount of flags have exceeded capacity\n");
        exit(1);
    }
    // allocate a pointer on the stack to the address of the flag count after it's been incremented
    Flag *flag = &flags[flag_count++];
    // set values
    flag->type = type;
    flag->name = name;
    flag->desc = desc;
    return flag;
}

/**
 * @brief Construct a boolean flag.
 * @param name Name of the flag to be printed in a help message.
 * @param def Default boolean value of the flag.
 * @param desc Description of the flag to be printed in the help message.
 * @returns A pointer to the boolean value of the provided flag.
 */
bool *flag_bool(const char *name, bool def, const char *desc)
{
    Flag *flag = flag_new(FLAG_BOOL, name, desc);
    *((bool*) &flag->data[DATA_DEF]) = def; // reinterpret the address of the default value of the flag to the boolean address of the given default
    *((bool*) &flag->data[DATA_VAL]) = def; // doing the same as above but for the data as default
    return (bool*) &flag->data[DATA_VAL]; // return the address as a boolean
}

/**
 * @brief Construct an unsigned 64-bit integer flag.
 * @param name Name of the flag to be printed in a help message.
 * @param def Default unsigned 64-bit integer value of the flag.
 * @param desc Description of the flag to be printed in the help message.
 * @returns A pointer to the unsigned 64-bit integer value of the provided flag.
 */
uint64_t *flag_uint64(const char *name, uint64_t def, const char *desc)
{
    Flag *flag = flag_new(FLAG_UINT64, name, desc);
    *((uint64_t*) &flag->data[DATA_DEF]) = def;
    *((uint64_t*) &flag->data[DATA_VAL]) = def;
    *((uint64_t*) &flag->data[DATA_MIN]) = 0;
    *((uint64_t*) &flag->data[DATA_MAX]) = UINT64_MAX;
    return (uint64_t*) &flag->data[DATA_VAL];
}

/**
 * @brief Set the minimum and maximum values for an integer flag.
 * @param flag A mutable pointer to the unsigned 64-bit flag to modify.
 * @param min The minimum value accepted by the flag.
 * @param max The maximum value accepted by the flag.
 * @exception If the supplied minimum is greater than the given maximum, a `ValueError` is printed to `stderr` and the programme exits.
 * @exception If the supplied minimum is equal to the current minimum, a `ValueError` is printed to `stderr` and the programme exits.
 * @exception If the supplied maximum is equal to the current maximum, a `ValueError` is printed to `stderr` and the programme exits.
 */
void flag_uint64_range(uint64_t *flag, uint64_t min, uint64_t max)
{
    static_assert(sizeof(uint64_t) == sizeof(uintptr_t), "This will only work if the size of uint64_t and uintptr_t is the same");
    if (min > max)
    {
        fprintf(stderr, "ValueError: Minimum value of %"PRIu64" cannot be greater than maximum value of %"PRIu64".\n", min, max);
        exit(1);
    }
    else if (min == flag[DATA_MIN])
    {
        fprintf(stderr, "ValueError: Supplied minimum value of \"%"PRIu64"\" cannot be the same as the assigned default of %"PRIu64".", min, flag[DATA_MIN]);
        exit(1);
    }
    else if (max == flag[DATA_MAX])
    {
        fprintf(stderr, "ValueError: Supplied maximum value of \"%"PRIu64"\" cannot be the same as the assigned default of %"PRIu64".", max, flag[DATA_MAX]);
        exit(1);
    }
    flag[DATA_MIN] = min;
    flag[DATA_MAX] = max;
}

/**
 * @brief Construct a string flag.
 * @param name Name of the flag to be printed in a help message.
 * @param def Default string value of the flag.
 * @param desc Description of the flag to be printed in the help message.
 * @returns A pointer to the c-string value of the provided flag.
 */
const char **flag_string(const char *name, const char *def, const char *desc)
{
    Flag *flag = flag_new(FLAG_STR, name, desc);
    *((const char **) &flag->data[DATA_DEF]) = def;
    *((const char **) &flag->data[DATA_VAL]) = def;
    return (const char **) &flag->data[DATA_VAL];
}

/**
 * @brief Shift each argument provided as parsed at runtime.
 * @param argc A pointer to the arguments count provided in `main`. This parameter is decremented, thus its mutability.
 * @param argv A pointer to a dynamic array of strings provided in `main`.
 * @returns A string representation of the parsed argument.
 * @exception If there are no arguments provided, a `RuntimeError` is printed and the programme exits.
 */
static const char *_flag_shift_args(int *argc, char ***argv)
{
    if (*argc <= 0)
    {
        // fprintf(stderr, "RuntimeError: argc is less than or equal to zero.\n");
        fprintf(stderr, "RuntimeError: No arguments have been provided.\n");
        exit(1);
    }
    const char *result = **argv;
    *argv += 1; // Pointer arithmetic - since the array is stored as the pointer to the first element, we increment what the pointer is pointing to.
    *argc -= 1;
    return result;
}

/**
 * @brief Find a flag by its name.
 * @param name Name of the flag to find.
 * @returns A pointer to the matching flag, or NULL if no flag exists.
 */
static Flag *_flag_find(const char *name)
{
    for (size_t index = 0; index < flag_count; ++index)
    {
        if (!strcmp(flags[index].name, name))
        {
            return &flags[index];
        }
    }

    return NULL;
}

/**
 * @brief Parse the argument belonging to a string flag.
 * @param flag A pointer to the string flag to modify.
 * @param argc Argument count to consume from.
 * @param argv Argument array to consume from.
 */
static void _flag_scan_string(Flag *flag, int *argc, char ***argv)
{
    if (*argc == 0)
    {
        fprintf(stderr, "RuntimeError: No argument provided for \"-%s\".\n", flag->name);
        exit(1);
    }

    flag->data[DATA_VAL] = (uintptr_t)_flag_shift_args(argc, argv);
}

/**
 * @brief Parse the argument belonging to an unsigned 64-bit flag.
 * @param flag A pointer to the unsigned 64-bit flag to modify.
 * @param argc Argument count provided in `main`.
 * @param argv String array of runtime arguments provided in `main`.
 */
static void _flag_scan_uint64(Flag *flag, int *argc, char ***argv)
{
    if (*argc == 0)
    {
        fprintf(stderr, "RuntimeError: No argument provided for \"-%s\".\n", flag->name);
        exit(1);
    }

    const char *argument = _flag_shift_args(argc, argv);

    errno = 0;

    char *endptr = NULL;

    unsigned long long int result = strtoull(argument, &endptr, 10);

    if (argument == endptr || *endptr != '\0')
    {
        fprintf(stderr, "RuntimeError: '%s' is not a valid number.\n", flag->name);
        exit(1);
    }
    else if (result == ULLONG_MAX && errno == ERANGE)
    {
        fprintf(stderr, "OverflowError: '%s' is a 64 bit unsigned integer overflow.\n", flag->name);
        exit(1);
    }

    uint64_t value = result;
    uint64_t minimum = *(uint64_t *) &flag->data[DATA_MIN];
    uint64_t maximum = *(uint64_t *) &flag->data[DATA_MAX];

    if (!(minimum <= value && value <= maximum))
    {
        fprintf(stderr, "ValueError: The value provided for the argument " "\"-%s\" is outside of the [%"PRIu64"..%"PRIu64"] range.\n", flag->name, minimum, maximum);
        exit(1);
    }

    *(uint64_t *) &flag->data[DATA_VAL] = value;
}

/**
 * @brief Parse a flag according to its type.
 * @param flag A pointer to the flag to parse.
 * @param argc Argument count provided in `main`.
 * @param argv String array of runtime arguments provided in `main`.
 */
static void _flag_scan_value(Flag *flag, int *argc, char ***argv)
{
    switch (flag->type)
    {
        case FLAG_BOOL:
        {
            *(bool *)&flag->data[DATA_VAL] = true;
        } break;
        case FLAG_STR:
        {
            _flag_scan_string(flag, argc, argv);
        } break;
        case FLAG_UINT64:
        {
            _flag_scan_uint64(flag, argc, argv);
        } break;
        default:
        {
            fprintf(stderr, "UnreachableError: Execution has reached an unreachable section.\n");
            exit(1);
        } break;
    }
}

/**
 * @brief Internal implementation of the flag parsing.
 * @param argc Argument count provided in `main`.
 * @param argv String array of runtime arguments provided in `main`.
 */
static void _flag_scan(int argc, char **argv)
{
    _flag_shift_args(&argc, &argv);

    while (argc > 0)
    {
        const char *argument = _flag_shift_args(&argc, &argv);

        if (!strcmp(argument, "-"))
        {
            fprintf(stderr, "ValueError: Unknown Flag \"%s\".\n", argument);
            exit(1);
        }

        argument += 1;

        Flag *flag = _flag_find(argument);

        if (flag == NULL)
        {
            fprintf(stderr, "ValueError: Unknown Flag \"%s\".\n", argument);
            exit(1);
        }

        _flag_scan_value(flag, &argc, &argv);
    }
}

/**
 * @brief Print the range of an unsigned 64-bit integer flag.
 * @param stream A pointer to an output stream where the range will be printed.
 * @param flag A pointer to the flag whose range will be printed.
 */
static void _flag_print_range(FILE *stream, const Flag *flag)
{
    if (flag->type != FLAG_UINT64) return;
    uint64_t minimum = *(uint64_t *)&flag->data[DATA_MIN];
    uint64_t maximum = *(uint64_t *)&flag->data[DATA_MAX];
    fprintf(stream, "\n\t\trange: ");
    if (minimum == 0 && maximum == UINT64_MAX)
    {
        fprintf(stream, "No range has been set.");
        return;
    }
    fprintf(stream, "[%"PRIu64"..%"PRIu64"]", minimum, maximum);
}

/**
 * @brief Print the default value of a flag.
 * @param stream A pointer to an output stream where the default value will be printed.
 * @param flag A pointer to the flag whose default value will be printed.
 */
static void _flag_print_default(FILE *stream, const Flag *flag)
{
    switch (flag->type)
    {
        case FLAG_BOOL:
        {
            if (!strcmp(flag->name, "help")) return;
            fprintf(stream, " (default: %s)", *(bool *)&flag->data[DATA_DEF] ? "true" : "false");
        } break;
        case FLAG_UINT64:
        {
            fprintf(stream, " (default: %"PRIu64")", *(uint64_t *)&flag->data[DATA_DEF]);
        } break;
        case FLAG_STR:
        {
            const char **value = (const char **)&flag->data[DATA_DEF];
            fprintf(stream, " (default: \"%s\")", *value);
        } break;
        default:
        {
            fprintf(stderr, "UnreachableError: Execution has reached an unreachable section.\n");
            exit(1);
        }
    }
}

/**
 * @brief Print a help message to a given stream.
 * @param stream A pointer to an output stream where the output message will be printed.
 * @param print_default A boolean flag to customise the printing of the default values of each flag. By default the value should be false.
 */
void flag_print_help(FILE *stream, bool print_default)
{
    for (size_t index = 0; index < flag_count; ++index)
    {
        fprintf(stream, "\n");
        const Flag *flag = &flags[index];

        fprintf(stream, "\t-%s\n\t\t%s", flag->name, flag->desc);

        if (print_default)
        {
            _flag_print_default(stream, flag);
        }

        _flag_print_range(stream, flag);
    }
}

/**
 * @brief Set the name of the programme that takes in the flags. The name of the programme will be displayed in the help menu. A global `PROGRAMME_NAME` variable will be set to be used in the `flag_parse` function. If this function is not called, then the first argument supplied to `argv` will be used.
 * @param name String to set as the name of the programme marked with const.
 */
void flag_set_programme_name(const char *name)
{
    _PROGRAMME_NAME = name;
}

/**
 * @brief Parse the flags provided to the programme at runtime. This function must be called for any of the flags to be parsed.
 * @param argc Argument count provided in `main`.
 * @param argv String array of runtime arguments provided in `main`
 */
void flag_parse(int argc, char **argv)
{
    char *programme = argv[0];
    bool *help = flag_bool("help", false, "Print this help to stdout and exit with 0.");
    _flag_scan(argc, argv);
    if (*help)
    {
        // assert(argc >= 1);
        if (argc < 1)
        {
            fprintf(stderr, "ValueError: There is not enough arguments to parse.\n");
            exit(1);
        }
        fprintf(stdout, "Usage: %s [OPTIONS]\nOPTIONS:", _PROGRAMME_NAME ? _PROGRAMME_NAME : programme);
        flag_print_help(stdout, true);
        exit(0);
    };
}

#if defined(__cplusplus)
}
#endif

#endif // FLAG_IMPLEMENTATION