#pragma once

#include <entt.hpp>
#include <box2d/box2d.h>

#include <string>

namespace Stella
{

class Scene;
class DeltaTime;
class Ray2D;
class RayCast2DHit;

// Entity //

class Entity
{
public:

	Entity() = default;
	Entity(entt::entity id, Scene *scene) :
		m_entityHandle(id), m_pScene(scene) { }

	template<typename T>
	bool has_component();

	template<typename T, typename... Args>
	T& add_component(Args&&... args);

	template<typename T>
	T& get_component();

	template<typename T>
	void remove_component();

	operator bool() const { return m_entityHandle != entt::null; }
	operator entt::entity() const { return m_entityHandle; }

	uint32_t get_id() { return (uint32_t)m_entityHandle; }

private:

	entt::entity m_entityHandle = entt::null;
	Scene * m_pScene = nullptr;
};

// Scene //
class Scene : public entt::registry
{
public:

    Scene(const std::string &name="Scene");
    ~Scene();

	void on_render();
	void on_update(const DeltaTime & dt);

    Entity create_entity(const std::string &name="");
    Entity get_entity(uint32_t entity_id);
    void destroy_entity(Entity entity);

	bool raycast2d(RayCast2DHit & p_rc_out, const Ray2D & ray);

private:

    std::string m_name;

	b2World * m_pb2World;
};

// Entity Implementation //

template<typename T>
bool Entity::has_component()
{
	return m_pScene->all_of<T>(m_entityHandle);
}

template<typename T, typename... Args>
T& Entity::add_component(Args&&... args)
{
	STELLA_ASSERT(!has_component<T>(), "Entity already has component: %s", typeid(T).name());

	return m_pScene->emplace<T>(m_entityHandle,std::forward<Args>(args)...);
}

template<typename T>
T& Entity::get_component()
{
	STELLA_ASSERT(has_component<T>(), "Entity doesn't have component: %s", typeid(T).name());
	return m_pScene->get<T>(m_entityHandle);
}

template<typename T>
void Entity::remove_component()
{
	STELLA_ASSERT(has_component<T>(), "Entity doesn't have component: %s", typeid(T).name());

	m_pScene->remove<T>(m_entityHandle);
}


} // namespace Stella
