#include "Component.hpp"

#include "ECS/Entity.hpp"

void Component::_OCR(Component* c)
{
	c->OnCreated();
}

CTransform* Component::GetAssignedTransform() const
{
	return mAssignedEntity->transform;
}

CIdentity* Component::GetAssignedIdentity() const
{
	return mAssignedEntity->identity;
}

void Component::SetEnable(const bool enable)
{
	mEnable = false;
}
