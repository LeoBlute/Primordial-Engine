#pragma once

namespace ECS {
	class Entity;;
}

class CTransform;
class CIdentity;

//Implement this to prorperly define the class has a component
#define COMPONENT_IMPLEMENT(NAME)\
public:\
	NAME(ECS::Entity* entity)\
	: Component(entity) {\
		OnCreated();\
	};\
\
	~NAME()\
	{\
		OnDestroyed();\
	}

//Generic component definition
//*WARNING* use COMPONENT_IMPLEMENT(Componenet name) to prorpely implement component features
class Component
{
private:
	friend ECS::Entity;
public:
	Component(const Component&) = delete;
	Component() = delete;
	virtual ~Component() = default;
	Component(ECS::Entity* assignedEntity)
		:mAssignedEntity(assignedEntity) {};

protected:
	virtual void OnCreated() {};
	virtual void OnDestroyed() {};
	virtual void OnTargetUpdate() {};
	virtual void OnTickUpdate() {};

public:
	constexpr inline ECS::Entity* GetAssignedEntity() const { return mAssignedEntity; };
	CTransform* GetAssignedTransform() const;
	CIdentity* GetAssignedIdentity() const;

	constexpr inline bool IsEnable() const { return mEnable; };
	void SetEnable(const bool enable);
private:
	ECS::Entity* mAssignedEntity;
	bool mEnable = true;
};