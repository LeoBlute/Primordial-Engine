#pragma once

#include "Scene.hpp"

namespace Inputs {
	enum Type;
}

namespace ECS {
	class Entity
	{
	private:
		//Friends
		friend void DeleteEntity(Entity* entity);
		friend void Scene::Terminate();
		friend void Scene::TickUpdate();
		friend void Scene::TargetUpdate();
		friend void HackOnCreated(Entity* entity);
		friend void KeyEventsHandle(int key, Inputs::Type type);
		friend void MouseButtonEventsHandle(int key, Inputs::Type type);
	private:
		Entity() = delete;
		Entity(const Entity&) = delete;
	public:
		Entity(const entt::entity id) noexcept
			:ID(id)
		{
			transform = this->GetComponent<CTransform>();
		}
		~Entity() = default;
	protected:
		virtual void OnCreated() {};
		virtual void OnDestroyed() {};
		virtual void TargetUpdate() {};
		virtual void TickUpdate() {};

		virtual void OnKeyEvent(int key, Inputs::Type type) {};
		virtual void OnMouseButtonEvent(int key, Inputs::Type type) {};
	public:
		template<typename T>
		inline const bool HasComponent()
		{
			return Scene::mRegistry.any_of<T>(ID);
		}

		template<typename T>
		inline T* GetComponent()
		{
			if (!HasComponent<T>())
				return NULL;

			return &Scene::mRegistry.get<T>(ID);
		}

		template<typename T>
		inline void RemoveComponent()
		{
			if (!HasComponent<T>())
				return;

			Scene::mRegistry.remove<T>(ID);
		}

		template<typename T, typename... Args>
		inline T* AddComponent(Args&&... args)
		{
			return &Scene::mRegistry.emplace<T>(ID, std::forward<Args>(args)...);
		}

		template<typename T, typename... Args>
		inline T* AddOrReplaceComponent(Args&&... args)
		{
			return &Scene::mRegistry.emplace_or_replace<T>(ID, std::forward<Args>(args)...);
		}

	public:
		inline const entt::entity GetID() const { return ID; };
		CTransform* transform;
	private:
		entt::entity ID;
	};
}