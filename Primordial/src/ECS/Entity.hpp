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
		friend void _HackOnCreated(Entity* entity);
		friend void KeyEventsHandle(int key, Inputs::Type type);
		friend void MouseButtonEventsHandle(int key, Inputs::Type type);
		friend void ScrollEventsHandle(const float xoffset, const float yoffset);
		friend class CPhysicsBody;
	private:
		Entity() = delete;
		Entity(const Entity&) = delete;
	public:
		Entity(const entt::entity id) noexcept
			:mID(id)
		{
			Transform = this->GetComponent<CTransform>();
			Identity = this->GetComponent<CIdentity>();
		}
		~Entity() = default;
		bool operator==(const Entity& other) const = default;
	protected:
		virtual void OnCreated() {};
		virtual void OnDestroyed() {};
		virtual void TargetUpdate() {};
		virtual void TickUpdate() {};

		virtual void OnKeyEvent(int key, Inputs::Type type) {};
		virtual void OnMouseButtonEvent(int key, Inputs::Type type) {};
		virtual void OnScrollEvent(float xoffset, float yoffset) {};

		virtual void OnCollisionBegin(CPhysicsBody* other) {};
		virtual void OnCollisionEnd(CPhysicsBody* other) {};
	public:
		template<typename T>
		inline bool HasComponent()
		{
			return Scene::mRegistry.any_of<T>(mID);
		}

		template<typename T>
		inline T* GetComponent()
		{
			if (!HasComponent<T>())
				return NULL;

			return &Scene::mRegistry.get<T>(mID);
		}

		template<typename T>
		inline void RemoveComponent()
		{
			if (!HasComponent<T>())
				return;

			Scene::mRegistry.remove<T>(mID);
		}

		template<typename T, typename... Args>
		inline T* AddComponent(Args&&... args)
		{
			return &Scene::mRegistry.emplace<T>(mID, std::forward<Args>(args)...);
		}

		template<typename T, typename... Args>
		inline T* AddOrReplaceComponent(Args&&... args)
		{
			return &Scene::mRegistry.emplace_or_replace<T>(mID, std::forward<Args>(args)...);
		}

	public:
		inline const entt::entity GetID() const { return mID; };
		CTransform* Transform;
		CIdentity* Identity;
	private:
		const entt::entity mID;
	};
}