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

#endif // PATHLIB_HPP_