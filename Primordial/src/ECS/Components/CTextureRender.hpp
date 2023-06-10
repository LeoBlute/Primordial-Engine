
namespace Renderer2D
{
	class Texture;
}

class CTextureRender
{
public:
	CTextureRender() = delete;
	CTextureRender(const CTextureRender&) = delete;
	CTextureRender(Renderer2D::Texture* texture, const glm::vec4& color)
		:Texture(texture), Color(color) {};
	~CTextureRender() = default;

	void Draw(const glm::vec2& repetition, const glm::vec2& position, float rotation, const glm::vec2& scale);
public:
	glm::vec4 Color;
	Renderer2D::Texture* Texture;
};