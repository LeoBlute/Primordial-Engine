
namespace Renderer2D
{
	class Shader;
	class Texture;

	enum Primitive
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
	void Clear(const glm::vec4& color, unsigned int mask);
	void DrawPrimitive(Primitive type, const glm::vec2& position, const glm::vec2& scale, float rotation, const glm::vec4& color);
	void DrawTexture(Texture* texture, const glm::vec2& repetition,
		const glm::vec2& position, const glm::vec2& scale, float rotation, const glm::vec4& color);

	//#Settings
	void SetViewport(int x, int y, int width, int height);
	void BindVertexArray();

	//#Internal variables
	static inline unsigned int mVAO;
	static inline unsigned int mQuadVBO;
	static inline unsigned int mQuadIBO;
	static inline unsigned int mTriangleVBO;
	static inline unsigned int mTriangleIBO;
	static inline unsigned int mTextureVBO;
	static inline unsigned int mTextureIBO;
	static inline Shader* mPrimitiveShader;
	static inline Shader* mTextureShader;

	//#Debug
	void DebugCallback(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message, const void* userParam);
}