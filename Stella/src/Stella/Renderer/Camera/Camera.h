#pragma once

#include <glm/glm.hpp>

namespace Stella
{

class Camera
{
public:

    //Camera(float fov, float aspect, float near, float far, glm::vec3 position, glm::vec3 front);
    Camera(float near, float far, float ar, glm::vec3 position, glm::vec3 front);
    virtual ~Camera() = default;

    glm::mat4 get_view_matrix() const;
    glm::mat4 get_projection_matrix() const;
    glm::mat4 get_view_projection_matrix() const;

    glm::vec3 get_position() const;
    void set_position(glm::vec3 position);

    glm::vec3 get_direction() const;
    void set_direction(glm::vec3 direction);

    glm::vec3 get_up() const;
    void set_up(glm::vec3 up);

    float get_near() const;
    void set_near(float near);

    float get_far() const;
    void set_far(float far);

    float get_aspect_ratio() const;
    void set_aspect_ratio(float ar);

protected:

    virtual void update_projection() = 0;

    glm::mat4 m_projection;
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;

    float m_near;
    float m_far;
    float m_aspect;
};

class PerspectiveCamera : public Camera
{
public:

    PerspectiveCamera(float fov, float aspect, float near, float far, glm::vec3 position, glm::vec3 front);

    float get_fov() const;
    void set_fov(float fov);

private:

    virtual void update_projection() override;    

    float m_fov;
};

class OrthographicCamera : public Camera
{
public:

    OrthographicCamera(float left, float right, float bottom, float top, float near, float far, float ar, glm::vec3 position, glm::vec3 front);

    float get_left() const;
    void set_left(float l);

    float get_right() const;
    void set_right(float r);

    float get_top() const;
    void set_top(float t);

    float get_bottom() const;
    void set_bottom(float b);

private:
    
    virtual void update_projection() override;    

    float m_left;
    float m_right;
    float m_top;
    float m_bottom;

};


} // namespace Stella
