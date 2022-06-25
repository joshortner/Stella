#include "Texture.h"
#include "Stella/Core/Assert/Assert.h"

#include <glad/glad.h>
#include <stb_image.h>

#define IDX3TO1(_i, _j, _c, _w, _d) (((((_i) * (_w)) + (_j)) * (_d)) + (_c))

namespace Stella
{

bool Texture2D::create(Texture2D * p_tex, const Texture2DInfo & info)
{
    glGenTextures(1, &p_tex->m_id);
    glBindTexture(GL_TEXTURE_2D, p_tex->m_id); 
    
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, to_render_api_wrap(info.m_wrap));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, to_render_api_wrap(info.m_wrap));
    
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, to_render_api_filter(info.m_minFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, to_render_api_filter(info.m_magFilter));

    if (info.m_pData == nullptr) {
        // load image, create texture and generate mipmaps
        if (info.m_flip) {
            stbi_set_flip_vertically_on_load(true); 
        }
        unsigned char * data = stbi_load(info.m_fileName.c_str(), (int *)&p_tex->m_width, (int *)&p_tex->m_height, (int *)&p_tex->m_channels, 0);
        bool success = false;
        if (data) {
            glTexImage2D(
                GL_TEXTURE_2D, 
                0, 
                to_render_api_format(info.m_internalFormat), 
                p_tex->m_width, 
                p_tex->m_height, 
                0, 
                to_render_api_format(info.m_dataFormat), 
                to_render_api_data_type(info.m_dataType), 
                data
            );
            if (info.m_generateMipMap) {
                glGenerateMipmap(GL_TEXTURE_2D);
            }
            success = true;
        }
        stbi_image_free(data);
        return success;
    }
    else {

        uint8_t channels = texture_format_channel_count(info.m_dataFormat);
        uint8_t * final_data = nullptr;
        if (info.m_flip) {
            final_data = new uint8_t[info.m_width * info.m_height * channels];
            flip_vertical(info.m_width, info.m_height, channels, info.m_pData, final_data);
        }
        else {
            final_data = info.m_pData;
        }

        p_tex->m_width  = info.m_width;
        p_tex->m_height = info.m_height;
        glTexImage2D(
            GL_TEXTURE_2D, 
            0, 
            to_render_api_format(info.m_internalFormat), 
            p_tex->m_width, 
            p_tex->m_height, 
            0, 
            to_render_api_format(info.m_dataFormat), 
            to_render_api_data_type(info.m_dataType), 
            final_data 
        );
        
        if (info.m_generateMipMap) {
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        if (info.m_flip) {
            delete final_data;
        }
    }
    return true;
}

void Texture2D::destroy(Texture2D * p_tex)
{
    glDeleteTextures(1, &p_tex->m_id);
}

void Texture2D::bind()
{
    glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture2D::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

int Texture2D::to_render_api_wrap(TextureWrap wrapping)
{
    switch (wrapping)
    {
        case TextureWrap::REPEAT: 		   return GL_REPEAT;
        case TextureWrap::MIRRORED_REPEAT: return GL_MIRRORED_REPEAT;
        case TextureWrap::CLAMP_TO_EDGE:   return GL_CLAMP_TO_EDGE;
    }
    return GL_NONE;
}

int Texture2D::to_render_api_filter(TextureFilter filter)
{
    switch (filter)
    {
        case TextureFilter::LINEAR: return GL_LINEAR;
        case TextureFilter::NEAREST: return GL_NEAREST;
        case TextureFilter::NEAREST_MIPMAP_NEAREST: return GL_NEAREST_MIPMAP_NEAREST;
        case TextureFilter::LINEAR_MIPMAP_NEAREST: return GL_LINEAR_MIPMAP_NEAREST;
        case TextureFilter::NEAREST_MIPMAP_LINEAR: return GL_NEAREST_MIPMAP_LINEAR;
        case TextureFilter::LINEAR_MIPMAP_LINEAR: return GL_LINEAR_MIPMAP_LINEAR;
    }
    return GL_NONE;
}

int Texture2D::to_render_api_format(TextureFormat format)
{
    switch (format)
    {
        case TextureFormat::RGB: return GL_RGB; 
        case TextureFormat::RGBA: return GL_RGBA; 
    }
    return GL_NONE;
}

int Texture2D::to_render_api_data_type(TextureDataType type)
{
    switch (type)
    {
        case TextureDataType::UNSIGNED_BYTE: return GL_UNSIGNED_BYTE;
    }
    return GL_NONE;
}

uint8_t Texture2D::texture_format_channel_count(TextureFormat format)
{
    switch (format)
    {
        case TextureFormat::RGB: return 3; 
        case TextureFormat::RGBA: return 4; 
    }
    return 0;
}

void Texture2D::flip_vertical(uint32_t width, uint32_t height, uint32_t channels, uint8_t * to_flip, uint8_t * out)
{
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            for (int c = 0; c < channels; c++) {
                out[IDX3TO1(i, j, c, width, channels)] = to_flip[IDX3TO1((height - 1) - i, j, c, width, channels)];
            }
        }
    }
}



} // namespace Stella
