#include "Scene.h"
#include "Stella/EngineState.h"
#include "Stella/Math/Ray2D/Ray2D.h"
#include "Stella/Renderer/MeshRenderer/MeshRenderer.h"
#include "Stella/Renderer/SpriteRenderer/SpriteRenderer.h"
#include "Stella/Renderer/QuadRenderer/QuadRenderer.h"
#include "Stella/Renderer/Camera/Camera.h"
#include "Stella/Core/Logger/Logger.h"
#include "Stella/Resources/ShaderManager/ShaderManager.h"
#include "Stella/Systems/Component/Component.h"
#include "Stella/Systems/ModelManager/ModelManager.h"
#include "Stella/Systems/SpriteManager/SpriteManager.h"
#include "Stella/Debug/Print/Print.h"

namespace Stella
{

Scene::Scene(const std::string &name) :
    m_name(name)
{
    m_pb2World = new b2World({ 0.0f, -10.0f });
}

Scene::~Scene()
{
    auto view = this->view<NativeScriptComponent>();
    for (auto [entity, script] : view.each()) 
    {
        if (script.m_pInstance)
        {
            script.m_pInstance->destroy();
            script.destroy_script(&script);
        }
    }

    // NOTE: Box 2D cleans all resources allocated here
    delete m_pb2World;
}

void Scene::on_render()
{
    Camera * p_mainCamera = nullptr;

    // Get main scene camera //
    {
        auto view = this->view<CameraComponent>();

        for (auto [entity, cam_comp] : view.each()) {
            p_mainCamera = cam_comp.m_pCamera;
            break;
        }
    }

    if (p_mainCamera == nullptr) {
        STELLA_ENGINE_LOG("Scene has no camera!");
    }

    // Render Sprites //
    if (p_mainCamera != nullptr) {
        EngineState::get().m_pDebugQuadRenderer->start(p_mainCamera);

        {
            auto view = this->view<SpriteComponent, TransformComponent>();
            for (auto [entity, sc, tc] : view.each()) {
                //EngineState::get().m_pSpriteRenderer->render_sprite(
                //    tc.m_transform.get_model_matrix(), 
                //    EngineState::get().m_pSpriteManager->get_sprite(sc.m_spriteId)
                //);

                EngineState::get().m_pDebugQuadRenderer->push_sprite(
                    tc.m_transform.get_model_matrix(), 
                    EngineState::get().m_pSpriteManager->get_sprite(sc.m_spriteId)
                );
            }
        }

        EngineState::get().m_pDebugQuadRenderer->end();
    }

    // Render Meshes //
    if (p_mainCamera != nullptr) {
        EngineState::get().m_pMeshRenderer->start(p_mainCamera);

        {
            auto view = this->view<MeshComponent, TransformComponent>();
            for (auto [entity, mc, tc] : view.each()) {
                EngineState::get().m_pMeshRenderer->render_mesh(
                    EngineState::get().m_pModelManager->get_model(mc.m_modelId),
                    EngineState::get().m_pShaderManager->get(mc.m_shaderId), 
                    tc.m_transform.get_model_matrix()
                );
            }
        }

        EngineState::get().m_pMeshRenderer->end();
    }
}

void Scene::on_update(const DeltaTime & dt)
{
    // Initialize Physics Bodies //
    static bool first = true;
    if (first) {

        {
            auto view = this->view<RigidBody2DComponent, TransformComponent>();
            for (auto [entity, rbc, tc] : view.each()) {

                b2BodyDef body_def;
                body_def.type  = RigidBody2DComponent::to_box2d_type(rbc.m_type);
                body_def.angle = tc.m_transform.m_rotation.z;
                body_def.position.Set(tc.m_transform.m_translation.x, tc.m_transform.m_translation.y);
                body_def.userData.pointer = (uintptr_t)((uint32_t)entity);
                
                b2Body * body = m_pb2World->CreateBody(&body_def); 

                b2PolygonShape shape;

                // NOTE: Dims of base sprite quad is 1x1 so scale represents actual dims
                shape.SetAsBox(tc.m_transform.m_scale.x / 2.0f, tc.m_transform.m_scale.y / 2.0f); //half width, half height

                if (rbc.m_type == RigidBody2DComponent::Type::DYNAMIC) {
                    b2FixtureDef fixture_def;
                    fixture_def.shape    = &shape;
                    fixture_def.density  = 1.0f;
                    fixture_def.friction = 0.3f;

                    body->CreateFixture(&fixture_def);
                }
                else {
                    body->CreateFixture(&shape, 0.0f);
                }

                rbc.m_pBody = body;
            }
        }

        first = false;
    }

    // Update Scripts //
    {
        auto view = this->view<NativeScriptComponent>();
        for (auto [entity, script] : view.each()) 
        {
            if (!script.m_pInstance)
            {
                script.m_pInstance = script.instantiate_script(script.m_entity);
                script.m_pInstance->initialize();
            }
            script.m_pInstance->on_update();
        }
    }

    // Update Physics //
    {
        auto view = this->view<RigidBody2DComponent, TransformComponent>();
        // TODO Think about this....
        for (auto [entity, rbc, tc] : view.each()) {
            rbc.m_pBody->SetTransform(
                { tc.m_transform.m_translation.x, tc.m_transform.m_translation.y },
                tc.m_transform.m_rotation.z
            );
        }

        // Run sim
        int32_t velocity_iterations = 6;
        int32_t position_iterations = 2;
        m_pb2World->Step(1.0f / 60.0f, velocity_iterations, position_iterations);
        
        for (auto [entity, rbc, tc] : view.each()) {
            //STELLA_ENGINE_LOG("Entity %d Pos: (%f, %f), Angle: (%f)", 
            //            (uint32_t)entity, rbc.m_pBody->GetPosition().x, rbc.m_pBody->GetPosition().y, rbc.m_pBody->GetAngle());

            b2Vec2 pos = rbc.m_pBody->GetPosition();
            tc.m_transform.m_translation.x = pos.x;
            tc.m_transform.m_translation.y = pos.y;
            tc.m_transform.m_rotation.z = (rbc.m_pBody->GetAngle());
        }
    }
}
    

Entity Scene::create_entity(const std::string &name)
{
    entt::entity id = this->create();

    Entity entity(id, this);

    return entity;
}

Entity Scene::get_entity(uint32_t entity_id)
{
    return { (entt::entity)entity_id, this };
}

void Scene::destroy_entity(Entity entity)
{
    this->destroy(entity);
}


class RaysCastCallback : public b2RayCastCallback
{
public:
    RaysCastCallback() : 
        m_fixture(nullptr) {}
    
    float ReportFixture(b2Fixture * fixture, const b2Vec2 & point, const b2Vec2 & normal, float fraction) {        
        m_fixture  = fixture; 
        m_point    = point;        
        m_normal   = normal;        
        m_fraction = fraction;
        m_entityID = (uint32_t)fixture->GetBody()->GetUserData().pointer;        
        return fraction;     
    }    

    b2Fixture * m_fixture;    
    b2Vec2 m_point;    
    b2Vec2 m_normal;    
    float m_fraction;
    uint32_t m_entityID;

};

bool Scene::raycast2d(RayCast2DHit & rc_out, const Ray2D & ray)
{
    RaysCastCallback callback;
    glm::vec2 end = ray.m_origin + ray.m_dir * ray.m_length;
    m_pb2World->RayCast(&callback, { ray.m_origin.x, ray.m_origin.y }, { end.x, end.y });

    if (callback.m_fixture) { 
        rc_out.m_point  = glm::vec2(callback.m_point.x, callback.m_point.y);
        rc_out.m_normal = glm::vec2(callback.m_normal.x, callback.m_normal.y);
        rc_out.m_entity = callback.m_entityID;
        return true;
    }
    else {
        return false;
    }
}
    
} // namespace Stella

