#ifndef FLAG_H_
#define FLAG_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>
#include <assert.h>
#include <errno.h>
#include <limits.h>

#define FLAG_CAPACITY 256

typedef enum
{
    FLAG_BOOL,
    FLAG_UINT64,
    FLAG_STR
} Flag_Type;

typedef enum
{
    DATA_VAL,
    DATA_DEF,
    DATA_MIN,
    DATA_MAX
} Flag_Data;

typedef struct
{
    Flag_Type type;
    char *name;
    char *desc;
    uintptr_t data[sizeof(Flag_Data)];
} Flag;

static Flag flags[FLAG_CAPACITY];
static size_t flag_count = 0;

#define FLAG_TMP_STR_CAPACITY 1024
static char flag_tmp_str[FLAG_TMP_STR_CAPACITY];
static size_t flag_tmp_str_size = 0;

Flag *flag_new(Flag_Type type, char *name, char *desc);
bool *flag_bool(char *name, bool def, char *desc);
uint64_t *flag_uint64(char *name, uint64_t def, char *desc);
void flag_uint64_range(uint64_t *flag, uint64_t min, uint64_t max);
char **flag_str(char *name, char *def, char *desc);
void flag_print_help(FILE *stream, bool print_default);
void flag_parse(int argc, char **argv);

#endif // FLAG_H_

#ifndef FLAG_IMPLEMENTATION
#define FLAG_IMPLEMENTATION

Flag *flag_new(Flag_Type type, char *name, char *desc)
{
    // make sure the count doesn't exceed the capacity
    assert(flag_count < FLAG_CAPACITY);
    // allocate a pointer on the stack to the address of the flag count after it's been incremented
    Flag *flag = &flags[flag_count++];
    // set values
    flag->type = type;
    flag->name = name;
    flag->desc = desc;
    return flag;
}

bool *flag_bool(char *name, bool def, char *desc)
{
    Flag *flag = flag_new(FLAG_BOOL, name, desc);
    *((bool*) &flag->data[DATA_DEF]) = def; // reinterpret the adress of the default value of the flag to the boolean address of the given default
    *((bool*) &flag->data[DATA_VAL]) = def; // doing the same as above but for the data as default
    return (bool*) &flag->data[DATA_VAL]; // return the address as a boolean
}

uint64_t *flag_uint64(char *name, uint64_t def, char *desc)
{
    Flag *flag = flag_new(FLAG_UINT64, name, desc);
    *((uint64_t*) &flag->data[DATA_DEF]) = def;
    *((uint64_t*) &flag->data[DATA_VAL]) = def;
    *((uint64_t*) &flag->data[DATA_MIN]) = 0;
    *((uint64_t*) &flag->data[DATA_MAX]) = UINT64_MAX;
    return (uint64_t*) &flag->data[DATA_VAL];
}

void flag_uint64_range(uint64_t *flag, uint64_t min, uint64_t max)
{
    static_assert(sizeof(uint64_t) == sizeof(uintptr_t), "This will only work if the size of uint64_t and uintptr_t is the same");
    if (min == flag[DATA_MIN])
    {
        fprintf(stderr, "ERROR: Supplied minimum value of \"%"PRIu64"\" cannot be the same as the assigned default of %"PRIu64".", min, flag[DATA_MIN]);
    }
    if (max == flag[DATA_MAX])
    {
        fprintf(stderr, "ERROR: Supplied maximum value of \"%"PRIu64"\" cannot be the same as the assigned default of %"PRIu64".", max, flag[DATA_MAX]);
    }
    flag[DATA_MIN] = min;
    flag[DATA_MAX] = max;
}

char **flag_str(char *name, char *def, char *desc)
{
    Flag *flag = flag_new(FLAG_STR, name, desc);
    *((char **) &flag->data[DATA_DEF]) = def;
    *((char **) &flag->data[DATA_VAL]) = def;
    return (char **) &flag->data[DATA_VAL];
}

static char *flag_shift_args(int *argc, char ***argv)
{
    assert(*argc > 0);
    char *result = **argv;
    *argv += 1;
    *argc -= 1;
    return result;
}

