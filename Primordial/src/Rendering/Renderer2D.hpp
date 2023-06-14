
namespace Renderer2D
{
	class Shader;
	class Texture;

	enum Shape
	{
		Quad,
		Triangle
	};

	enum BitMask
	{
		DepthBufferBit = 0x00000100,
		StencilBufferBit = 0x00000400,
		ColorBufferBit = 0x00004000
	};

	//#Life cycle
	void Init(void* procAdress);
	void Terminate();
	void Clear(const glm::vec4& color, const unsigned int mask);
	void DrawShape(Shape shape, const glm::vec2& position, const glm::vec2& scale,
		const float rotation, const glm::vec4& color);
	void DrawTexture(Shape shape, Texture* texture, const glm::vec2& repetition,
		const glm::vec2& position, const glm::vec2& scale, const float rotation, const glm::vec4& color);

	//Checks if object is visible on screen/viewport
	constexpr static inline bool CheckVisibility(const glm::vec2& position, const glm::vec2& scale,
		const float screen_width, const float screen_height)
	{
		const glm::vec2 top_left = position - (scale * 0.5f);
		const glm::vec2 bottom_right = position + (scale * 0.5f);

		return (top_left.x > screen_width       ||
			    bottom_right.x < -screen_width  ||
			    bottom_right.y < -screen_height ||
			    top_left.y     >  screen_height);
	}


	//#Settings
	void SetViewport(const int x, const int y, const int width, const int height);
	void BindVertexArray();

	//#Internal variables
	static inline unsigned int mVAO;
	static inline unsigned int mQuadVBO;
	static inline unsigned int mQuadIBO;
	static inline unsigned int mTriangleVBO;
	static inline unsigned int mTriangleIBO;
	static inline unsigned int mTextureCoordsBO;
	static inline Shader* mShapeShader;
	static inline Shader* mTextureShader;

	constinit static inline float ScaleMultiplying = 50.0f;

	//#Debug
	void DebugCallback(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message, const void* userParam);

	namespace Camera {
		static inline float ViewPlaneX = 1.0f;
		static inline float ViewPlaneY = 1.0f;
	}
}