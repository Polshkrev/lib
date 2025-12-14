#ifndef FLAG_H_
#define FLAG_H_

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdio.h> // FILE, fprintf, snprintf, stdout, stderr
#include <stdbool.h> // bool
#include <inttypes.h> // uintptr_t, uint64_t, size_t, PRIu64

/*
* @brief Type representation of the "psuedo-union" type defined in the Flag struct.
*/
typedef enum
{
    FLAG_BOOL,
    FLAG_UINT64,
    FLAG_STR
} Flag_Type;

/*
* @brief Enum to represent the index in a "psuedo-union" array.
*/
typedef enum
{
    DATA_VAL,
    DATA_DEF,
    DATA_MIN,
    DATA_MAX
} Flag_Data;

/*
* @brief Struct to represent a runtime flag given to a programme.
*/
typedef struct
{
    Flag_Type type;
    char *name;
    char *desc;
    uintptr_t data[sizeof(Flag_Data)];
} Flag;

/*
* @brief Internal way to create a new flag.
* @param type An enum representation of a type of flag.
* @param name Name of the flag to be printed in a help message.
* @param desc Description of the flag to be printed in the help message.
* @returns A pointer to the address of the allocated flag.
* @exception If the amount of allocated flags have exceeded the capacity, a `ValueError` is printed to `stderr` and the programme exits.
*/
Flag *flag_new(Flag_Type type, char *name, char *desc);

/*
* @brief Allocate a boolean flag.
* @param name Name of the flag to be printed in a help message.
* @param def Default boolean value of the flag.
* @param desc Description of the flag to be printed in the help message.
* @returns A pointer to the boolean value of the provided flag.
*/
bool *flag_bool(char *name, bool def, char *desc);

/*
* @brief Allocate an unsigned 64-bit integer flag.
* @param name Name of the flag to be printed in a help message.
* @param def Default unsigned 64-bit integer value of the flag.
* @param desc Description of the flag to be printed in the help message.
* @returns A pointer to the unsigned 64-bit integer value of the provided flag.
*/
uint64_t *flag_uint64(char *name, uint64_t def, char *desc);

/*
* @brief Set a range between a given minimum and maximum for a given integer flag.
* @param flag A mutable pointer to the unsigned 64-bit flag to set the range.
* @param min An unsinged 64-bit integer minimum to set.
* @param max An unsinged 64-bit integer maximum to set.
* @exception If the supplied value is greater than or equal to the maximum allowed by the type, a `ValueError` is printed and the programme exits.
* @exception If the supplied value is less than or equal to the minimum allowed by the type, a `ValueError` is printed and the programme exits.
*/
void flag_uint64_range(uint64_t *flag, uint64_t min, uint64_t max);

/*
* @brief Allocate a string flag.
* @param name Name of the flag to be printed in a help message.
* @param def Default string value of the flag.
* @param desc Description of the flag to be printed in the help message.
* @returns A pointer to the c-string value of the provided flag.
*/
char **flag_string(char *name, char *def, char *desc);

/*
* @brief Print a help message to a given stream.
* @param stream A pointer to an ouput stream where the ouptut message will be printed.
* @param print_default A boolean flag to customize the printing of the default values of each flag. By default the value should be false.
*/
void flag_print_help(FILE *stream, bool print_default);

/*
* @brief Set the name of the programme that takes in the flags. The name of the programme will be displayed in the help menu. A global `PROGRAMME_NAME` variable will be set to be used in the `flag_parse` function. If this function is not called, then the first argument supplied to `argv` will be used.
* @param name String to set as the name of the programme marked with const.
*/
void flag_set_programme_name(const char *name);

