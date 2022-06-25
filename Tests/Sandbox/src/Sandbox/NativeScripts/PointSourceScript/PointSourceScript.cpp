#include "PointSourceScript.h"


PointSourceScript::~PointSourceScript() { }

void PointSourceScript::initialize()
{
    glm::vec3 pos = get_component<TransformComponent>().m_transform.m_translation;

    for (int angle = 0; angle < FOV; angle++) {
        m_sourceRays.push_back(Ray2D(pos, Math2D::angle_to_dir(glm::radians((float)angle)), MAX_RAY_DIST));
    }
}

void PointSourceScript::on_update() 
{
    const glm::vec4 debug_ray_color = glm::vec4(0.7f, 0.7f, 0.0f, 0.5f);
    const glm::vec4 debug_ray_hit_color = glm::vec4(0.6f, 0.0f, 0.0f, 1.0f);
    const float debug_ray_width = 0.02f;
    Scene & scene = STELLA_ENGINE_GET_SCENE();

    for (Ray2D & ray : m_sourceRays) {
        //Debug::draw_line(ray.m_origin, ray.m_origin + ray.m_dir * ray.m_length, debug_ray_color, debug_ray_width);
        
        RayCast2DHit hit;
        if (scene.raycast2d(hit, ray)) {
            Debug::draw_line(ray.m_origin, hit.m_point, debug_ray_hit_color, debug_ray_width);
        }
        else {
            Debug::draw_line(ray.m_origin, ray.m_origin + ray.m_dir * ray.m_length, debug_ray_color, debug_ray_width);
        }
    }

}

void PointSourceScript::destroy() 
{

}