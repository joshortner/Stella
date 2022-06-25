#pragma once

#include <glm/glm.hpp>

#include <stdint.h>

struct GLFWwindow;

namespace Stella
{
    
struct Window
{

public:

    Window(uint32_t w, uint32_t h);
    ~Window();

    bool is_open();

    void swap_buffers();
    void poll_events();
    glm::vec2 get_framebuffer_dims();

    uint32_t m_width;
    uint32_t m_height;

private:

    GLFWwindow * m_pWindowHandle;

};

} // namespace Stella
