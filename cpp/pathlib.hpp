#ifndef PATHLIB_HPP_
#define PATHLIB_HPP_

#ifdef _WIN32
#include <direct.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define PATH_SEPERATOR "\\"
#else // TODO: Include the linux implementation
#include <filesystem>
#include <sys/stat.h>
#define PATH_SEPERATOR "/"
#endif // _WIN32

#include <string> // std::string

#include <fstream> // std::ofstream
#include <iostream> // std::cerr

#ifndef MAX_PATH_SIZE
#define MAX_PATH_SIZE _MAX_PATH
#endif // MAX_PATH_SIZE


std::string __path_buffer;


class Path
{
    public:
        explicit Path() noexcept;
        Path(const std::string path) noexcept;
        bool exists() const;
        Path absolute();
        const std::string to_string() const noexcept;
        Path operator/(Path &child) const noexcept;
        Path operator/(const std::string child) const noexcept;
        bool is_relative();
        Path get_parent();
        void mkdir();
        void touch();
    private:
        std::string __path;
};


// static bool _is_directory(const std::string path);
// static bool _path_is_file(const std::string path);
static void _make_directory(const std::string path);
static void _make_file(const std::string filename);
static std::string _get_absolute(const std::string relative_path);
static std::string _path_append(const std::string parent, const std::string child);

#endif // PATH_HPP_

#ifdef PATHLIB_IMPLEMENTATION

static void _make_directory(const std::string path)
{
#ifdef _WIN32
    int error = _mkdir(path.data());
    if(error < 0 || errno == EEXIST || errno == ENOENT)
    {
        fprintf(stderr, "IOError: Cannot create directory: %s\n", strerror(errno));
        exit(1);
    }
#else
    int error = mkdir(path.raw, 0755);
    if(error < 0 || errno == EEXIST || errno == ENOENT)
    {
        std::cerr << "Unable to create directory: " << path << "\n";
        exit(1);
    }
#endif // _WIN32
}

static void _make_file(const std::string filename)
{
    std::ofstream file{filename};
}


static std::string _get_absolute(const std::string relative_path)
{
#ifdef _WIN32
    if(GetFullPathName(relative_path.c_str(), MAX_PATH_SIZE, const_cast<char*>(__path_buffer.c_str()), NULL) == 0)
    {
        std::cerr << "Unable to get the full path of " << relative_path << "\n";
        exit(1);
    }
    return std::string(__path_buffer.c_str());
#else
    // TODO: Implement the linux version.
#endif // _WIN32
}

static std::string _path_append(const std::string parent, const std::string child)
{
    return parent + PATH_SEPERATOR + child;
}

Path::Path() noexcept
{
    __path = ".";
}

Path::Path(const std::string path) noexcept
{
    __path = path.data();
}

bool Path::exists() const
{
#ifdef _WIN32
    DWORD dwAttrib = GetFileAttributes(to_string().c_str());
    return (dwAttrib != INVALID_FILE_ATTRIBUTES);
#else
    struct stat stat_buffer = {0};
    if (stat(to_string().c_str(), &stat_buffer) < 0)
    {
        if (errno == ENOENT)
        {
            errno = 0;
            return false;
        }
    }
    return S_ISDIR(stat_buffer.st_mode);
#endif // _WIN32
}

Path Path::absolute()
{
    const std::string abs = _get_absolute(__path);
    __path.clear();
    __path = abs;
    return *this;
}

const std::string Path::to_string() const noexcept
{
    return __path;
}

Path Path::operator/(Path &child) const noexcept
{
    return Path(_path_append(to_string(), child.to_string()));
}

Path Path::operator/(const std::string child) const noexcept
{
    return Path(_path_append(to_string(), child.data()));
}

bool Path::is_relative()
{
    if ((to_string().find(".") != 0) && (to_string().find("..") != 0))
    {
        return false;
    }
    else if ((to_string().find(".") != to_string().find_last_of(".")) && (to_string().find("..") != to_string().find_last_of("..")))
    {
        return false;
    }
    return true;
}

Path Path::get_parent()
{
    const std::string full_path = absolute().to_string();
    const std::size_t last_stroke = full_path.find_last_of(PATH_SEPERATOR);
    std::string temp = std::string();
    for (std::size_t i = 0; i < last_stroke; ++i)
    {
        temp.push_back(full_path[i]);
    }
    // std::string::iterator it;
    __path.clear();
    __path = temp;
    return *this;
}

void Path::mkdir()
{
    if (exists())
    {
        return;
    }
    std::string full_path = absolute().to_string();
    _make_directory(full_path);
}

void Path::touch()
{
    if (exists())
    {
        return;
    }
    std::string full_path = absolute().to_string();
    _make_file(full_path);
}

#endif // PATHLIB_IMPLEMENTATION