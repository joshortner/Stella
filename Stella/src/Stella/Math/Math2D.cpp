#include "Math2D.h"

namespace Stella
{

namespace Math2D
{

glm::vec2 angle_to_dir(float rad)
{
	return glm::normalize(glm::vec2(cosf(rad), sinf(rad)));
}

glm::vec2 lerp(glm::vec2 x, glm::vec2 y, float t) {
	return x * (1.0f - t) + y * t;
}

} // namespace Math2D

} // namespace Stella