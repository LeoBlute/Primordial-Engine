#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

constexpr static inline float finterp(const float a, const float b, const float f) noexcept
{
	return (a * (1.0f - f)) + (b * f);
}