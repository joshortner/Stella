#include "Model.h"
#include "Stella/EngineState.h"
#include "Stella/Core/Logger/Logger.h"
#include "Stella/Core/Assert/Assert.h"

#include <glad/glad.h>

namespace Stella
{

Model::Model() :
    id(STELLA_INVALID_MESH_ID)
{

}

Model * Model::create(PrimitiveType type)
{
    static int s_next_mesh_id = 0;

    uint32_t vao, vbo, ibo, uvbo, nbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &uvbo);
    glGenBuffers(1, &ibo);
    glGenBuffers(1, &nbo);

    Model * model = new Model();
    model->type             = type;
    model->id               = s_next_mesh_id;
    model->vertex_array_id  = vao;
    model->vertex_buffer_id = vbo;
    model->index_buffer_id  = ibo;
    model->uv_buffer_id     = uvbo;
    model->normal_buffer_id = nbo;

    s_next_mesh_id++;
    return model;
}

void Model::destroy(Model * model)
{
    glDeleteVertexArrays(1, &model->vertex_array_id);
    glDeleteBuffers(1, &model->vertex_buffer_id);
    glDeleteBuffers(1, &model->index_buffer_id);
    glDeleteBuffers(1, &model->normal_buffer_id);
    model->id = STELLA_INVALID_MESH_ID;
    delete model;
}

void Model::build()
{
    STELLA_ASSERT((id != STELLA_INVALID_MESH_ID), "Cannot build an invalid model");
    glBindVertexArray(vertex_array_id);

    if (vertices.size() != 0) {
        // Verticees
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // Vertices
        glEnableVertexAttribArray(0);

        if (uvs.size() != 0) {
            glBindBuffer(GL_ARRAY_BUFFER, uv_buffer_id);
            glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), uvs.data(), GL_DYNAMIC_DRAW);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0); // UVs
            glEnableVertexAttribArray(1);
        }

        if (normals.size() != 0) {
            glBindBuffer(GL_ARRAY_BUFFER, normal_buffer_id);
            glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_DYNAMIC_DRAW);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // normals
            glEnableVertexAttribArray(2);
        }

        if (indices.size() != 0) {
            // Indices
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_DYNAMIC_DRAW);
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0); 

        glBindVertexArray(0); 
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    else {
        STELLA_ENGINE_LOG("Model built with no vertices!");
    }
}

void Model::bind()
{
    glBindVertexArray(vertex_array_id); 
}

Model * Model::quad()
{
    Model * quad_model = Model::create(Model::PrimitiveType::TRIANGLES);
    quad_model->vertices = {
        {  1.0f,  1.0f, 0.0f }, 
        {  1.0f, -1.0f, 0.0f }, 
        { -1.0f, -1.0f, 0.0f }, 
        { -1.0f,  1.0f, 0.0f }   
    };
    quad_model->indices = {  
        0, 1, 3,  
        1, 2, 3   
    };
    quad_model->build();
    return quad_model;
}

Model * Model::cube()
{
    Model * quad_model = Model::create(Model::PrimitiveType::TRIANGLES);
    quad_model->vertices = {
        { -1.0f, -1.0f, -1.0f },
        {  1.0f, -1.0f, -1.0f },
        {  1.0f,  1.0f, -1.0f },
        {  1.0f,  1.0f, -1.0f },
        { -1.0f,  1.0f, -1.0f },
        { -1.0f, -1.0f, -1.0f },
        { -1.0f, -1.0f,  1.0f },
        {  1.0f, -1.0f,  1.0f },
        {  1.0f,  1.0f,  1.0f },
        {  1.0f,  1.0f,  1.0f },
        { -1.0f,  1.0f,  1.0f },
        { -1.0f, -1.0f,  1.0f },
        { -1.0f,  1.0f,  1.0f },
        { -1.0f,  1.0f, -1.0f },
        { -1.0f, -1.0f, -1.0f },
        { -1.0f, -1.0f, -1.0f },
        { -1.0f, -1.0f,  1.0f },
        { -1.0f,  1.0f,  1.0f },
        {  1.0f,  1.0f,  1.0f },
        {  1.0f,  1.0f, -1.0f },
        {  1.0f, -1.0f, -1.0f },
        {  1.0f, -1.0f, -1.0f },
        {  1.0f, -1.0f,  1.0f },
        {  1.0f,  1.0f,  1.0f },
        { -1.0f, -1.0f, -1.0f },
        {  1.0f, -1.0f, -1.0f },
        {  1.0f, -1.0f,  1.0f },
        {  1.0f, -1.0f,  1.0f },
        { -1.0f, -1.0f,  1.0f },
        { -1.0f, -1.0f, -1.0f },
        { -1.0f,  1.0f, -1.0f },
        {  1.0f,  1.0f, -1.0f },
        {  1.0f,  1.0f,  1.0f },
        {  1.0f,  1.0f,  1.0f },
        { -1.0f,  1.0f,  1.0f },
        { -1.0f,  1.0f, -1.0f }
    };

    quad_model->uvs = {
        { 0.0f, 0.0f },
        { 1.0f, 0.0f },
        { 1.0f, 1.0f },
        { 1.0f, 1.0f },
        { 0.0f, 1.0f },
        { 0.0f, 0.0f },
        { 0.0f, 0.0f },
        { 1.0f, 0.0f },
        { 1.0f, 1.0f },
        { 1.0f, 1.0f },
        { 0.0f, 1.0f },
        { 0.0f, 0.0f },
        { 1.0f, 0.0f },
        { 1.0f, 1.0f },
        { 0.0f, 1.0f },
        { 0.0f, 1.0f },
        { 0.0f, 0.0f },
        { 1.0f, 0.0f },
        { 1.0f, 0.0f },
        { 1.0f, 1.0f },
        { 0.0f, 1.0f },
        { 0.0f, 1.0f },
        { 0.0f, 0.0f },
        { 1.0f, 0.0f },
        { 0.0f, 1.0f },
        { 1.0f, 1.0f },
        { 1.0f, 0.0f },
        { 1.0f, 0.0f },
        { 0.0f, 0.0f },
        { 0.0f, 1.0f },
        { 0.0f, 1.0f },
        { 1.0f, 1.0f },
        { 1.0f, 0.0f },
        { 1.0f, 0.0f },
        { 0.0f, 0.0f },
        { 0.0f, 1.0f }
    };

    quad_model->build();
    return quad_model;
}
    
} // namespace Stella
