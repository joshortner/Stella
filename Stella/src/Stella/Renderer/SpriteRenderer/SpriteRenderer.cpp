#include "SpriteRenderer.h"
#include "StellaDefines.h"
#include "Stella/EngineState.h"
#include "Stella/Resources/ShaderManager/ShaderManager.h"
#include "Stella/Renderer/Shader/Shader.h"
#include "Stella/Renderer/Camera/Camera.h"
#include "Stella/Renderer/MeshRenderer/Model.h"
#include "Stella/Renderer/Sprite/Sprite.h"

#include <glad/glad.h>

namespace Stella
{

SpriteRenderer::SpriteRenderer() :
    m_pCamera(nullptr)
{
    m_pBaseQuad = Model::create(Model::PrimitiveType::TRIANGLES);
	m_pBaseQuad->vertices = {
		{ -0.5f,  0.5f, 0.0f },
		{ -0.5f, -0.5f, 0.0f },
		{  0.5f, -0.5f, 0.0f },
		{ -0.5f,  0.5f, 0.0f },
		{  0.5f, -0.5f, 0.0f },
		{  0.5f,  0.5f, 0.0f }
    };
	m_pBaseQuad->uvs = {
		{ 0.0f, 1.0f },
		{ 0.0f, 0.0f },
		{ 1.0f, 0.0f },
		{ 0.0f, 1.0f },
		{ 1.0f, 0.0f },
		{ 1.0f, 1.0f }
	};
	m_pBaseQuad->build();
}

SpriteRenderer::~SpriteRenderer()
{
    Model::destroy(m_pBaseQuad);
}

void SpriteRenderer::start(Camera * camera)
{
    m_pCamera = camera;
}

void SpriteRenderer::render_sprite(const glm::mat4 & transform, Sprite * p_sprite)
{
    Shader * shader = EngineState::get().m_pShaderManager->get(STELLA_BASIC_QUAD_SHADER_NAME);
    shader->use();
    shader->set_mat4(STELLA_UNIFORM_NAME_PROJECTION_MATRIX, m_pCamera->get_projection_matrix());
    shader->set_mat4(STELLA_UNIFORM_NAME_VIEW_MATRIX, m_pCamera->get_view_matrix());
    shader->set_mat4(STELLA_UNIFORM_NAME_MODEL_MATRIX, transform);
    if (p_sprite != nullptr) {
        //shader->set_int("s_Texture", 0);
        p_sprite->bind();
    }
    m_pBaseQuad->bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void SpriteRenderer::end()
{

}

    
} // namespace Stella
