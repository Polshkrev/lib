#ifndef PATHLIB_H_
#define PATHLIB_H_

#include <stdlib.h> // _MAX_PATH
// #include <windef.h> // MAX_PATH

#ifndef MAX_PATH_SIZE
#define MAX_PATH_SIZE _MAX_PATH
#endif // MAX_PATH_SIZE

typedef struct
{
    char raw[MAX_PATH_SIZE];
} Path;

Path path_new();
Path path_from(const char *path);
bool path_exists(const Path *path);
Path pasb(const Path *path);
char *path_to_string(const Path *path);

#endif // PATHLIB_H_

#ifdef PATHLIB_IMPLEMENTATION

Path path_new();
Path path_from(const char *path);
bool path_exists(const Path *path);
Path pasb(const Path *path);
char *path_to_string(const Path *path);

#endif // PATHLIB_IMPLEMENTATION