/*
* @brief Parse the flags provided to the programme at runtime. This function must be called for any of the flags to be parsed.
* @param argc Argument count provided in `main`.
* @param argv String array of runtime arguments provided in `main`.
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

#include <stdlib.h> // NULL, exit
#include <string.h> // strcmp, strtoull
#include <assert.h> // static_assert
#include <errno.h> // errorno, ERANGE
#include <limits.h> // ULLONG_MAX, UINT64_MAX

#define FLAG_CAPACITY 256

static const char *_PROGRAMME_NAME;

static Flag flags[FLAG_CAPACITY];
static size_t flag_count = 0;

#define FLAG_TMP_STR_CAPACITY 1024
static char flag_tmp_str[FLAG_TMP_STR_CAPACITY];
static size_t flag_tmp_str_size = 0;

/*
* @brief Internal way to create a new flag.
* @param type An enum representation of a type of flag.
* @param name Name of the flag to be printed in a help message.
* @param desc Description of the flag to be printed in the help message.
* @returns A pointer to the address of the allocated flag.
* @exception If the amount of allocated flags have exceeded the capacity, a `ValueError` is printed to `stderr` and the programme exits.
*/
Flag *flag_new(Flag_Type type, char *name, char *desc)
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

/*
* @brief Allocate a boolean flag.
* @param name Name of the flag to be printed in a help message.
* @param def Default boolean value of the flag.
* @param desc Description of the flag to be printed in the help message.
* @returns A pointer to the boolean value of the provided flag.
*/
bool *flag_bool(char *name, bool def, char *desc)
{
    Flag *flag = flag_new(FLAG_BOOL, name, desc);
    *((bool*) &flag->data[DATA_DEF]) = def; // reinterpret the adress of the default value of the flag to the boolean address of the given default
    *((bool*) &flag->data[DATA_VAL]) = def; // doing the same as above but for the data as default
    return (bool*) &flag->data[DATA_VAL]; // return the address as a boolean
}

/*
* @brief Allocate an unsigned 64-bit integer flag.
* @param name Name of the flag to be printed in a help message.
* @param def Default unsigned 64-bit integer value of the flag.
* @param desc Description of the flag to be printed in the help message.
* @returns A pointer to the unsigned 64-bit integer value of the provided flag.
*/
uint64_t *flag_uint64(char *name, uint64_t def, char *desc)
{
    Flag *flag = flag_new(FLAG_UINT64, name, desc);
    *((uint64_t*) &flag->data[DATA_DEF]) = def;
    *((uint64_t*) &flag->data[DATA_VAL]) = def;
    *((uint64_t*) &flag->data[DATA_MIN]) = 0;
    *((uint64_t*) &flag->data[DATA_MAX]) = UINT64_MAX;
    return (uint64_t*) &flag->data[DATA_VAL];
}

/*
* @brief Set a range between a given minimum and maximum for a given integer flag.
* @param flag A mutable pointer to the unsigned 64-bit flag to set the range.
* @param min An unsinged 64-bit integer minimum to set.
* @param max An unsinged 64-bit integer maximum to set.
* @exception If the supplied value is greater than or equal to the maximum allowed by the type, a `ValueError` is printed and the programme exits.
* @exception If the supplied value is less than or equal to the minimum allowed by the type, a `ValueError` is printed and the programme exits.
*/
void flag_uint64_range(uint64_t *flag, uint64_t min, uint64_t max)
{
    static_assert(sizeof(uint64_t) == sizeof(uintptr_t), "This will only work if the size of uint64_t and uintptr_t is the same");
    if (min == flag[DATA_MIN])
    {
        fprintf(stderr, "ValueError: Supplied minimum value of \"%"PRIu64"\" cannot be the same as the assigned default of %"PRIu64".", min, flag[DATA_MIN]);
        exit(1);
    }
    if (max == flag[DATA_MAX])
    {
        fprintf(stderr, "ValueError: Supplied maximum value of \"%"PRIu64"\" cannot be the same as the assigned default of %"PRIu64".", max, flag[DATA_MAX]);
        exit(1);
    }
    flag[DATA_MIN] = min;
    flag[DATA_MAX] = max;
}

/*
* @brief Allocate a string flag.
* @param name Name of the flag to be printed in a help message.
* @param def Default string value of the flag.
* @param desc Description of the flag to be printed in the help message.
* @returns A pointer to the c-string value of the provided flag.
*/
char **flag_string(char *name, char *def, char *desc)
{
    Flag *flag = flag_new(FLAG_STR, name, desc);
    *((char **) &flag->data[DATA_DEF]) = def;
    *((char **) &flag->data[DATA_VAL]) = def;
    return (char **) &flag->data[DATA_VAL];
}

