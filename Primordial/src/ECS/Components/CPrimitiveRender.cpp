#include "glm/vec4.hpp"
#include "glm/vec2.hpp"
#include "CPrimitiveRender.hpp"
#include "Rendering/Renderer2D.hpp"

CPrimitiveRender::CPrimitiveRender(Renderer2D::Primitive type, const glm::vec4& color)
	:Type(type), Color(color)
{
}

void CPrimitiveRender::Draw(const glm::vec2& position, float rotation, const glm::vec2& scale)
{
	Renderer2D::DrawPrimitive(Type, position, scale, rotation, Color);
}