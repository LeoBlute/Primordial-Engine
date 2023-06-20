
namespace Renderer2D {

	class Texture
	{
	private:
		Texture(const Texture&) = delete;
		Texture() = delete;
	public:
		Texture(const char* path);
		~Texture();
	public:
		void Bind();
	public:
		constexpr inline const unsigned int GetID() const noexcept { return mID; };
		constexpr inline const glm::ivec2& GetSize() const noexcept { return mSize; };
	private:
		unsigned int mID;
		glm::ivec2 mSize;
	};
}