#pragma once

#include <glm/glm.hpp>

namespace Stella
{

struct Ray2D
{
	glm::vec2 m_origin;
	glm::vec2 m_dir;
	float m_length;

	Ray2D() = default;
	Ray2D(const glm::vec2 & o, const glm::vec2 & dir, float len) :
		m_origin(o), m_dir(glm::normalize(dir)), m_length(len) { }
};

struct RayCast2DHit
{
    glm::vec2 m_point;
    glm::vec2 m_normal;
    uint32_t m_entity;
};
    
} // namespace Stella
