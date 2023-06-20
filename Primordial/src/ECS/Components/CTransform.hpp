#pragma once

#include "glm/vec2.hpp"

class CTransform
{
public:
	CTransform(const glm::vec2& pPos, const glm::vec2& pScale, float pRotation)
		: position(pPos), scale(pScale), rotation(pRotation) {};
public:
	glm::vec2 position;
	glm::vec2 scale;
	float rotation;
};