static void flag_scan(int argc, char **argv)
{
    flag_shift_args(&argc, &argv);
    while (argc > 0)
    {
        char *flag = flag_shift_args(&argc, &argv);
        if (!strcmp(flag, "-"))
        {
            fprintf(stderr, "ERROR: Unknown Flag \"%s\".", flag);
            exit(1);
        }
        flag += 1;
        for (size_t i = 0; i < flag_count; ++i)
        {
            if (!strcmp(flags[i].name, flag))
            {
                switch (flags[i].type)
                {
                    case FLAG_BOOL: {
                        *(bool *)&flags[i].data = true;
                    } break;
                    case FLAG_STR: {
                        if (argc == 0){
                            fprintf(stderr, "ERROR: No argument provided for \"-%s\".", flag);
                            exit(1);
                        }
                        char *arg = flag_shift_args(&argc, &argv);
                        *(char**)&flags[i].data = arg;
                    } break;
                    case FLAG_UINT64: {
                        if (argc == 0)
                        {
                            fprintf(stderr, "ERROR: No argument provided for \"-%s\".", flag);
                            exit(1);
                        }
                        char *arg = flag_shift_args(&argc, &argv);
                        static_assert(sizeof(unsigned long long int) == sizeof(uint64_t), "The original author designed this for x86_64 machine with the compiler that expects unsigned long long int and uint64_t to be the same thing, so they could use strtoull() function to parse it. Please adjust this code for your case and maybe even send the patch to upstream to make it work on a wider range of environments.");
                        char *endptr;
                        unsigned long long int result = strtoull(arg, &endptr, 10);
                        if (arg == endptr || strcmp(endptr, "\0"))
                        {
                            fprintf(stderr, "ERROR %s is not a valid number.", flag);
                            exit(1);
                        };
                        if (result == ULLONG_MAX && errno == ERANGE)
                        {
                            fprintf(stderr, "ERROR %s is a 64 bit unsigned integer overflow\n", flag);
                            exit(1);
                        }
                        uint64_t val = result;
                        uint64_t min = *(uint64_t*)&flags[i].data[DATA_MIN];
                        uint64_t max = *(uint64_t*)&flags[i].data[DATA_MAX];

                        if (!(min <= val && val <= max)) {
                            fprintf(stderr, "ERROR: The value provided for the argument \"-%s\" is outside of the [%"PRIu64"..%"PRIu64"] range\n", flag, min, max);
                            exit(1);
                        }
                        *(uint64_t *)&flags[i].data = result;
                    } break;
                    default: {
                        assert(0 && "unreachable");
                        exit(69);
                    }
                }
            };
        };
    };
}

static char *flag_show_data(Flag_Type type, uintptr_t data)
{
    switch(type)
    {
        case FLAG_BOOL:
            return (*(bool*) &data) ? "true" : "false";
        case FLAG_UINT64: {
            int n = snprintf(NULL, 0, "%"PRIu64, *(uint64_t*) &data);
            assert(n>=0);
            assert(flag_tmp_str_size + n + 1 <= FLAG_TMP_STR_CAPACITY);
            int m = snprintf(flag_tmp_str + flag_tmp_str_size, FLAG_TMP_STR_CAPACITY - flag_tmp_str_size, "%"PRIu64, *(uint64_t*) &data);
            assert(n == m);
            char *result = flag_tmp_str + flag_tmp_str_size;
            flag_tmp_str_size += n + 1;
            return result;
        }
        case FLAG_STR:
            return *(char**)&data;
    };
    assert(0 && "unreachable");
    exit(69);
}

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
            fprintf(stream, " (Default: %s)\n", flag_show_data(flags[i].type, flags[i].data[DATA_DEF]));
        }
        if (flags[i].type == FLAG_UINT64)
        {
            fprintf(stream, "\t\tMinimum: %s\n", flag_show_data(flags[i].type, flags[i].data[DATA_MIN]));
            fprintf(stream, "\t\tMaximum: %s\n", flag_show_data(flags[i].type, flags[i].data[DATA_MAX]));
        }
        else {
            fprintf(stream, "\n");
        }
        flag_tmp_str_size = 0;
    }
}

void flag_parse(int argc, char **argv)
{
    char *programme = argv[0];
    bool *help = flag_bool("help", false, "Print this help to stdout and exit with 0.");
    flag_scan(argc, argv);
    if (*help) {
        assert(argc >= 1);
        fprintf(stdout, "Usage: %s [OPTIONS]\nOPTIONS:\n", programme);
        flag_print_help(stdout, true);
        exit(1);
    };
}

#endif