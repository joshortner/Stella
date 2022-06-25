#pragma once

#include "Stella/Systems/Scene/Scene.h"

namespace Stella
{
    
class NativeScriptBase
{
public:

    //NativeScriptBase(Entity ent) : 
    //    m_entity(ent) {}
    virtual ~NativeScriptBase() = default;

    virtual void initialize() = 0;
    virtual void destroy() = 0;
    virtual void on_update() = 0;

protected:

    template<typename T>
	T& get_component()
    {
        return m_entity.get_component<T>();
    }

    uint32_t get_entity_id()
    {
        return m_entity.get_id();
    }

private:

    Entity m_entity;
    friend struct NativeScriptComponent;

};
    
} // namespace Stella
