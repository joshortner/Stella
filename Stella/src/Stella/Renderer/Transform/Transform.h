#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Stella
{

struct Transform
{
    glm::vec3 m_translation;
    glm::vec3 m_rotation;
    glm::vec3 m_scale;

    Transform() :
        m_translation(glm::vec3(0)), m_rotation(glm::vec3(0)), m_scale(glm::vec3(1.0f)) { }
    Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale) :
        m_translation(pos), m_rotation(rot), m_scale(scale) { }

    glm::mat4 get_model_matrix() const 
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, m_translation);
        model = glm::rotate(model, (m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, (m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, (m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, m_scale);
        return model;
    }
};
    
} // namespace Stella
