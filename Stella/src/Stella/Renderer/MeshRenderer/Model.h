#pragma once

#include <glm/glm.hpp>

#include <stdint.h>

#include <vector>

#define STELLA_INVALID_MESH_ID (-1)

namespace Stella
{

/**
 * @brief NOTE: GPU resources are not allocated for a mesh until build is called.
 *              If data is update, build must be called again to update the data on the GPU.
 *        TODO: Keep cpu data?
 */
struct Model
{

enum PrimitiveType
{
    TRIANGLES,
    QUADS,
    LINES,
    LINE_STRIP,
    POINTS
};

enum BuiltinType
{
    QUAD, CUBE
};

    uint32_t id;
    PrimitiveType type;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
    std::vector<uint32_t> indices;

    Model();
    Model(const Model & obj) = default;

    static Model * create(PrimitiveType type);
    static void destroy(Model * mesh);

    void build();
    void bind();

    static Model * quad();
    static Model * cube();

private:

    uint32_t vertex_buffer_id;
    uint32_t index_buffer_id;
    uint32_t uv_buffer_id;
    uint32_t normal_buffer_id;
    uint32_t vertex_array_id;
    
    friend class MeshRenderer;
};

} // namespace Stella
