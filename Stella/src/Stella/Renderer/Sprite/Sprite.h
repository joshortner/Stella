#pragma once

#include <stdint.h>

#include <string>

namespace Stella
{

class Sprite
{

public:

    Sprite(std::string file_path);
    ~Sprite();

    void bind();
    void unbind();

    uint32_t get_id() const;

    //void Delete();

private:

    uint32_t m_id;
    int m_width;
    int m_height;
    int m_nChannels;

};
    
} // namespace Stella
