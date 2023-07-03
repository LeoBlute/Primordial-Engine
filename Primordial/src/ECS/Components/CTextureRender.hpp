#pragma once

namespace Renderer2D
{
	enum Shape;
	class Texture;
}

class CTextureRender
{
public:
	CTextureRender() = delete;
	CTextureRender(const CTextureRender&) = delete;
	CTextureRender(const Renderer2D::Shape shape, Renderer2D::Texture* texture, const glm::vec4& color)
		:Shape(shape), Texture(texture), Color(color) {};
	~CTextureRender() = default;

	void Draw(const glm::vec2& repetition, const glm::vec2& position, float rotation, const glm::vec2& scale);
public:
	glm::vec4 Color;
	Renderer2D::Texture* Texture;
	Renderer2D::Shape Shape;
};