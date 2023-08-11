#ifndef PATHLIB_H_
#define PATHLIB_H_

#ifdef _WIN32
#include <direct.h>
#define PATH_SEPERATOR '\\'
#define _PATH_SEPERATOR_LITERAL "\\"
#else
#define PATH_SEPERATOR '/'
#define _PATH_SEPERATOR_LITERAL "/"
#endif // _WIN32

#include <stdlib.h> // _MAX_PATH
#include <stdbool.h> // bool
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>


#ifndef MAX_PATH_SIZE
#define MAX_PATH_SIZE _MAX_PATH
#endif // MAX_PATH_SIZE

typedef struct
{
    char *raw;
} Path;

Path path_new();
Path path_from(const char *path);
bool path_exists(const Path *path);
Path pasb(const Path *path);
char *path_to_string(const Path *path);

#endif // PATHLIB_H_

// #ifdef PATHLIB_IMPLEMENTATION

static char *_get_absolute(char *relative_path)
{
    char buffer[MAX_PATH_SIZE];
    char *rel_path = _fullpath(buffer, relative_path, MAX_PATH_SIZE);
    if (NULL == rel_path)
    {
        fprintf(stderr, "IOError: Unable to convert relative path '%s' to an absolute path.\n", relative_path);
        exit(1);
    }
    return rel_path;
}

static bool _file_exists(char *filepath)
{
    struct stat info;
    if (stat(filepath, &info) != 0)
    {
        return false;
    }
    else if (NULL == fopen(filepath, "r"))
    {
        return false;
    }
    return true;
}

static void _make_file(char *filepath)
{
    FILE *file = fopen(filepath, "w");
    if (NULL == file || errno == EEXIST)
    {
        fprintf(stderr, "IOError: Unable to create file %s\n", filepath);
        fclose(file);
        exit(1);
    }
    fclose(file);
}

Path path_new()
{
    Path path = {0};
    path.raw = ".";
    return path;
}
Path path_from(const char *path);
bool path_exists(const Path *path)
{
    return _file_exists(path_to_string(path)) || errno == EEXIST;
}

Path pasb(const Path *path)
{
    char *abs = _get_absolute(path_to_string(path));
    Path return_path = {0};
    if (strlen(path->raw) == 0)
    {
        return_path.raw = "";
    }
    return_path.raw = abs;
    return return_path;
}

char *path_to_string(const Path *path)
{
    return path->raw;
}

// #endif // PATHLIB_IMPLEMENTATION
