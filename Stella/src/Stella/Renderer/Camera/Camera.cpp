#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Stella
{
// Camera //

Camera::Camera(float near, float far, float ar, glm::vec3 position, glm::vec3 front) :
    m_near(near), m_far(far), m_aspect(ar), m_position(position), m_front(front), m_up(glm::vec3(0.0f, 1.0f, 0.0f))
{

}

glm::mat4 Camera::get_view_matrix() const
{
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

glm::mat4 Camera::get_projection_matrix() const
{
    return m_projection;
}

glm::mat4 Camera::get_view_projection_matrix() const
{
    return m_projection * glm::lookAt(m_position, m_position + m_front, m_up);
}


glm::vec3 Camera::get_position() const
{
    return m_position;
}

void Camera::set_position(glm::vec3 position)
{
    m_position = position;
}

glm::vec3 Camera::get_direction() const
{
    return m_front;
}

void Camera::set_direction(glm::vec3 direction)
{
    m_front = direction;
}

glm::vec3 Camera::get_up() const
{
    return m_up;
}

void Camera::set_up(glm::vec3 up)
{
    m_up = up;
}

float Camera::get_near() const
{
    return m_near;
}

void Camera::set_near(float near)
{
    m_near = near;
    update_projection();
}

float Camera::get_far() const
{
    return m_far;
}

void Camera::set_far(float far)
{
    m_far = far;
    update_projection();
}

float Camera::get_aspect_ratio() const
{
    return m_aspect;
}

void Camera::set_aspect_ratio(float ar)
{
    m_aspect = ar;
    update_projection();
}

// Perspective Camera //

PerspectiveCamera::PerspectiveCamera(float fov, float ar, float near, float far, glm::vec3 position, glm::vec3 front) :
    Camera(near, far, ar, position, front), m_fov(fov)
{ 
    update_projection();
}

float PerspectiveCamera::get_fov() const
{
    return m_fov;
}

void PerspectiveCamera::set_fov(float fov)
{
    m_fov = fov;
    update_projection();
}

void PerspectiveCamera::update_projection() 
{ 
    m_projection = glm::perspective(m_fov, m_aspect, m_near, m_far); 
}

// Orthographic Camera //

OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float near, float far, float ar, glm::vec3 position, glm::vec3 front) :
    Camera(near, far, ar, position, front), m_left(left), m_right(right), m_bottom(bottom), m_top(top)
{
    update_projection();
}


float OrthographicCamera::get_left() const
{
    return m_left;
}
void OrthographicCamera::set_left(float l)
{
    m_left = l;
    update_projection();
}
float OrthographicCamera::get_right() const
{
    return m_right;
}
void OrthographicCamera::set_right(float r)
{
    m_right = r;
    update_projection();
}
float OrthographicCamera::get_top() const
{
    return m_top;
}
void OrthographicCamera::set_top(float t)
{
    m_top = t;
    update_projection();
}
float OrthographicCamera::get_bottom() const
{
    return m_bottom;
}
void OrthographicCamera::set_bottom(float b)
{
    m_bottom = b;
    update_projection();
}
    
void OrthographicCamera::update_projection() 
{
    m_projection = glm::ortho(m_left * m_aspect, m_right * m_aspect, m_bottom, m_top, m_near, m_far);
}

} // namespace Stella
