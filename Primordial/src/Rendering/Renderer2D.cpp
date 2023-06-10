#include "pch.h"
#include "Renderer2D.hpp"
#include "Rendering/Shader.hpp"
#include "Rendering/Texture.hpp"
#include "Glad/glad.h"

void Renderer2D::Init(void* procAdress)
{
	//Initialize opengl with glad
	if (!gladLoadGLLoader(static_cast<GLADloadproc>(procAdress))) [[unlikely]]
	{
		DEBUG_ERROR("Glad could not initialize");
	}

//Setup opengl's debug context
#ifdef DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	int context_flags = 0;
	glGetIntegerv(GL_CONTEXT_FLAGS, &context_flags);

	if (context_flags == GL_CONTEXT_FLAG_DEBUG_BIT) [[unlikely]]
	{
		DEBUG_INFO("GL Debug Context has been created");
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(DebugCallback, NULL);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
	}
	else
	{
		DEBUG_INFO("GL Debug Context has not been created");
	}

#endif // DEBUG

	//Shader initialization
	mPrimitiveShader = new Shader("res/Shaders/PrimitiveVertex2D.shader", "res/Shaders/PrimitiveFragment2D.shader");
	mTextureShader = new Shader("res/Shaders/TextureVertex2D.shader", "res/Shaders/TextureFragment2D.shader");

	//Create and bind Vertex Array Object
	glCreateVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	//std::vector<glm::vec2> quadVertices = {
	//	glm::vec2(1.0f, 1.0f),  // top right
	//	glm::vec2(1.0f, 0.0f),  // bottom right
	//	glm::vec2(0.0f, 0.0f),  // bottom left
	//	glm::vec2(0.0f, 1.0f)   // top left 
	//};
	const std::vector<glm::vec2> quadVertices = {
		glm::vec2(0.5f, 0.5f),  // top right
		glm::vec2(0.5f, -0.5f),  // bottom right
		glm::vec2(-0.5f, -0.5f),  // bottom left
		glm::vec2(-0.5f, 0.5f)   // top left 
	};
	const std::vector<unsigned int> quadIndices = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};

	const std::vector<glm::vec2> triangleVertices = {
		glm::vec2(-0.5f, -0.5f),
		glm::vec2( 0.5f, -0.5f),
		glm::vec2(-0.5f, 0.5f)
	};
	const std::vector<unsigned int> triangleIndices = {
		0, 1, 3
	};

	const std::vector<glm::vec4> textureVertices = {
		glm::vec4( 0.5f,  0.5f, 1.0f, 1.0f),  // top right
		glm::vec4( 0.5f, -0.5f, 1.0f, 0.0f),  // bottom right
		glm::vec4(-0.5f, -0.5f, 0.0f, 0.0f),  // bottom left
		glm::vec4(-0.5f,  0.5f, 0.0f, 1.0f)   // top left 
	};
	const std::vector<unsigned int> textureIndices = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};

	//Quad bufffers
	glCreateBuffers(1, &mQuadVBO);
	glCreateBuffers(1, &mQuadIBO);
	glBindBuffer(GL_ARRAY_BUFFER, mQuadVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mQuadIBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * quadVertices.size(), quadVertices.data(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * quadIndices.size(), quadIndices.data(), GL_STATIC_DRAW);

	//Triangle buffers
	glCreateBuffers(1, &mTriangleVBO);
	glCreateBuffers(1, &mTriangleIBO);
	glBindBuffer(GL_ARRAY_BUFFER, mTriangleVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mTriangleIBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * triangleVertices.size(), triangleVertices.data(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * triangleIndices.size(), triangleIndices.data(), GL_STATIC_DRAW);

	//Texture buffers
	glCreateBuffers(1, &mTextureVBO);
	glCreateBuffers(1, &mTextureIBO);
	glBindBuffer(GL_ARRAY_BUFFER, mTextureVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mTextureIBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * textureVertices.size(), textureVertices.data(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * textureIndices.size(), textureIndices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//Enable vertext attributes
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}

void Renderer2D::Terminate()
{
	delete mPrimitiveShader;
	delete mTextureShader;

	glDeleteBuffers(1, &mQuadVBO);
	glDeleteBuffers(1, &mQuadIBO);
	glDeleteBuffers(1, &mTriangleVBO);
	glDeleteBuffers(1, &mTriangleIBO);
	glDeleteBuffers(1, &mTextureVBO);
	glDeleteBuffers(1, &mTextureIBO);

	glDeleteVertexArrays(1, &mVAO);
}

void Renderer2D::Clear(const glm::vec4& color, unsigned int mask)
{
	glClearColor(color.x, color.y, color.z, color.w);
	glClear(mask);
}

void Renderer2D::DrawPrimitive(Primitive type, const glm::vec2& position, const glm::vec2& scale, float rotation, const glm::vec4& color)
{
	//Get opengl current viewport values
	glm::ivec4 viewport(0);
	glGetIntegerv(GL_VIEWPORT, &viewport.x);
	const float width  = static_cast<float>(viewport.z);
	const float height = static_cast<float>(viewport.w);

	//Bind/Use appropriate shader
	mPrimitiveShader->Use();

	//Values
	constexpr float s_mult = 50.0f;
	constexpr float limit = 1.0f;
	constexpr glm::mat4 mat4_base = glm::mat4(1.0f);

	//Matrices
	const glm::mat4 projection = glm::ortho(-width, width, -height, height, -limit, limit);
	const glm::mat4 view = glm::translate(mat4_base, glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 model = glm::translate(mat4_base, glm::vec3(position.x, position.y, 1.0f));
	model = glm::scale(model, glm::vec3(scale.x * s_mult, scale.y * s_mult, 0.0f));
	model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));

	//Set Matrices data in shader
	mPrimitiveShader->SetMat4("uProjection", projection);
	mPrimitiveShader->SetMat4("uView", view);
	mPrimitiveShader->SetMat4("uModel", model);

	//Set color data in shader
	mPrimitiveShader->SetVec4("uColor", color);

	switch (type)
	{
	case Renderer2D::Quad:
	{
		glBindBuffer(GL_ARRAY_BUFFER, mQuadVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mQuadIBO);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

		//Draw data
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
		break;
	case Renderer2D::Triangle:
	{
		glBindBuffer(GL_ARRAY_BUFFER, mTriangleVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mTriangleIBO);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

		//Draw data
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	}
		break;
	default:
		break;
	}
}

void Renderer2D::DrawTexture(Texture* texture, const glm::vec2& repetition,
	const glm::vec2& position, const glm::vec2& scale, float rotation, const glm::vec4& color)
{
	if (!texture)
	{
		DEBUG_INFO("Você é um animal, texture cannot be a null pointer");
		return;
	}

	//Get opengl current viewport values
	glm::ivec4 viewport(0);
	glGetIntegerv(GL_VIEWPORT, &viewport.x);
	const float width =  static_cast<float>(viewport.z);
	const float height = static_cast<float>(viewport.w);

	mTextureShader->Use();

	glBindBuffer(GL_ARRAY_BUFFER, mTextureVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mTextureIBO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	//Values
	constexpr float s_mult = 50.0f;
	constexpr float limit = 1.0f;
	constexpr glm::mat4 mat4_base = glm::mat4(1.0f);

	//Matrices
	const glm::mat4 projection = glm::ortho(-width, width, -height, height, -limit, limit);
	const glm::mat4 view = glm::translate(mat4_base, glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 model = glm::translate(mat4_base, glm::vec3(position.x, position.y, 1.0f));
	model = glm::scale(model, glm::vec3(scale.x * s_mult, scale.y * s_mult, 0.0f));
	model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));

	//Set Matrices data in shader
	mTextureShader->SetMat4("uProjection", projection);
	mTextureShader->SetMat4("uView", view);
	mTextureShader->SetMat4("uModel", model);

	//Set texture repetition in shader
	mTextureShader->SetVec2("uTexRepetition", repetition);
	//Set color data in shader
	mTextureShader->SetVec4("uColor", color);

	glActiveTexture(GL_TEXTURE0);
	//Bind texture, ~obrigado pela explanação
	texture->Bind();

	//Draw data
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Renderer2D::SetViewport(int x, int y, int width, int height)
{
	glViewport(x, y, width, height);
}

void Renderer2D::BindVertexArray()
{
	glBindVertexArray(mVAO);
}

void Renderer2D::DebugCallback(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message, const void* userParam)
{
	DEBUG_CRIT("GL_ERROR!");
	std::cout << message << std::endl;
}
