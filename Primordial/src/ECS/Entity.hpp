#pragma once

#include "Scene.hpp"

namespace ECS {
	struct Scene;
	class Entity
	{
	private:
		//Friends
		friend Scene;
		friend void EndScene(Scene* scene);
		friend void TickUpdateScene(Scene* scene);
		friend void TargetUpdateScene(Scene* scene);
		friend void DeleteEntity(Scene* scene, Entity* entity);
		friend void HackOnCreated(Entity* entity);
		friend void KeyPressedHandle(int key);
		friend void KeyReleasedHandle(int key);
		friend void KeyRepeatedHandle(int key);
		friend void MousePressedHandle(int button);
		friend void MouseReleaseHandle(int button);
		friend void MouseRepeatedHandle(int button);
	private:
		Entity() = delete;
		Entity(const Entity&) = delete;
	public:
		Entity(const entt::entity id, Scene* scene) noexcept
			:ID(id), mAssignedScene(scene)
		{
			transform = this->GetComponent<CTransform>();
		}
		~Entity() = default;
	protected:
		virtual void OnCreated() {};
		virtual void OnDestroyed() {};
		virtual void TargetUpdate() {};
		virtual void TickUpdate() {};

		virtual void OnKeyPressed(int key) {};
		virtual void OnKeyReleased(int key) {};
		virtual void OnKeyRepeated(int key) {};
		virtual void OnMousePressed(int button) {};
		virtual void OnMouseReleased(int button) {};
		virtual void OnMouseRepeated(int button) {};
	public:
		template<typename T>
		inline const bool HasComponent()
		{
			return mAssignedScene->Registry.any_of<T>(ID);
		}

		template<typename T>
		inline T* GetComponent()
		{
			if (!HasComponent<T>())
				return NULL;

			return &mAssignedScene->Registry.get<T>(ID);
		}

		template<typename T>
		inline void RemoveComponent()
		{
			if (!HasComponent<T>())
				return;

			mAssignedScene->Registry.remove<T>(ID);
		}

		template<typename T, typename... Args>
		inline T* AddComponent(Args&&... args)
		{
			return &mAssignedScene->Registry.emplace<T>(ID, std::forward<Args>(args)...);
		}

		template<typename T, typename... Args>
		inline T* AddOrReplaceComponent(Args&&... args)
		{
			return &mAssignedScene->Registry.emplace_or_replace<T>(ID, std::forward<Args>(args)...);
		}

	public:
		inline const entt::entity GetID() const { return ID; };
		CTransform* transform;
	private:
		Scene* mAssignedScene;
		entt::entity ID;
	};
}