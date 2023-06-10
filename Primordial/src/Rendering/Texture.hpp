
namespace Renderer2D {

	class Texture
	{
	private:
		Texture(const Texture&) = delete;
		Texture() = delete;
	public:
		Texture(const char* path);
		~Texture();

		//#Settings
	public:
		void Bind();

		//#Getters and setters
	public:
		constexpr inline const unsigned int GetID() const { return mID; };
		constexpr inline const glm::ivec2& GetSize() const { return mSize; };

	private:
		unsigned int mID;
		glm::ivec2 mSize;
	};
}