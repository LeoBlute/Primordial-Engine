#pragma once

#include "Scene.hpp"
#include "Components/Component.hpp"

class CPhysicsBody;
class CRenderer;
class Component;

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
			transform = this->GetComponent<CTransform>();
			identity = this->GetComponent<CIdentity>();
		}
		~Entity() = default;
		bool operator==(const Entity& other) const = default;
	private:
		void _PrintComponentWarning(const char* str) const;
	protected:
		virtual void OnCreated() {};
		virtual void OnDestroyed() {};
		virtual void OnTargetUpdate() {};
		virtual void OnTickUpdate() {};
		void TargetUpdate();
		void TickUpdate();

		virtual void OnKeyEvent(int key, Inputs::Type type) {};
		virtual void OnMouseButtonEvent(int key, Inputs::Type type) {};
		virtual void OnScrollEvent(float xoffset, float yoffset) {};

		virtual void OnCollisionBegin(CPhysicsBody* other) {};
		virtual void OnCollisionEnd(CPhysicsBody* other) {};
	public:
		template<typename T>
		inline bool HasComponent() const
		{
			return Scene::mRegistry.any_of<T>(mID);
		}

		template<typename T>
		inline T* GetComponent() const
		{
			if (!HasComponent<T>())
				return NULL;

			T* c = &Scene::mRegistry.get<T>(mID);
			return c;
		}

		template<typename T>
		inline void RemoveComponent()
		{
			if (!HasComponent<T>())
				return;

			Component* component_c = GetComponent<T>();
			//component_c->OnDestroyed();
			auto it = std::find(mAttachedComponents.begin(),
				mAttachedComponents.end(), component_c);
			if (it != mAttachedComponents.end())
			{
				mAttachedComponents.erase(it);
			}

			Scene::mRegistry.remove<T>(mID);
		}

		template<typename T>
		inline T* AddComponent()
		{
			if (HasComponent<T>())
			{
				_PrintComponentWarning("Entity already has component,\
				cannot add another on top");
				return GetComponent<T>();
			}
			T* c = &Scene::mRegistry.emplace<T>(mID, this);
			Component* component_c = c;
			mAttachedComponents.emplace_back(component_c);
			return c;
		}

		template<typename... Args>
		inline CPhysicsBody* AddPhysicsBody(Args&&... args)
		{
			if (HasComponent<CPhysicsBody>())
			{
				_PrintComponentWarning("Entity already has component CPhysicsBody\
				,cannot add another on top");
				return GetComponent<CPhysicsBody>();
			}
			return &Scene::mRegistry.emplace<CPhysicsBody>(mID, this, std::forward<Args>(args)...);
		}

		template<typename... Args>
		inline CRenderer* AddRenderer(Args&&... args)
		{
			if (HasComponent<CRenderer>())
			{
				_PrintComponentWarning("Entity already has component CRenderer\
				,cannot add another on top");
				return GetComponent<CRenderer>();
			}
			return &Scene::mRegistry.emplace<CRenderer>(mID, this, std::forward<Args>(args)...);
		}

		void RemovePhysicsBody();

		void RemoveCRenderer();

	public:
		constexpr inline const entt::entity GetID() const { return mID; };
		constexpr inline bool IsEnable() const { return mEnable; };
		//Determines if the entity will receive updates requests
		constexpr inline void SetEnable(const bool enable) { mEnable = enable; };
		CTransform* transform;
		CIdentity* identity;
	private:
		std::vector<Component*> mAttachedComponents;
		const entt::entity mID;
		bool mEnable = true;
	};
}