/*
* @brief Shift each argument provided as parced at runtime.
* @param argc A pointer to the arguments count provided in `main`. This parametre is decremented, thus its mutability.
* @param argv A pointer to a dynamic array of strings provided in `main`.
* @returns A string representation of the parced argument.
* @exception If there are no arguments provided, a `RuntimeError` is printed and the programme exits.
*/
static char *_flag_shift_args(int *argc, char ***argv)
{
    if (*argc <= 0)
    {
        // fprintf(stderr, "RuntimeError: argc is less than or equal to zero.\n");
        fprintf(stderr, "RuntimeError: No arguments have been provided.\n");
        exit(1);
    }
    char *result = **argv;
    *argv += 1; // Pointer arithmatic - since the array is stored as the pointer to the first element, we increment what the pointer is pointing to.
    *argc -= 1;
    return result;
}

// TODO: I've tried to refactor this to look more clean. After I do my changes, everything breaks. I don't know. Fix this eventually.

/*
* @brief Internal implementation of the flag parsing.
* @param argc Argument count provided in `main`.
* @param argv String array of runtime arguments provided in `main`.
*/
static void _flag_scan(int argc, char **argv)
{
    _flag_shift_args(&argc, &argv);
    while (argc > 0)
    {
        char *flag = _flag_shift_args(&argc, &argv);
        if (!strcmp(flag, "-"))
        {
            fprintf(stderr, "ValueError: Unknown Flag \"%s\".", flag);
            exit(1);
        }
        flag += 1;
        for (size_t i = 0; i < flag_count; ++i)
        {
            if (!strcmp(flags[i].name, flag))
            {
                switch (flags[i].type)
                {
                    case FLAG_BOOL:
                    {
                        *(bool *)&flags[i].data = true;
                    } break;
                    case FLAG_STR:
                    {
                        if (argc == 0){
                            fprintf(stderr, "RuntimeError: No argument provided for \"-%s\".", flag);
                            exit(1);
                        }
                        char *arg = _flag_shift_args(&argc, &argv);
                        *(char**)&flags[i].data = arg;
                    } break;
                    case FLAG_UINT64:
                    {
                        if (argc == 0)
                        {
                            fprintf(stderr, "RuntimeError: No argument provided for \"-%s\".", flag);
                            exit(1);
                        }
                        char *arg = _flag_shift_args(&argc, &argv);
                        static_assert(sizeof(unsigned long long int) == sizeof(uint64_t), "The original author designed this for x86_64 machine with the compiler that expects unsigned long long int and uint64_t to be the same thing, so they could use strtoull() function to parse it. Please adjust this code for your case and maybe even send the patch to upstream to make it work on a wider range of environments.");
                        char *endptr;
                        unsigned long long int result = strtoull(arg, &endptr, 10);
                        if (arg == endptr || strcmp(endptr, "\0"))
                        {
                            fprintf(stderr, "RuntimeError: '%s' is not a valid number.", flag);
                            exit(1);
                        };
                        if (result == ULLONG_MAX && errno == ERANGE)
                        {
                            fprintf(stderr, "OverflowError: '%s' is a 64 bit unsigned integer overflow\n", flag);
                            exit(1);
                        }
                        uint64_t val = result;
                        uint64_t min = *(uint64_t*)&flags[i].data[DATA_MIN];
                        uint64_t max = *(uint64_t*)&flags[i].data[DATA_MAX];

                        if (!(min <= val && val <= max)) {
                            fprintf(stderr, "ValueError: The value provided for the argument \"-%s\" is outside of the [%"PRIu64"..%"PRIu64"] range\n", flag, min, max);
                            exit(1);
                        }
                        *(uint64_t *)&flags[i].data = result;
                    } break;
                    default:
                    {
                        fprintf(stderr, "UnreachableError: Execution has reached an unreachable section.\n");
                        exit(1);
                    }
                }
            };
        };
    };
}

