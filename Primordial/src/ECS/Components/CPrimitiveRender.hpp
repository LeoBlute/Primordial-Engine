
namespace Renderer2D {
	enum Shape;
}

class CPrimitiveRender
{
public:
	CPrimitiveRender(Renderer2D::Shape type, const glm::vec4& color);
	CPrimitiveRender(const CPrimitiveRender&) = delete;
	~CPrimitiveRender() = default;
public:
	void Draw(const glm::vec2& position, float rotation, const glm::vec2& scale);
public:
	Renderer2D::Shape Type;
	glm::vec4 Color;
};