#pragma once

#include "Stella/Renderer/Transform/Transform.h"
#include "Stella/Systems/Scene/Scene.h"
#include "Stella/Systems/NativeScriptBase/NativeScriptBase.h"

#include <glm/glm.hpp>
#include <box2d/box2d.h> //TMP

#include <string>

#define STELLA_COMPONENT_TYPE(type) #type

namespace Stella
{

class Camera;

// Mesh Component //
struct MeshComponent
{
    std::string m_modelId;
    std::string m_shaderId;

    MeshComponent() = default;
    MeshComponent(std::string model_id, std::string shader_id) :
        m_modelId(model_id), m_shaderId(shader_id) {}
};

// Sprite Compnent //
struct SpriteComponent
{
    std::string m_spriteId;

    SpriteComponent() = default;
    SpriteComponent(std::string sprite_id) : m_spriteId(sprite_id) { }
};

// Native Script Component //  
//@TODO Think about hwo to pass entity to native scripts
struct NativeScriptComponent
{
    NativeScriptBase * m_pInstance;

    NativeScriptBase * (* instantiate_script)(const Entity &);
    void (* destroy_script)(NativeScriptComponent*);

    NativeScriptComponent() :
        m_pInstance(nullptr)
    {}

    NativeScriptComponent(const NativeScriptComponent & obj) = default;

    template<typename T>
    NativeScriptComponent bind(const Entity & e)
    {
        m_entity = e;
        instantiate_script = [](const Entity & ent) { 
            NativeScriptBase * script = (NativeScriptBase *)new T();
            script->m_entity = ent;
            return (NativeScriptBase *)script;
        };
        destroy_script = [](NativeScriptComponent * nsc) { delete nsc->m_pInstance; nsc->m_pInstance = nullptr; };
        return *this;
    }

private:

    Entity m_entity;
    friend class Scene;

};

// Camera Component //
struct CameraComponent
{
    Camera * m_pCamera;

    CameraComponent(Camera * p_cam) :
        m_pCamera(p_cam) { }
};

// Transform Component //
struct TransformComponent
{
    Transform m_transform;

    TransformComponent() :
        m_transform(glm::vec3(0), glm::vec3(0), glm::vec3(1.0f)) { }
};

// Rigid Body 2D //
struct RigidBody2DComponent
{

enum Type
{
    STATIC, DYNAMIC
};

    Type m_type;
    b2Body * m_pBody;

    RigidBody2DComponent(RigidBody2DComponent::Type type);


    static b2BodyType to_box2d_type(RigidBody2DComponent::Type type);
};

} // namespace Stella
