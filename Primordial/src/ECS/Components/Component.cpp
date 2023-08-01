#include "Component.hpp"

#include "ECS/Entity.hpp"

CTransform* Component::GetAssignedTransform() const
{
	return mAssignedEntity->transform;
}

CIdentity* Component::GetAssignedIdentity() const
{
	return mAssignedEntity->identity;
}
