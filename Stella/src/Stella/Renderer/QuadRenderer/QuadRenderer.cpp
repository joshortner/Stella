#include "QuadRenderer.h"
#include "StellaDefines.h"
#include "Stella/EngineState.h"
#include "Stella/Renderer/Shader/Shader.h"
#include "Stella/Renderer/Camera/Camera.h"
#include "Stella/Renderer/Sprite/Sprite.h"
#include "Stella/Resources/ShaderManager/ShaderManager.h"

#include <glad/glad.h>

namespace Stella
{

QuadBatchRenderState::QuadBatchRenderState() :
    m_pCamera(nullptr), m_pShader(nullptr), m_textureSlotIdx(1)
{
    uint32_t quad_indices[MAX_QUADS * 6];
    uint32_t offset = 0;
    for ( uint32_t i = 0; i < MAX_QUADS * 6; i += 6 ) {
        quad_indices[i + 0] = offset + 0;
        quad_indices[i + 1] = offset + 1;
        quad_indices[i + 2] = offset + 3;

        quad_indices[i + 3] = offset + 1;
        quad_indices[i + 4] = offset + 2;
        quad_indices[i + 5] = offset + 3;

        offset += 4;
    }

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);
    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(QuadVertex) * MAX_QUADS * 4, nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void *)0); // Position
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void *)(3 * sizeof(float))); // Color
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void *)(7 * sizeof(float))); // UV
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void *)(9 * sizeof(float))); // Texture id
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quad_indices), quad_indices, GL_STATIC_DRAW);

    glBindVertexArray(0);

    int tex_units_supported;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &tex_units_supported);
    //STELLA_LOG(Logger::INFO, "Graphics: Max Texture Units Supported = %d", tex_units_supported);

    //TODO: Shader will need to reflect tex_units_supported...
    m_textureSlots.resize(8);

}

void QuadBatchRenderState::start(Camera * p_cam)
{
    m_pCamera = p_cam;
    m_vertexBufferPtr = m_vertexBufferBase;
}

void QuadBatchRenderState::end()
{
    flush();
    m_pCamera = nullptr;
}

void QuadBatchRenderState::push_quad(const glm::vec3 & tr, const glm::vec3 & br, const glm::vec3 & bl, const glm::vec3 & tl, const glm::vec4 & color)
{
    if (m_indexCount >= (MAX_QUADS * 6)) {
        flush();
    }

    m_vertexBufferPtr->m_position = tr;
    m_vertexBufferPtr->m_color = color;
    m_vertexBufferPtr->m_textureId = 0;
    m_vertexBufferPtr->m_uv = glm::vec2(1.0f, 1.0f);
    m_vertexBufferPtr++;

    m_vertexBufferPtr->m_position = br;
    m_vertexBufferPtr->m_color = color;
    m_vertexBufferPtr->m_textureId = 0;
    m_vertexBufferPtr->m_uv = glm::vec2(1.0f, 0.0f);
    m_vertexBufferPtr++;

    m_vertexBufferPtr->m_position = bl;
    m_vertexBufferPtr->m_color = color;
    m_vertexBufferPtr->m_textureId = 0;
    m_vertexBufferPtr->m_uv = glm::vec2(0.0f, 0.0f);
    m_vertexBufferPtr++;

    m_vertexBufferPtr->m_position = tl;
    m_vertexBufferPtr->m_color = color;
    m_vertexBufferPtr->m_textureId = 0;
    m_vertexBufferPtr->m_uv = glm::vec2(0.0f, 1.0f);
    m_vertexBufferPtr++;

    m_indexCount += 6;
}

