#include "pch.h"
#include "RenderingObject.h"
#include "Renderer.h"
#include "Texture.h"
#include "Glad/glad.h"

RenderingObject::RenderingObject(std::vector<float>* verticesData, std::vector<unsigned int>* indicesData)
	:mVerticesData(verticesData), mIndicesData(indicesData), mTexture(nullptr), mPosition(0, 0 ,0)
{
	glCreateBuffers(1, &mVBO);
	glCreateBuffers(1, &mIBO);

	Renderer::AddRO(this);

	Bind();
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mVerticesData->size(), mVerticesData->data(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mIndicesData->size(), mIndicesData->data(), GL_STATIC_DRAW);
}

RenderingObject::RenderingObject(std::vector<float>* verticesData, std::vector<unsigned int>* indicesData, const char* path)
	:mVerticesData(verticesData), mIndicesData(indicesData), mTexture(new Texture(path)), mPosition(0, 0, 0)
{
	glCreateBuffers(1, &mVBO);
	glCreateBuffers(1, &mIBO);

	Renderer::AddRO(this);

	//Cannot use bind because it also binds the texture
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mVerticesData->size(), mVerticesData->data(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mIndicesData->size(), mIndicesData->data(), GL_STATIC_DRAW);
}

RenderingObject::~RenderingObject()
{
	if (mTexture)
		delete mTexture;

	glDeleteBuffers(1, &mVBO);
	glDeleteBuffers(1, &mIBO);
}

const glm::vec3& RenderingObject::GetPosition()
{
	return mPosition;
}

void RenderingObject::SetPosition(const glm::vec3& pos)
{
	mPosition = pos;
}

void RenderingObject::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
	if (mTexture)
		mTexture->Bind();
}
