
namespace ECS {
	class Entity;;
}

class CTransform;
class CIdentity;

//Generic component definition
class Component
{
public:
	Component(const Component&) = delete;
	Component() = delete;
	~Component() = default;
	Component(ECS::Entity* assignedEntity)
		:mAssignedEntity(assignedEntity) {};

	constexpr inline ECS::Entity* GetAssignedEntity() const { return mAssignedEntity; };
	CTransform* GetAssignedTransform() const;
	CIdentity* GetAssignedIdentity() const;
private:
	ECS::Entity* mAssignedEntity;
};