#pragma once

#include "StellaConfig.h"

#include <glm/glm.hpp>

#include <stdint.h>

#include <vector>

namespace Stella
{

class Model;
class Shader;
class Camera;
    
struct Mesh
{
    Model * m_model;
    Shader * m_shader;
    glm::mat4 m_transform;

    Mesh() : m_model(nullptr), m_shader(nullptr) { }
    Mesh(Model * model, Shader * shader, glm::mat4 transform) :
        m_model(model), m_shader(shader), m_transform(transform) { }
};

class MeshRenderer
{

public:

    MeshRenderer();

    void start(Camera * camera);
    void render_mesh(Model * mesh, Shader * shader, const glm::mat4 & transform);
    void end();

private:

    void flush();

private:

    struct MeshRendererState
    {
        Camera * m_pCamera;

        Mesh m_meshBatchBuffer[STELLA_CONFIG_MESH_RENDERER_BATCH_SIZE];
        uint32_t m_meshCount;   
    };

    MeshRendererState m_renderState;
};

} // namespace Stella
