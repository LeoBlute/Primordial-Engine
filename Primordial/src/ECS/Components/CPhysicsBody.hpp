#pragma once

#pragma region Fake class definitions

class CTransform;

namespace ECS {
	class Entity;
	namespace Scene {
		namespace Physics {
			void Step(const float timestep);
			class ContactListener;
		}
	}
}

#pragma endregion

class CPhysicsBody
{
private:
	friend void ECS::Scene::Physics::Step(const float timeStep);
	friend ECS::Scene::Physics::ContactListener;
public:
	enum Type
	{
		Static = 0,
		Kinematic = 1,
		Dynamic = 2
	};
	struct Stats
	{
		//Static: zero mass, zero velocity, may be manually moved
		//Kinematic: zero mass, non-zero velocity set by user, moved by solver
		//Dynamic: positive mass, non-zero velocity determined by forces, moved by solver
		Type type = Type::Static;

		//Should this body be prevented from rotating? Useful for characters.
		bool fixedRotation = false;

		//If true will collect contact information but never generates a collision response.
		bool isTrigger = false;

		/// Linear damping is use to reduce the linear velocity. The damping parameter
		// can be larger than 1.0f but the damping effect becomes sensitive to the
		// time step when the damping parameter is large.
		float linearDamping = 0.0f;

		/// Angular damping is use to reduce the angular velocity. The damping parameter
		/// can be larger than 1.0f but the damping effect becomes sensitive to the
		/// time step when the damping parameter is large.
		float angularDamping = 0.0f;

		//Scale the gravity applied to the body.
		float gravity = 1.0f;

		//The density, usually in kg/m^2.
		float density = 1.0f;

		//The restitution (elasticity) usually in the range [0,1].
		float restitution = 0.0f;

		//Restitution velocity threshold, usually in m/s. Collisions above this
		//speed have restitution applied (will bounce).
		float restitutionThreshold = 1.0f;

		//The friction coefficient, usually in the range [0,1].
		float friction = 0.2f;
	};
public:
	#pragma region Deleted construction
	CPhysicsBody(const CPhysicsBody&) = delete;
	CPhysicsBody() = delete;
	#pragma endregion

	#pragma region Life cycle
	CPhysicsBody(ECS::Entity* assignedEntity, const Stats& stats);
	~CPhysicsBody();
	#pragma endregion

	#pragma region Velocity Related
	void ApplyLinearImpulseAt(const glm::vec2& impulse, const glm::vec2& position);
	void ApplyLinearImpulse(const glm::vec2& impulse);
	void ApplyForceAt(const glm::vec2& force, const glm::vec2& position);
	void ApplyForce(const glm::vec2& force);
	void ApplyAngularImpulse(const float impulse);
	void ApplyTorque(const float torque);
	const glm::vec2 GetLinearVelocity();
	const float GetAngularVelocity();
	void SetVelocity(const glm::vec2& linear, const float angular);
	void SetLinearVelocity(const glm::vec2& velocity);
	void SetAngularVelocity(const float velocity);
#pragma endregion

	//Get the CPhysicsBody* associated with the given b2Body
	static CPhysicsBody* GetFromBody(void* body);

	#pragma region Getters and setters
	inline const Stats& GetStats() const { return mStats; };
	inline const Type GetType() const { return mStats.type; };
	inline const bool GetIsFixedRotation() const { return mStats.fixedRotation; };
	inline const bool GetIsTrigger() const { return mStats.isTrigger; };
	inline const float GetGravity() const { return mStats.gravity; };
	inline const float GetDensity() const { return mStats.density; };
	inline const float GetRestitution() const { return mStats.restitution; };
	inline const float GetRestitutionThreshold() const { return mStats.restitutionThreshold; };
	inline const float GetFriction() const { return mStats.friction; };
	inline void SetStats(const Stats& stats) {
		mStats = stats;
		UpdateBodyStats();
	};
	inline void SetType(const Type type) {
		mStats.type = type;
		UpdateBodyStats();
	};
	inline void SetIsFixedRotation(const bool isFixed) {
		mStats.fixedRotation = isFixed;
		UpdateBodyStats();
	}
	inline void SetIsTrigger(const bool isTrigger) {
		mStats.isTrigger = isTrigger;
		UpdateBodyStats();
	}
	inline void SetGravity(const float gravity) {
		mStats.gravity = gravity;
		UpdateBodyStats();
	}
	inline void SetDensity(const float density) {
		mStats.density = density;
		UpdateBodyStats();
	}
	inline void SetRestitution(const float restitution) {
		mStats.restitution = restitution;
		UpdateBodyStats();
	}
	inline void SetRestitutionThreshold(const float restitutionThreshold) {
		mStats.restitutionThreshold = restitutionThreshold;
		UpdateBodyStats();
	}
	inline void SetFriction(const float friction) {
		mStats.friction = friction;
		UpdateBodyStats();
	}
	inline ECS::Entity* GetOwner() { return mAssignedEntity; };
	inline CTransform* GetTransform() { return mAssignedTransform; };
	const bool IsCollidingWith(CPhysicsBody* other);
	const bool IsCollidingWith(ECS::Entity* other);
	#pragma endregion
private:
	void UpdateBodyStats();
	void SetCollidingWith(CPhysicsBody* col, const bool is);
private:
	ECS::Entity* mAssignedEntity;
	CTransform* mAssignedTransform;
	void* mBody;
	Stats mStats;
	std::unordered_map<CPhysicsBody*, bool> mCollidings;
};