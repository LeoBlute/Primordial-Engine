#pragma once

#include "entt/entt.hpp"
#include "Components/CTransform.hpp"
#include "Components/CIdentity.hpp"

namespace ECS {
	class Entity;

	//Holds entities
	struct Scene
	{
	private:
		//Friends, more than you have
		friend Entity;
		friend Scene* CreateScene();
		friend void EndScene(Scene* scene);
		friend void TickUpdateScene(Scene* scene);
		friend void TargetUpdateScene(Scene* scene);
		friend void DeleteEntity(Scene* scene, Entity* entity);
	public:
		Scene() = default;
		~Scene() = default;

		entt::registry Registry;
	public:
		//Entities in scene
		std::vector<Entity*> entities;
	};

	static inline Scene* mActiveScene;

	Scene* CreateScene();
	void EndScene(Scene* scene);
	void TickUpdateScene(Scene* scene);
	void TargetUpdateScene(Scene* scene);

	const bool LayerAlgorithm(Entity* a, Entity* b);
	void HackOnCreated(Entity* entity);

	//This function creates a entity correctly,it is a friend of class Entity
	template<typename T>
	constexpr static inline T* CreateEntity(Scene* scene, const std::string& name, unsigned int layer = 0,
		const glm::vec2& position = glm::vec2(0.0f, 0.0f), const glm::vec2& scale = glm::vec2(1.0f, 1.0f))
	{
		if (!scene)
			return nullptr;

		const entt::entity id = scene->Registry.create();
		//Creates some essential components for the entity
		scene->Registry.emplace<CTransform>(id, position, scale, 0.0f);
		scene->Registry.emplace<CIdentity>(id, name, layer);
		T* entity = &scene->Registry.emplace<T>(id, id, scene);
		HackOnCreated(entity);
		scene->entities.emplace_back(entity);
		//Maybe when the antichrist comes this code will be acceptable
		std::sort(scene->entities.begin(), scene->entities.end(), LayerAlgorithm);
		return entity;
	};

	//This function deletes a entity correctly,it is a friend of class Entity
	void DeleteEntity(Scene* scene, Entity* entity);
}