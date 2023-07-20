#include <memory>
#include <unordered_map>
#include "box2d/box2d.h"
#include "PMath.h"
#include "VUtils/DebugUtils.hpp"
#include "CPhysicsBody.hpp"
#include "ECS/Entity.hpp"
#include "ECS/Components/CTransform.hpp"

//Quickly define body in function
#define BDEF b2Body* body = static_cast<b2Body*>(mBody)
#define WDEF b2World* world = static_cast<b2World*>(ECS::Scene::Physics::GetRawWorld());

CPhysicsBody::CPhysicsBody(ECS::Entity* assignedEntity, const Stats& stats)
{
	mAssignedEntity = assignedEntity;
	mAssignedTransform = mAssignedEntity->GetComponent<CTransform>();
	mStats = stats;

	//Body definition
	b2BodyDef bodyDef;
	bodyDef.type = static_cast<b2BodyType>(mStats.type);
	bodyDef.gravityScale = mStats.gravity;
	bodyDef.fixedRotation = mStats.fixedRotation;
	bodyDef.linearDamping = mStats.linearDamping;
	bodyDef.angularDamping = mStats.angularDamping;
	//This sucks,it's worse than that shitty algorithm in ECS
	bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(this);

	//Finished with transformation values
	const glm::vec2 pos = mAssignedTransform->position;
	bodyDef.position.Set(pos.x, pos.y);
	bodyDef.angle = glm::radians(mAssignedTransform->rotation);

	//Internal body creation
	
	WDEF;
	b2Body* body = world->CreateBody(&bodyDef);
	mBody = body;

	//Shape and size definition
	b2PolygonShape dynamicBox;
	const glm::vec2 size = mAssignedTransform->scale;
	constexpr float v_mult = 0.5f;
	dynamicBox.SetAsBox(size.x * v_mult, size.y * v_mult);

	//Fixture defintion
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = mStats.density;
	fixtureDef.restitution = mStats.restitution;
	fixtureDef.restitutionThreshold = mStats.restitutionThreshold;
	fixtureDef.friction = mStats.friction;
	fixtureDef.isSensor = mStats.isTrigger;

	//Fixture creation
	body->CreateFixture(&fixtureDef);
	b2RayCastInput input;
	b2RayCastOutput output;
}

CPhysicsBody::~CPhysicsBody()
{
	BDEF;
	WDEF;
	world->DestroyBody(body);
	mBody = nullptr;
}

void CPhysicsBody::PreStep()
{
	BDEF;
	const glm::vec2 pos = mAssignedTransform->position;
	const glm::vec2 size = mAssignedTransform->scale;
	const b2Vec2 position = b2Vec2(pos.x, pos.y);
	const float angle = glm::radians(mAssignedTransform->rotation);

	constexpr float v_mult = 0.5f;
	b2Shape* shape = body->GetFixtureList()->GetShape();
	static_cast<b2PolygonShape*>(shape)->SetAsBox(size.x * v_mult, size.y * v_mult);
	body->SetTransform(position, angle);
}

void CPhysicsBody::PostStep()
{
	BDEF;
	const b2Vec2 pos = body->GetPosition();
	const glm::vec2 position = glm::vec2(pos.x, pos.y);
	const float angle = glm::degrees(body->GetAngle());
	mAssignedTransform->position = position;
	mAssignedTransform->rotation = angle;
}

void CPhysicsBody::ApplyLinearImpulse(const glm::vec2& impulse)
{
	ApplyLinearImpulseAt(impulse, mAssignedTransform->position);
}

void CPhysicsBody::ApplyForceAt(const glm::vec2& force, const glm::vec2& position)
{
	//Avoid pointless operations
	if (force == glm::vec2(0.0f, 0.0f) || mStats.type != Type::Dynamic)
		return;

	BDEF;

	//This useless value is applied if the body velocity is zero
	//because it will not apply any velocity otherwise
	constexpr float fixingValue = 0.000000001f;
	if (body->GetLinearVelocity() == b2Vec2_zero)
		body->SetLinearVelocity(b2Vec2(fixingValue * force.x, fixingValue * force.y));

	constexpr float _mult = 50.0f;
	const b2Vec2 _force(force.x * _mult, force.y * _mult);
	const b2Vec2 _position(position.x, position.y);
	body->ApplyForce(_force, _position, false);
}

void CPhysicsBody::ApplyForce(const glm::vec2& force)
{
	ApplyForceAt(force, mAssignedTransform->position);
}

