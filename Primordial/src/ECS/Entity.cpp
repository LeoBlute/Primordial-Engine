#include "pch.h"
#include "Entity.hpp"
#include "ECS/Components/CPhysicsBody.hpp"
#include "ECS/Components/CRenderer.hpp"
#include "Components/Component.hpp"
#include "VUtils/DebugUtils.hpp"

namespace ECS
{
    void Entity::_PrintComponentWarning(const char* str) const
    {
        DEBUG_FILE_WARN(str);
    }
    void Entity::TargetUpdate()
    {
        //A copy is made because if a new component is added it should not be update
        //until the next update call
        const std::vector<Component*> attached_components = mAttachedComponents;
        for (Component* c : attached_components)
        {
            assert(c != NULL && c->mAssignedEntity == this);
            if (c->IsEnable())
                c->OnTargetUpdate();
        }
        OnTargetUpdate();
    }
    void Entity::TickUpdate()
    {
        //A copy is made because if a new component is added it should not be update
        //until the next update call
        const std::vector<Component*> attached_components = mAttachedComponents;
        for (Component* c : mAttachedComponents)
        {
            assert(c != NULL && c->mAssignedEntity == this);
            if (c->IsEnable())
                c->OnTickUpdate();
        }
        OnTickUpdate();
    }
    void Entity::RemovePhysicsBody()
    {
        Scene::mRegistry.remove<CPhysicsBody>(mID);
    }
    void Entity::RemoveCRenderer()
    {
        Scene::mRegistry.remove<CRenderer>(mID);
    }
}