#include "box2d/box2d.h"
#include "PMath.h"
#include "VUtils/DebugUtils.hpp"
#include "CCollision.hpp"
#include "ECS/Entity.hpp"
#include "ECS/Components/CTransform.hpp"

CCollision::CCollision(ECS::Entity* assignedEntity, const Stats& stats)
{
	mAssignedTransform = assignedEntity->GetComponent<CTransform>();
	mStats = stats;

	b2BodyDef bodyDef;
	bodyDef.type = static_cast<b2BodyType>(mStats.type);
	bodyDef.gravityScale = mStats.gravity;
	bodyDef.fixedRotation = mStats.fixedRotation;
	bodyDef.linearDamping = mStats.linearDamping;
	bodyDef.angularDamping = mStats.angularDamping;
	const glm::vec2 pos = mAssignedTransform->position;
	bodyDef.position.Set(pos.x, pos.y);
	bodyDef.angle = glm::radians(mAssignedTransform->rotation);
	mBody = ECS::Scene::Physics::GetWorld()->CreateBody(&bodyDef);

	b2PolygonShape dynamicBox;
	const glm::vec2 size = mAssignedTransform->scale;
	constexpr float v_mult = 0.5f;
	dynamicBox.SetAsBox(size.x * v_mult, size.y * v_mult);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = mStats.density;
	fixtureDef.restitution = mStats.restitution;
	fixtureDef.restitutionThreshold = mStats.restitutionThreshold;
	fixtureDef.friction = mStats.friction;
	fixtureDef.isSensor = mStats.isTrigger;

	mBody->CreateFixture(&fixtureDef);
}

CCollision::~CCollision()
{
	ECS::Scene::Physics::GetWorld()->DestroyBody(mBody);
}

void CCollision::PreStep()
{
	const glm::vec2 pos = mAssignedTransform->position;
	const glm::vec2 size = mAssignedTransform->scale;
	const b2Vec2 position = b2Vec2(pos.x, pos.y);
	const float angle = glm::radians(mAssignedTransform->rotation);

	constexpr float v_mult = 0.5f;

	b2Shape* shape = mBody->GetFixtureList()->GetShape();
	static_cast<b2PolygonShape*>(shape)->SetAsBox(size.x * v_mult, size.y * v_mult);
	mBody->SetTransform(position, angle);
}

void CCollision::PostStep()
{
	 const b2Vec2 pos = mBody->GetPosition();
	 const glm::vec2 position = glm::vec2(pos.x, pos.y);
	 const float angle = glm::degrees(mBody->GetAngle());
	 mAssignedTransform->position = position;
	 mAssignedTransform->rotation = angle;
}

void CCollision::ApplyImpulse(const glm::vec2& impulse)
{
	constexpr float _mult = 1.0f;
	const b2Vec2 _impulse(impulse.x * _mult, impulse.y * _mult);
	mBody->ApplyLinearImpulseToCenter(_impulse, false);
}

void CCollision::ApplyForce(const glm::vec2& force)
{
	constexpr float _mult = 50.0f;
	const b2Vec2 _force(force.x * _mult, force.y * _mult);
	mBody->ApplyForceToCenter(_force, false);
}

void CCollision::UpdateBodyStats()
{
	b2Fixture* fixt = mBody->GetFixtureList();

	fixt->SetDensity(mStats.density);
	fixt->SetFriction(mStats.friction);
	mBody->SetGravityScale(mStats.gravity);
	mBody->SetFixedRotation(mStats.fixedRotation);
	fixt->SetSensor(mStats.isTrigger);
	fixt->SetRestitution(mStats.restitution);
	fixt->SetRestitutionThreshold(mStats.restitutionThreshold);
	mBody->SetType(static_cast<b2BodyType>(mStats.type));

	mBody->ResetMassData();
}