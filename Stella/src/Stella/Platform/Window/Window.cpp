#include "Window.h"
#include "StellaConfig.h"
#include "Stella/Platform/Platform.h"
#include "Stella/Core/Assert/Assert.h"

#include <stdio.h>

namespace Stella
{

static void window_framebuffer_size_callback(GLFWwindow* window, int width, int height);

Window::Window(uint32_t w, uint32_t h) :
    m_width(w), m_height(h)
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    m_pWindowHandle = glfwCreateWindow(w, h, "Stella Window", NULL, NULL);
    if (m_pWindowHandle == NULL)
    {
        glfwTerminate();
        STELLA_ASSERT((0), "Failed to create GLFW Window");
    }

    glfwMakeContextCurrent(m_pWindowHandle);
    
    // Event Callbacks
    glfwSetFramebufferSizeCallback(m_pWindowHandle, window_framebuffer_size_callback);

    glfwSetWindowPos(m_pWindowHandle, 250, 75);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    STELLA_ASSERT((gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)), "Failed to initialize glad");
}

Window::~Window()
{
    glfwTerminate();
}

void Window::swap_buffers()
{
    glfwSwapBuffers(m_pWindowHandle);
}

void Window::poll_events()
{
    glfwPollEvents();
}

glm::vec2 Window::get_framebuffer_dims()
{
    int x;
    int y;
    glfwGetFramebufferSize(m_pWindowHandle, &x, &y);
    return glm::vec2(x, y);
}

bool Window::is_open()
{
    return !glfwWindowShouldClose(m_pWindowHandle);
}

void window_framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

} // namespace Stella
