#pragma once

#pragma region Fake class definitions

class b2Body;

class CTransform;

namespace ECS {
	class Entity;
}

#pragma endregion

class CCollision
{
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
		float restitutionThreshold = 0.5f;

		//The friction coefficient, usually in the range [0,1].
		float friction = 0.2f;
	};
public:
	//#Deleted construction
	CCollision(const CCollision&) = delete;
	CCollision() = delete;

	//#Life cycle
	CCollision(ECS::Entity* assignedEntity, const Stats& stats);
	~CCollision();
	void PreStep();
	void PostStep();

	//#Other
	void ApplyImpulse(const glm::vec2& impulse);
	void ApplyForce(const glm::vec2& force);

	//#Getters and setters
	inline const Stats& GetStats() { return mStats; };
	inline const Type GetType() { return mStats.type; };
	inline const bool GetIsFixedRotation() { return mStats.fixedRotation; };
	inline const bool GetIsTrigger() { return mStats.isTrigger; };
	inline const float GetGravity() { return mStats.gravity; };
	inline const float GetDensity() { return mStats.density; };
	inline const float GetRestitution() { return mStats.restitution; };
	inline const float GetRestitutionThreshold() { return mStats.restitutionThreshold; };
	inline const float GetFriction() { return mStats.friction; };
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
private:
	void UpdateBodyStats();
private:
	CTransform* mAssignedTransform;
	b2Body* mBody;
	Stats mStats;
};