#pragma once

#include <string>
#include <vector>

#define PATH_DELIMITER '/'

namespace Stella
{

class Path
{
public:

    Path();
    Path(std::string s);

    Path join_path(std::string s);
    bool is_file() const;
    bool is_dir() const;

    const char* c_str();
    const char* c_str() const;
    void clear();
    bool is_empty();

    std::vector<Path> list_dir() const;

    bool create() const;

    std::string name();
    std::string name() const;

    std::string stem();
    std::string stem() const;

    Path parent();

    static Path home();

    bool operator != (const Path& rhs);

private:

    std::string m_path;

};
    
} // namespace Stella