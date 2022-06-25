#include "MeshRenderer.h"
#include "StellaDefines.h"
#include "Model.h"
#include "Stella/Core/Logger/Logger.h"
#include "Stella/Renderer/Camera/Camera.h"
#include "Stella/Renderer/Shader/Shader.h"

#include <glad/glad.h>

namespace Stella
{
    
MeshRenderer::MeshRenderer()
{
    m_renderState.m_pCamera = nullptr;
    m_renderState.m_meshCount = 0;
}

void MeshRenderer::start(Camera * camera)
{
    m_renderState.m_pCamera = camera;
    m_renderState.m_meshCount = 0;
}

void MeshRenderer::render_mesh(Model * model, Shader * shader, const glm::mat4 & transform)
{
    if (m_renderState.m_meshCount == STELLA_CONFIG_MESH_RENDERER_BATCH_SIZE) {
        flush();
    }
    m_renderState.m_meshBatchBuffer[m_renderState.m_meshCount].m_model     = model;
    m_renderState.m_meshBatchBuffer[m_renderState.m_meshCount].m_shader    = shader;
    m_renderState.m_meshBatchBuffer[m_renderState.m_meshCount].m_transform = transform;
    m_renderState.m_meshCount++;
}

void MeshRenderer::end()
{
    flush();
    m_renderState.m_pCamera = nullptr;
}

void MeshRenderer::flush()
{
    for (int i = 0; i < m_renderState.m_meshCount; i++) {
        //STELLA_UNIFORM_NAME_MODEL_MATRIX
        m_renderState.m_meshBatchBuffer[i].m_shader->use();
        m_renderState.m_meshBatchBuffer[i].m_shader->set_mat4(STELLA_UNIFORM_NAME_PROJECTION_MATRIX, m_renderState.m_pCamera->get_projection_matrix());
        m_renderState.m_meshBatchBuffer[i].m_shader->set_mat4(STELLA_UNIFORM_NAME_VIEW_MATRIX, m_renderState.m_pCamera->get_view_matrix());
        m_renderState.m_meshBatchBuffer[i].m_shader->set_mat4(STELLA_UNIFORM_NAME_MODEL_MATRIX, m_renderState.m_meshBatchBuffer[i].m_transform);
        m_renderState.m_meshBatchBuffer[i].m_model->bind();

        if (m_renderState.m_meshBatchBuffer[i].m_model->type == Model::PrimitiveType::TRIANGLES) {
            if (m_renderState.m_meshBatchBuffer[i].m_model->indices.size()) {
                glDrawElements(GL_TRIANGLES, m_renderState.m_meshBatchBuffer[i].m_model->indices.size(), GL_UNSIGNED_INT, 0);
            }
            else {
                glDrawArrays(GL_TRIANGLES, 0, m_renderState.m_meshBatchBuffer[i].m_model->vertices.size());
            }
        }
    }
    m_renderState.m_meshCount = 0;
}


} // namespace Stella
