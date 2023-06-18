#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "CTextureRender.hpp"
#include "Rendering/Renderer2D.hpp"

void CTextureRender::Draw(const glm::vec2& repetition, const glm::vec2& position, float rotation, const glm::vec2& scale)
{
	if (!Texture) return;
	Renderer2D::DrawTexture(Shape, Texture, repetition, position, scale, rotation, Color);
}
