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
#include <sys/stat.h>
#include <fstream>

#include <iostream>

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


static bool _is_directory(const std::string path);
static bool _path_is_file(const std::string path);
static void _make_directory(const std::string path);
static void _make_file(const std::string filename);
static std::string _get_absolute(const std::string relative_path);
static std::string _path_append(const std::string parent, const std::string child);

#endif // PATH_HPP_

#ifdef PATHLIB_IMPLEMENTATION

static bool _is_directory(const std::string path)
{
    struct stat info;
    if (stat(path.data(), &info) != 0)
    {
        return false;
    }
    else if (!(info.st_mode & S_IFDIR))
    {
        return false;
    }
    else if (info.st_mode & S_IFREG)
    {
        return false;
    }
    return true;
}

static bool _path_is_file(const std::string path)
{
    struct stat info;
    if (stat(path.data(), &info) != 0)
    {
        return false;
    }
    else if (info.st_mode & S_IFDIR)
    {
        return false;
    }
    else if (!(info.st_mode & S_IFREG))
    {
        return false;
    }
    return true;
}

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

static void _make_file(const std::string filename)
{
    std::ofstream file{filename};
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

Path::Path()
{
    this->__path = ".";
}

Path::Path(const std::string path)
{
    this->__path = path.data();
}

bool Path::exists()
{
    return _path_is_file(this->to_string()) || _is_directory(this->to_string());
}

Path Path::absolute()
{
    std::string abs = _get_absolute(this->__path);
    this->__path.clear();
    this->__path = abs;
    return *this;
}

std::string Path::to_string()
{
    return this->__path;
}

Path Path::operator/(Path &child)
{
    return Path(_path_append(this->to_string(), child.to_string()));
}

Path Path::operator/(const std::string child)
{
    return Path(_path_append(this->to_string(), child.data()));
}

bool Path::is_relative()
{
    if ((this->to_string().find(".") != 0) && (this->to_string().find("..") != 0))
    {
        return false;
    }
    else if ((this->to_string().find(".") != this->to_string().find_last_of(".")) && (this->to_string().find("..") != this->to_string().find_last_of("..")))
    {
        return false;
    }
    return true;
}

Path &Path::get_parent()
{
    std::string full_path;
    if (this->is_relative())
    {
        full_path = this->to_string();
    }
    else if (!this->is_relative())
    {
        full_path = this->absolute().to_string();
    }
    std::size_t last_stroke = full_path.find_last_of(PATH_SEPERATOR);
    std::string temp;
    for (std::size_t i = 0; i < last_stroke; ++i)
    {
        temp += full_path[i];
    }
    // std::string::iterator it;
    this->__path.clear();
    this->__path = temp;
    return *this;
}

void Path::mkdir()
{
    std::string full_path;
    if (this->exists())
    {
        return;
    }
    else if (this->is_relative())
    {
        if (!full_path.empty())
        {
            full_path.clear();
        }
        full_path = this->absolute().to_string();
    }
    else if (!(this->is_relative()))
    {
        if (!full_path.empty())
        {
            full_path.clear();
        }
        full_path = this->to_string();
    }
    _make_directory(full_path);
}

void Path::touch()
{
    std::string full_path;
    if (this->exists())
    {
        return;
    }
    else if (!(this->is_relative()))
    {
        full_path = this->absolute().to_string();
    }
    else if (this->is_relative())
    {
        full_path = this->to_string();
    }
    _make_file(full_path);
}

#endif // PATHLIB_IMPLEMENTATION