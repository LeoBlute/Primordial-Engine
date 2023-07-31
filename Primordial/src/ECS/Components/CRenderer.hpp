#pragma once

#include "glm/vec2.hpp"
#include "glm/vec4.hpp"

#include "CTransform.hpp"

namespace Renderer2D
{
	class Texture;
}

class CRenderer
{
private:
	friend void ECS::Scene::TargetUpdate();
public:
	enum Shape
	{
		Quad     = 0,
		Triangle = 1
	};
	struct TexDef
	{
		glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		CRenderer::Shape shape = Quad;
		//determines how much repetition the texture will have
		glm::vec2 texRepetition = glm::vec2(1.0f, 1.0f);
		Renderer2D::Texture* texture = NULL;
	};
	struct ShapeDef
	{
		glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		CRenderer::Shape shape = Quad;
	};
	struct Data
	{
		glm::vec4 color;
		CRenderer::Shape shape;
		//determines how much repetition the texture will have
		glm::vec2 texRepetition;
		Renderer2D::Texture* texture;
	};
public:
	CRenderer() = delete;
	CRenderer(ECS::Entity* e, const unsigned int _layer = 0)
		:layer(_layer)
	{
		mAssignedEntity = e;
		mAssignedTransform = e->transform;
		data.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		data.shape = Shape::Quad;
		data.texRepetition = glm::vec2(1.0f, 1.0f);
		data.texture = NULL;
	};
	CRenderer(ECS::Entity* e, const TexDef def, const unsigned int _layer = 0)
		:layer(_layer)
	{
		mAssignedEntity = e;
		mAssignedTransform = e->transform;
		data.color = def.color;
		data.shape = def.shape;
		data.texRepetition = def.texRepetition;
		data.texture = def.texture;
	};
	CRenderer(ECS::Entity* e, const ShapeDef def, const unsigned int _layer = 0)
		:layer(_layer)
	{
		mAssignedEntity = e;
		mAssignedTransform = e->transform;
		data.color = def.color;
		data.shape = def.shape;
		data.texRepetition = glm::vec2(1.0f, 1.0f);
		data.texture = NULL;
	};
	CRenderer(const CRenderer&) = delete;
	~CRenderer() = default;
	inline bool HasTexture() const { return (data.texture != NULL); };
	bool operator==(const CRenderer& other) const = default;
public:
	Data data;
	unsigned int layer;
	bool enable = true;
private:
	ECS::Entity* mAssignedEntity;
	CTransform* mAssignedTransform;
};