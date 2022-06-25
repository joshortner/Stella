#include "Path.h"

#include <algorithm>
#include <filesystem>
#include <stdlib.h>

namespace Stella
{

//
// Default constructor
//
Path::Path() :
    m_path("") { }

//
// Construct a path with a string
//
Path::Path(std::string s) :
    m_path(s)
{
    if (PATH_DELIMITER == '\\') std::replace(m_path.begin(), m_path.end(), '/', PATH_DELIMITER);
    if (PATH_DELIMITER == '/')  std::replace(m_path.begin(), m_path.end(), '\\', PATH_DELIMITER);
}

//
// Returns a new path by appending argument to the existing path
//
Path Path::join_path(std::string s) 
{
    if (s == "") 
        return (*this);

    std::string temp(m_path);

    if (s.back() != PATH_DELIMITER) 
        temp += PATH_DELIMITER;

    temp += s;

    return Path(temp);
}

//
// Returns true if the path is a file
//
bool Path::is_file() const
{
    return std::filesystem::exists(m_path);
}

//
// Returns true if the path is a directory
//
bool Path::is_dir() const
{
    return std::filesystem::exists(m_path) && std::filesystem::is_directory(m_path);
}

//
// Returns const char pointer to internal data
//
const char* Path::c_str()
{
    return m_path.c_str();
}

//
// Returns const char pointer to internal data
//
const char* Path::c_str() const
{
    return m_path.c_str();
}

//
// Erases the path making it an empty string: ""
//
void Path::clear()
{
    m_path.clear();
}

//
// Returns true if path is empty
//
bool Path::is_empty()
{
    return m_path.empty();
}

//
// Returns the children paths in a directory
//
std::vector<Path> Path::list_dir() const
{
    std::vector<Path> dir_contents;

    if (this->is_dir())
    {
        for (const auto &file : std::filesystem::directory_iterator(m_path.c_str()))
        {
            dir_contents.push_back(file.path().string());
        }
    }

    return dir_contents;
}

//
// Creates path if it's a directory
//
bool Path::create() const
{
    if (is_file()) {
        return false;
    }
    else {
        return std::filesystem::create_directory(m_path);
    }
}

//
// Returns the final path component
//
std::string Path::name()
{
    std::size_t found = m_path.find_last_of(PATH_DELIMITER);

    if (found != std::string::npos)
    {
        return m_path.substr(found + 1);
    }
    else
    {
        return m_path;
    }
}

//
// Returns the final path component
//
std::string Path::name() const
{
    std::size_t found = m_path.find_last_of(PATH_DELIMITER);

    if (found != std::string::npos)
    {
        return m_path.substr(found + 1);
    }
    else
    {
        return m_path;
    }
}

//
// Returns the final path component without the suffix
//
std::string Path::stem()
{
    std::string n = name();
    std::size_t found = n.find(".");
    if (found != std::string::npos)
    {
        return n.substr(0, found);
    }
    return n;
}

//
// Returns the final path component without the suffix
//
std::string Path::stem() const
{
    std::string n = name();
    std::size_t found = n.find(".");
    if (found != std::string::npos)
    {
        return n.substr(0, found);
    }
    return n;
}

Path Path::parent()
{
    if (is_empty()) return Path("");
    if (m_path == "/") return (*this);

    std::size_t found = m_path.find_last_of(PATH_DELIMITER);

    if (found != std::string::npos)
    {
        if (found == 0) return Path("/");
        else return m_path.substr(0, found);
    }
    else 
    {
        return (*this);
    }
}

//
// Returns home directory as as path
//
Path Path::home()
{
    return Path(getenv("HOME"));
}

bool Path::operator != (const Path& rhs)
{
    return m_path != rhs.m_path;
}

} // namespace Stella