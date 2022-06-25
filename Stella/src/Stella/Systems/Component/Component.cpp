#include "Component.h"
#include "Stella/Core/Assert/Assert.h"

namespace Stella
{
    

RigidBody2DComponent::RigidBody2DComponent(RigidBody2DComponent::Type type) :
    m_type(type) { }


b2BodyType RigidBody2DComponent::to_box2d_type(RigidBody2DComponent::Type type)
{
    switch (type)
    {
        case RigidBody2DComponent::Type::STATIC:  return b2_staticBody;
        case RigidBody2DComponent::Type::DYNAMIC: return b2_dynamicBody;
    }

    STELLA_ASSERT((0), "Rigid body type not supported: %d", type);
    return b2_staticBody;
}

} // namespace Stella
