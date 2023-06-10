#pragma once

class Camera
{
private:
	glm::vec3 mPosition;
	float mYaw;
	float mPitch;

public:
	Camera(const glm::vec3& position);
	~Camera();

	glm::mat4 GetViewMatrix();

	inline const glm::vec3& GetPosition() { return mPosition; };
	inline void SetPosition(const glm::vec3& position) { mPosition = position; };
	inline const float GetYaw() { return mYaw; };
	inline void SetYaw(const float yaw) { mYaw = yaw; };
	inline const float GetPitch() { return mPitch; };
	inline void SetPitch(const float pitch) { mPitch = pitch; };
	
	glm::vec3 Forward();
	glm::vec3 Right();
	glm::vec3 Up();
};