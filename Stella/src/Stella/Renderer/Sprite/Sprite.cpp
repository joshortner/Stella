#include "Sprite.h"
#include "Stella/EngineState.h"
#include "Stella/Core/Assert/Assert.h"
#include "Stella/Core/Logger/Logger.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glad/glad.h>

namespace Stella
{

Sprite::Sprite(std::string file_path)
{
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    //int width, height, nrChannels;
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char *data = stbi_load(file_path.c_str(), &m_width, &m_height, &m_nChannels, 0);
    STELLA_ENGINE_LOG("Texture Data: (w=%d)(h=%d)(chan=%d)", m_width, m_height, m_nChannels);

    if (data) {
        uint32_t format;

        if (m_nChannels == 3) {
            format = GL_RGB;
        }
        else if (m_nChannels == 4) {
            format = GL_RGBA;
        }
        else {
            STELLA_ASSERT((0), "Texture files with (%d) channels is unsupported", m_nChannels);
        }

        glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        STELLA_ASSERT((0), "Unable to load texture: %s", file_path.c_str());
    }
    stbi_image_free(data);
}

Sprite::~Sprite()
{
    glDeleteTextures(1, &m_id);
}

void Sprite::bind()
{
    glBindTexture(GL_TEXTURE_2D, m_id);
}

void Sprite::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

uint32_t Sprite::get_id() const
{
    return m_id;
}
    
} // namespace Stella
