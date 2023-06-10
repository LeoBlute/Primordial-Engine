#pragma once

class Texture;

class RenderingObject
{
private :
	unsigned int mVBO;
	unsigned int mIBO;

	std::vector<float>* mVerticesData;
	std::vector<unsigned int>* mIndicesData;

	Texture* mTexture;

	glm::vec3 mPosition;
public:
	RenderingObject() = delete;
	RenderingObject(std::vector<float>* verticesData, std::vector<unsigned int>* indicesData);
	RenderingObject(std::vector<float>* verticesData, std::vector<unsigned int>* indicesData, const char* path);
	~RenderingObject();

	unsigned int GetVerticesSize() const { return mVerticesData->size() * sizeof(float); };
	unsigned int GetVerticesCount() const { return mVerticesData->size(); };
	unsigned int GetIndicesSize() const { return mIndicesData->size() * sizeof(unsigned int); };
	unsigned int GetIndicesCount() const { return mIndicesData->size(); };

	const glm::vec3& GetPosition();
	void SetPosition(const glm::vec3& pos);

	void Bind();
};