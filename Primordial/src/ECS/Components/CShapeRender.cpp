#include "glm/vec4.hpp"
#include "glm/vec2.hpp"
#include "CShapeRender.hpp"
#include "Rendering/Renderer2D.hpp"

void CShapeRender::Draw(const glm::vec2& position, float rotation, const glm::vec2& scale)
{
	Renderer2D::DrawShape(Shape, position, scale, rotation, Color);
}