static char *_uint64_to_string(uintptr_t data)
{
    int n = snprintf(NULL, 0, "%"PRIu64, *(uint64_t*) &data);
    // assert(n>=0);
    if (n < 0)
    {
        fprintf(stderr, "ValueError: Can not convert the given number to a string.\n");
        exit(1);
    }
    // assert(flag_tmp_str_size + n + 1 <= FLAG_TMP_STR_CAPACITY);
    if (flag_tmp_str_size + n + 1 > FLAG_TMP_STR_CAPACITY)
    {
        fprintf(stderr, "AllocationError: There is not enough space to allocate a temporary string.\n");
        exit(1);
    }
    int m = snprintf(flag_tmp_str + flag_tmp_str_size, FLAG_TMP_STR_CAPACITY - flag_tmp_str_size, "%"PRIu64, *(uint64_t*) &data);
    // assert(n == m);
    if (n != m)
    {
        fprintf(stderr, "Exception: Can not convert temporary buffer to string.\n");
        exit(1);
    }
    char *result = flag_tmp_str + flag_tmp_str_size;
    flag_tmp_str_size += n + 1;
    return result;
}

static char *_flag_show_data(Flag_Type type, uintptr_t data)
{
    switch(type)
    {
        case FLAG_BOOL:
            return (*(bool*) &data) ? "true" : "false";
        case FLAG_UINT64: {
            return _uint64_to_string(data);
        }
        case FLAG_STR:
            return *(char**)&data;
    };
    fprintf(stderr, "UnreachableError: Execution has reached an unreachable section.\n");
    exit(1);
}

/*
* @brief Print a help message to a given stream.
* @param stream A pointer to an ouput stream where the ouptut message will be printed.
* @param print_default A boolean flag to customize the printing of the default values of each flag. By default the value should be false.
*/
void flag_print_help(FILE *stream, bool print_default)
{
    for (size_t i = 0; i < flag_count; ++i)
    {
        fprintf(stream, "\t-%s\n", flags[i].name);
        fprintf(stream, "\t\t%s", flags[i].desc);
        if (!strcmp(flags[i].name, "help"))
        {
            fprintf(stream, "\n");
        }
        else if (print_default)
        {
            fprintf(stream, " (Default: %s)\n", _flag_show_data(flags[i].type, flags[i].data[DATA_DEF]));
        }
        if (flags[i].type == FLAG_UINT64)
        {
            fprintf(stream, "\t\tMinimum: %s\n", _flag_show_data(flags[i].type, flags[i].data[DATA_MIN]));
            fprintf(stream, "\t\tMaximum: %s\n", _flag_show_data(flags[i].type, flags[i].data[DATA_MAX]));
        }
        else {
            fprintf(stream, "\n");
        }
        flag_tmp_str_size = 0;
    }
}

/*
* @brief Set the name of the programme that takes in the flags. The name of the programme will be displayed in the help menu. A global `PROGRAMME_NAME` variable will be set to be used in the `flag_parse` function. If this function is not called, then the first argument supplied to `argv` will be used.
* @param name String to set as the name of the programme marked with const.
*/
void flag_set_programme_name(const char *name)
{
    _PROGRAMME_NAME = name;
}

/*
* @brief Parse the flags provided to the programme at runtime. This function must be called for any of the flags to be parsed.
* @param argc Argument count provided in `main`.
* @param argv String array of runtime arguments provided in `main`.
*/
void flag_parse(int argc, char **argv)
{
    char *programme = argv[0];
    bool *help = flag_bool("help", false, "Print this help to stdout and exit with 0.");
    _flag_scan(argc, argv);
    if (*help) {
        // assert(argc >= 1);
        if (argc < 1)
        {
            fprintf(stderr, "ValueError: There is not enough arguments to parse.\n");
            exit(1);
        }
        fprintf(stdout, "Usage: %s [OPTIONS]\nOPTIONS:\n", _PROGRAMME_NAME ? _PROGRAMME_NAME : programme);
        flag_print_help(stdout, true);
        exit(1);
    };
}

#if defined(__cplusplus)
}
#endif

#endif // FLAG_IMPLEMENTATION