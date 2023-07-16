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
	mShapeShader = new Shader("res/Shaders/ShapeVertex2D.shader", "res/Shaders/ShapeFragment2D.shader");
	mTextureShader = new Shader("res/Shaders/TextureVertex2D.shader", "res/Shaders/TextureFragment2D.shader");

	//Create and bind Vertex Array Object
	glCreateVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	constexpr float quadVertices[] = {
		 0.5f,  0.5f,  // top right
		 0.5f, -0.5f,  // bottom right
		-0.5f, -0.5f,  // bottom left
		-0.5f,  0.5f   // top left 
	};
	constexpr unsigned int quadIndices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};

	constexpr float triangleVertices[] = {
		-0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.0f,  0.5f
	};
	constexpr unsigned int triangleIndices[] = {
		0, 1, 2
	};

	constexpr float quadTexCoords[] = {
		1.0f, 1.0f,  // top right
		1.0f, 0.0f,  // bottom right
		0.0f, 0.0f,  // bottom left
		0.0f, 1.0f   // top left
	};
	
	constexpr float triangleTexCoords[] = {
		0.0f, 0.0f,  // bottom left
		1.0f, 0.0f,  // bottom right
		0.5f, 1.0f  // top
	};

	//Quad bufffers
	glCreateBuffers(1, &mQuadVBO);
	glCreateBuffers(1, &mQuadIBO);
	glBindBuffer(GL_ARRAY_BUFFER, mQuadVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mQuadIBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices[0], GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), &quadIndices[0], GL_STATIC_DRAW);

	//Triangle buffers
	glCreateBuffers(1, &mTriangleVBO);
	glCreateBuffers(1, &mTriangleIBO);
	glBindBuffer(GL_ARRAY_BUFFER, mTriangleVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mTriangleIBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), &triangleVertices[0], GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangleIndices), &triangleIndices[0], GL_STATIC_DRAW);

	//Texture quad buffers
	glCreateBuffers(1, &mQuadTexCoordsBO);
	glBindBuffer(GL_ARRAY_BUFFER, mQuadTexCoordsBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadTexCoords),&quadTexCoords[0], GL_STATIC_DRAW);

	//Texture triangle buffers
	glCreateBuffers(1, &mTriangleTexCoordsBO);
	glBindBuffer(GL_ARRAY_BUFFER, mTriangleTexCoordsBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleTexCoords),&triangleTexCoords[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//Enable vertext attributes
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}

void Renderer2D::Terminate()
{
	delete mShapeShader;
	delete mTextureShader;

	glDeleteBuffers(1, &mQuadVBO);
	glDeleteBuffers(1, &mQuadIBO);
	glDeleteBuffers(1, &mTriangleVBO);
	glDeleteBuffers(1, &mTriangleIBO);
	glDeleteBuffers(1, &mQuadTexCoordsBO);
	glDeleteBuffers(1, &mTriangleTexCoordsBO);

	glDeleteVertexArrays(1, &mVAO);
}

void Renderer2D::Clear(const glm::vec4& color, const unsigned int mask)
{
	glClearColor(color.x, color.y, color.z, color.w);
	glClear(mask);
}

void Renderer2D::DrawShape(const Shape shape, const glm::vec2& position, const glm::vec2& scale, const float rotation, const glm::vec4& color)
{
	DrawTexture(shape, NULL, glm::vec2(1.0f), position, scale, rotation, color);
}

void Renderer2D::DrawTexture(const Shape shape, Texture* texture, const glm::vec2& repetition, const glm::vec2& position, const glm::vec2& scale, const float rotation, const glm::vec4& color)
{
	const bool has_texture = texture != NULL;

	//Get opengl current viewport values
	glm::ivec4 viewport(0);
	glGetIntegerv(GL_VIEWPORT, &viewport.x);
	const float width = static_cast<float>(viewport.z);
	const float height = static_cast<float>(viewport.w);

	//View dimensions
	const float view_width = width * Camera::ViewPlaneX;
	const float view_height = height * Camera::ViewPlaneY;

	//Calculate object's bounds
	const glm::vec2 final_scale = glm::vec2(scale * ScaleMultiplying);

	//Check if the object is outside viewport
	if (CheckVisibility(position * ScaleMultiplying, final_scale, view_width, view_height))
		return;

	//Values
	constexpr float limit = 1.0f;
	constexpr glm::mat4 mat4_base = glm::mat4(1.0f);

	//Camera projection and view
	const glm::mat4 projection = glm::ortho(-view_width, view_width, -view_height, view_height, -limit, limit);
	const glm::mat4 view = glm::translate(mat4_base, glm::vec3(0.0f, 0.0f, 0.0f));
	//Transformation
	const glm::mat4 model_translate = glm::translate
	(mat4_base, glm::vec3(position.x * ScaleMultiplying, position.y * ScaleMultiplying, 1.0f));
	const glm::mat4 model_scale = glm::scale(mat4_base, glm::vec3(final_scale, 0.0f));
	const glm::mat4 model_rotation = glm::rotate(mat4_base, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
	const glm::mat4 model = model_translate * model_scale * model_rotation;

	Shader* use_shader = nullptr;
	if (has_texture)
	{
		use_shader = mTextureShader;
		use_shader->Use();

		//Set texture repetition in shader
		use_shader->SetVec2("uTexRepetition", repetition);

		glActiveTexture(GL_TEXTURE0);
		//Bind texture, ~obrigado pela explanação
		texture->Bind();
	}
	else
	{
		use_shader = mShapeShader;
		use_shader->Use();
	}

	//Set Matrices data in shader
	use_shader->SetMat4("uProjection", projection);
	use_shader->SetMat4("uView", view);
	use_shader->SetMat4("uModel", model);

	//Set color data in shader
	use_shader->SetVec4("uColor", color);

	switch (shape)
	{
	case Renderer2D::Quad:
	{
		if (has_texture)
		{
			glBindBuffer(GL_ARRAY_BUFFER, mQuadTexCoordsBO);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		}

		glBindBuffer(GL_ARRAY_BUFFER, mQuadVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mQuadIBO);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

		//Draw data
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
	break;
	case Renderer2D::Triangle:
	{
		if (has_texture)
		{
			glBindBuffer(GL_ARRAY_BUFFER, mTriangleTexCoordsBO);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		}

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

void Renderer2D::SetViewport(const int x, const int y, const int width, const int height)
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