void CPhysicsBody::ApplyAngularImpulse(const float impulse)
{
	//Avoid pointless operations
	if (impulse == 0.0f || mStats.type != Type::Dynamic)
		return;

	BDEF;

	//This useless value is applied if the body velocity is zero
	//because it will not apply any velocity otherwise
	constexpr float fixingValue = 0.000000001f;
	if (!body->GetAngularVelocity())
		body->SetAngularVelocity(fixingValue);

	constexpr float _mult = 10.0f;
	body->ApplyAngularImpulse(impulse * _mult, false);
}

void CPhysicsBody::ApplyTorque(const float torque)
{
	//Avoid pointless operations
	if (torque == 0.0f || mStats.type != Type::Dynamic)
		return;

	BDEF;

	//This useless value is applied if the body velocity is zero
	//because it will not apply any velocity otherwise
	constexpr float fixingValue = 0.000000001f;
	if (!body->GetAngularVelocity())
		body->SetAngularVelocity(fixingValue);

	constexpr float _mult = 500.0f;
	body->ApplyTorque(torque * _mult, false);
}

void CPhysicsBody::ApplyLinearImpulseAt(const glm::vec2& impulse, const glm::vec2& position)
{
	//Avoid pointless operations
	if (impulse == glm::vec2(0.0f, 0.0f) || mStats.type != Type::Dynamic)
		return;

	BDEF;

	//This useless value is applied if the body velocity is zero
	//because it will not apply any velocity otherwise
	constexpr float fixingValue = 0.000000001f;
	if (body->GetLinearVelocity() == b2Vec2_zero)
		body->SetLinearVelocity(b2Vec2(fixingValue * impulse.x, fixingValue * impulse.y));

	const b2Vec2 _impulse(impulse.x, impulse.y);
	const b2Vec2 _position(position.x, position.y);
	body->ApplyLinearImpulse(_impulse, _position, false);
}

const glm::vec2 CPhysicsBody::GetLinearVelocity()
{
	BDEF;
	const b2Vec2 velocity(body->GetLinearVelocity());
	return glm::vec2(velocity.x, velocity.y);
}

const float CPhysicsBody::GetAngularVelocity()
{
	BDEF;
	return body->GetAngularVelocity();
}

CPhysicsBody* CPhysicsBody::GetFromBody(void* body)
{
	if (!body)
		return nullptr;
	b2Body* _body = static_cast<b2Body*>(body);

	uintptr_t ptr = _body->GetUserData().pointer;
	return reinterpret_cast<CPhysicsBody*>(ptr);
}

void CPhysicsBody::SetVelocity(const glm::vec2& linear, const float angular)
{
	BDEF;
	body->SetLinearVelocity(b2Vec2(linear.x, linear.y));
	body->SetAngularVelocity(angular);
}

void CPhysicsBody::SetLinearVelocity(const glm::vec2& velocity)
{
	BDEF;
	body->SetLinearVelocity(b2Vec2(velocity.x, velocity.y));
}

void CPhysicsBody::SetAngularVelocity(const float velocity)
{
	BDEF;
	body->SetAngularVelocity(velocity);
}

const bool CPhysicsBody::IsCollidingWith(CPhysicsBody* other)
{
	return mCollidings[other];
}

const bool CPhysicsBody::IsCollidingWith(ECS::Entity* other)
{
	if (!other || !(other->HasComponent<CPhysicsBody>()))
		return false;

	return mCollidings[other->GetComponent<CPhysicsBody>()];
}

void CPhysicsBody::UpdateBodyStats()
{
	b2Body* body = static_cast<b2Body*>(mBody);
	b2Fixture* fixt = body->GetFixtureList();

	fixt->SetDensity(mStats.density);
	fixt->SetFriction(mStats.friction);
	body->SetGravityScale(mStats.gravity);
	body->SetFixedRotation(mStats.fixedRotation);
	fixt->SetSensor(mStats.isTrigger);
	fixt->SetRestitution(mStats.restitution);
	fixt->SetRestitutionThreshold(mStats.restitutionThreshold);
	body->SetType(static_cast<b2BodyType>(mStats.type));

	body->ResetMassData();
}

void CPhysicsBody::SetCollidingWith(CPhysicsBody* col, const bool is)
{
	if (mCollidings[col] != is)
	{
		mCollidings[col] = is;
	}
}