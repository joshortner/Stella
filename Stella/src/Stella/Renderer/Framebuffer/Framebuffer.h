#pragma once

#include <glm/glm.hpp>

#include <stdint.h>

namespace Stella
{

struct Framebuffer
{
public:

    Framebuffer(uint32_t width, uint32_t height);
    ~Framebuffer();

    void bind();
    void unbind();
    void resize(uint32_t w, uint32_t h);

    uint32_t m_fbo;
    uint32_t m_colorAttachment;
    uint32_t m_depthAttachment;

    static void clear(glm::vec3 clear_color);

private:

    void setup();

    uint32_t m_width;
    uint32_t m_height;

    bool m_setup;

};
    
} // namespace Stella
