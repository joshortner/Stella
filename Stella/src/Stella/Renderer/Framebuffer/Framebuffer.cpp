#include "Framebuffer.h"
#include "Stella/Core/Assert/Assert.h"

#include <glad/glad.h>

namespace Stella
{

Framebuffer::Framebuffer(uint32_t width, uint32_t height) :
    m_width(width), m_height(height), m_setup(false)
{
    setup();
}

Framebuffer::~Framebuffer()
{
    glDeleteBuffers(1, &m_fbo);
    glDeleteTextures(1, &m_colorAttachment);
    glDeleteFramebuffers(1, &m_depthAttachment);
}

void Framebuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glViewport(0, 0, m_width, m_height);
}

void Framebuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::resize(uint32_t w, uint32_t h)
{
    if (w > 0 && h > 0 && (w != m_width || h != m_height))
    {
        m_width  = w;
        m_height = h;
        setup();
    }
}

void Framebuffer::setup()
{
     if (m_setup)
    {
        glDeleteFramebuffers(1, &m_fbo);
        glDeleteTextures(1, &m_colorAttachment);
        glDeleteFramebuffers(1, &m_depthAttachment);
    }

    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    // Color Attachment
    glGenTextures(1, &m_colorAttachment);
    glBindTexture(GL_TEXTURE_2D, m_colorAttachment);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorAttachment, 0);

    // Depth/Stencil Attachment
    glGenTextures(1, &m_depthAttachment);
    glBindTexture(GL_TEXTURE_2D, m_depthAttachment);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_width, m_height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthAttachment, 0);

    auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    STELLA_ASSERT((status == GL_FRAMEBUFFER_COMPLETE), "Framebuffer not complete!");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    m_setup = true;
}

void Framebuffer::clear(glm::vec3 clear_color)
{
    glClearColor(clear_color.r, clear_color.g, clear_color.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
}

    
} // namespace Stella
