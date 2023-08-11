#ifndef PATHLIB_HPP_
#define PATHLIB_HPP_

#ifdef _WIN32
#include <direct.h>
#define PATH_SEPERATOR "\\"
#else // TODO: Include the linux implementation
#include <filesystem>
#define PATH_SEPERATOR "/"
#endif // _WIN32

#include <string> // std::string

#ifndef MAX_PATH_SIZE
#define MAX_PATH_SIZE _MAX_PATH
#endif // MAX_PATH_SIZE

class Path
{
    public:
        Path();
        Path(const std::string path);
        bool exists();
        Path absolute();
        std::string to_string();
        Path operator/(Path &child);
        Path operator/(const std::string child);
        bool is_relative();
        Path &get_parent();
        void mkdir();
        void touch();
    private:
        std::string __path;
};


static void _make_directory(const std::string path);
static void _make_file(const std::string filename);
static std::string _get_absolute(const std::string relative_path);
static std::string _path_append(const std::string parent, const std::string child);

#endif // PATH_HPP_

#ifdef PATHLIB_IMPLEMENTATION

static void _make_directory(const std::string path)
{
    int error = mkdir(path.data());
    if (0 > error && (errno != EEXIST))
    {
        std::cerr << "Unable to create directory: " << path << "\n";
        exit(1);
    }
    else if (errno == EEXIST)
    {
        std::cerr << path.data() << " already exits" << "\n";
        exit(1);
    }
}

static std::string _get_absolute(const std::string relative_path)
{
    char buffer[MAX_PATH_SIZE];
    char *rel_path = _fullpath(buffer, relative_path.data(), MAX_PATH_SIZE);
    if (NULL == rel_path)
    {
        std::cerr << "Unable to convert a relative path to absolute" << "\n";
        exit(1);
    }
    return std::string(rel_path);
}

static std::string _path_append(const std::string parent, const std::string child)
{
    return parent + PATH_SEPERATOR + child;
}

#endif // PATHLIB_IMPLEMENTATION

// #endif // PATHLIB_HPP_