void QuadBatchRenderState::push_sprite(const glm::mat4 & transform, Sprite * p_sprite)
{
    if (m_indexCount >= (MAX_QUADS * 6)) {
        flush();
    }

    int tex_idx = -1;

    for (int i = 1; i < 8; i++){
        if (m_textureSlots[i] == p_sprite->get_id()) {
            tex_idx = i;
            break;
        }
    }

    if (tex_idx == -1) {
        m_textureSlots[m_textureSlotIdx] = p_sprite->get_id();
        tex_idx = m_textureSlotIdx;
        m_textureSlotIdx++;
    }

    STELLA_ASSERT((m_textureSlotIdx < 8), "TOO MANY TEXTURES!");

    m_vertexBufferPtr->m_position = transform * glm::vec4(0.5f, 0.5f, 0.0f, 1.0f); // tr
    m_vertexBufferPtr->m_color = glm::vec4(1.0f);
    m_vertexBufferPtr->m_textureId = tex_idx;
    m_vertexBufferPtr->m_uv = glm::vec2(1.0f, 1.0f);
    m_vertexBufferPtr++;

    m_vertexBufferPtr->m_position = transform * glm::vec4(0.5f, -0.5f, 0.0f, 1.0f); // br
    m_vertexBufferPtr->m_color = glm::vec4(1.0f);
    m_vertexBufferPtr->m_textureId = tex_idx;
    m_vertexBufferPtr->m_uv = glm::vec2(1.0f, 0.0f);
    m_vertexBufferPtr++;

    m_vertexBufferPtr->m_position = transform * glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f); // bl
    m_vertexBufferPtr->m_color = glm::vec4(1.0f);
    m_vertexBufferPtr->m_textureId = tex_idx;
    m_vertexBufferPtr->m_uv = glm::vec2(0.0f, 0.0f);
    m_vertexBufferPtr++;

    m_vertexBufferPtr->m_position = transform * glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f); // tl
    m_vertexBufferPtr->m_color = glm::vec4(1.0f);
    m_vertexBufferPtr->m_textureId = tex_idx;
    m_vertexBufferPtr->m_uv = glm::vec2(0.0f, 1.0f);
    m_vertexBufferPtr++;

    m_indexCount += 6;
}

void QuadBatchRenderState::flush()
{
    if (m_pShader != nullptr) {
        if (m_indexCount > 0) {

            int tex_units[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };

            // Bind used textures to texture slots
            for (int i = 1; i < m_textureSlotIdx; i++)
            {
                glActiveTexture(GL_TEXTURE0 + i);
                glBindTexture(GL_TEXTURE_2D, m_textureSlots[i]);
            }

            //Shader * shader = EngineState::get().m_pShaderManager->get(STELLA_QUAD_RENDERER_SHADER_NAME);
            m_pShader->use();
            m_pShader->set_mat4(STELLA_UNIFORM_NAME_PROJECTION_MATRIX, m_pCamera->get_projection_matrix());
            m_pShader->set_mat4(STELLA_UNIFORM_NAME_VIEW_MATRIX, m_pCamera->get_view_matrix());
            m_pShader->set_int_array("u_Textures", m_textureSlotIdx, (const int *)tex_units);

            uint32_t data_size = (uint32_t)((uint8_t *)m_vertexBufferPtr - (uint8_t *)m_vertexBufferBase);

            glBindVertexArray(m_vao);
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, data_size, m_vertexBufferBase);
            glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
        }

        m_indexCount = 0;
        m_vertexBufferPtr = m_vertexBufferBase;
    }
}

QuadRenderer::QuadRenderer() :
    m_pCamera(nullptr), m_indexCount(0)
{
 
}

QuadRenderer::~QuadRenderer()
{

}

void QuadRenderer::start(Camera * p_cam)
{
    m_defaultRenderState.m_pShader = EngineState::get().m_pShaderManager->get(STELLA_QUAD_RENDERER_SHADER_NAME);
    m_defaultRenderState.start(p_cam);
}

void QuadRenderer::end()
{
    m_defaultRenderState.end();
}

void QuadRenderer::push_quad(const glm::vec3 & tr, const glm::vec3 & br, const glm::vec3 & bl, const glm::vec3 & tl, const glm::vec4 & color)
{
    m_defaultRenderState.push_quad(tr, br, bl, tl, color);
}

void QuadRenderer::push_sprite(const glm::mat4 & transform, Sprite * p_sprite)
{
    m_defaultRenderState.push_sprite(transform, p_sprite);
}
    
} // namespace Stella
