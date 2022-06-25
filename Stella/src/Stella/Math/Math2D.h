#pragma once

#include <glm/glm.hpp>

namespace Stella
{

namespace Math2D
{
    
glm::vec2 angle_to_dir(float rad);

glm::vec2 lerp(glm::vec2 x, glm::vec2 y, float t);

} // namespace Math2D

} // namespace Stella
