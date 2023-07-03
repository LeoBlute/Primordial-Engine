#pragma once

namespace Renderer2D {
	enum Shape;
}

class CShapeRender
{
public:
	CShapeRender(const Renderer2D::Shape shape, const glm::vec4& color)
		:Shape(shape), Color(color) {}
	CShapeRender(const CShapeRender&) = delete;
	~CShapeRender() = default;
public:
	void Draw(const glm::vec2& position, float rotation, const glm::vec2& scale);
public:
	Renderer2D::Shape Shape;
	glm::vec4 Color;
};