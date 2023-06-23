#pragma once

#include "entt/entt.hpp"
#include "Components/CTransform.hpp"
#include "Components/CIdentity.hpp"

namespace ECS {
	class Entity;
	namespace Scene {
		//Life cycle
		void Init();
		void Terminate();
		void TickUpdate();
		void TargetUpdate();

		inline std::vector<Entity*> mEntities;
		inline entt::registry mRegistry;
	}

	const bool LayerAlgorithm(Entity* a, Entity* b);
	void HackOnCreated(Entity* entity);

	//This function creates a entity correctly,it is a friend of class Entity
	template<typename T>
	constexpr static inline T* CreateEntity(const std::string& name, unsigned int layer = 0,
		const glm::vec2& position = glm::vec2(0.0f, 0.0f), const glm::vec2& scale = glm::vec2(1.0f, 1.0f))
	{
		entt::registry& reg = Scene::mRegistry;
		const entt::entity id = reg.create();
		//Creates some essential components for the entity
		reg.emplace<CTransform>(id, position, scale, 0.0f);
		reg.emplace<CIdentity>(id, name, layer);
		T* entity = &reg.emplace<T>(id, id);
		HackOnCreated(entity);
		Scene::mEntities.emplace_back(entity);
		//Maybe when the antichrist comes this code will be acceptable
		std::sort(Scene::mEntities.begin(), Scene::mEntities.end(), LayerAlgorithm);
		return entity;
	};

	//This function deletes a entity correctly,it is a friend of class Entity
	void DeleteEntity(Entity* entity);
}