#pragma once

#include "entt/entt.hpp"
#include "Components/CTransform.hpp"
#include "Components/CIdentity.hpp"

class CPhysicsBody;

namespace ECS {
	class Entity;

	CIdentity* _HackToGetIdentity(ECS::Entity* e);
	void _HackOnCreated(Entity* entity);

	namespace Scene {
		#pragma region Life cycle
		void Init();
		void Terminate();
		void TickUpdate();
		void TargetUpdate();
		#pragma endregion

		inline std::vector<Entity*> mEntities;
		inline entt::registry mRegistry;

		#pragma region Utilities
		template<typename T>
		constexpr static inline std::vector<T*> GetEntitiesOrComponentsByType()
		{
			std::vector<T*> list;
			const auto view = mRegistry.view<T>();
			for (const entt::entity id : view)
			{
				list.emplace_back(&view.get<T>(id));
			}
			return list;
		}
		template<>
		constexpr static inline std::vector<ECS::Entity*> GetEntitiesOrComponentsByType<ECS::Entity>()
		{
			return Scene::mEntities;
		}

		template<typename T>
		constexpr static inline std::vector<T*> GetEntitiesOrComponentsByName(const std::string& name)
		{
			std::vector<T*> list;
			const auto view = mRegistry.view<T>();
			for (const entt::entity id : view)
			{
				CIdentity& i = mRegistry.get<CIdentity>(id);
				if (!i.Name.compare(name))
					list.emplace_back(&view.get<T>(id));
			}
			return list;
		}
		template<>
		constexpr static inline std::vector<ECS::Entity*> GetEntitiesOrComponentsByName<ECS::Entity>(const std::string& name)
		{
			std::vector<ECS::Entity*> list;
			for (ECS::Entity* e : mEntities)
			{
				CIdentity* i = _HackToGetIdentity(e);
				if(!i->Name.compare(name))
					list.emplace_back(e);
			}
			return list;
		}

		#pragma endregion

		namespace Physics {
			#pragma region Life cycle
			void Init();
			void Terminate();
			void Update(const float timestep);
			#pragma endregion
			#pragma region Getters and setters
			const glm::vec2 GetGravity();
			void SetGravity(const glm::vec2& value);
			void* GetRawWorld();
			#pragma endregion
		}
	}

	const bool LayerAlgorithm(Entity* a, Entity* b);

	//This function creates a entity correctly,it is a friend of class Entity
	template<typename T>
	constexpr static inline T* CreateEntity(const std::string& name, unsigned int layer = 0,
		const glm::vec2& position = glm::vec2(0.0f, 0.0f), const float rotation = 0.0f,
		const glm::vec2& scale = glm::vec2(1.0f, 1.0f))
	{
		entt::registry& reg = Scene::mRegistry;
		const entt::entity id = reg.create();
		//Creates some essential components for the entity
		reg.emplace<CTransform>(id, position, scale, rotation);
		reg.emplace<CIdentity>(id, name, layer);
		T* entity = &reg.emplace<T>(id, id);
		_HackOnCreated(entity);
		Scene::mEntities.emplace_back(entity);
		//Maybe when the antichrist comes this code will be acceptable
		std::sort(Scene::mEntities.begin(), Scene::mEntities.end(), LayerAlgorithm);
		return entity;
	};

	//This function deletes a entity correctly,it is a friend of class Entity
	void DeleteEntity(Entity* entity);
}