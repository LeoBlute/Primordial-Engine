#include <iostream>
#include "glm/glm.hpp"
#include "VUtils/Events.hpp"
#include "Entity.hpp"
#include "Scene.hpp"
#include "System/Inputs.hpp"
#include "VUtils/DebugUtils.hpp"

namespace ECS
{
	static inline void KeyEventsHandle(int key, Inputs::Type type)
	{
		for (Entity* entity : Scene::mEntities)
		{
			entity->OnKeyEvent(key, type);
		}
	}
	static inline void MouseButtonEventsHandle(int button, Inputs::Type type)
	{
		for (Entity* entity : Scene::mEntities)
		{
			entity->OnMouseButtonEvent(button, type);
		}
	}

	namespace Scene {
		void Init()
		{
			constexpr size_t capacity = static_cast<const size_t>(100);
			mEntities.reserve(capacity);
			Inputs::KeyEvents.AddFunc(KeyEventsHandle);
			Inputs::MouseButtonEvents.AddFunc(MouseButtonEventsHandle);
		}
		void Terminate()
		{
			for (Entity* e : mEntities)
			{
				e->OnDestroyed();
			}
			mRegistry.clear();
		}
		void TickUpdate()
		{
			for (Entity* e : mEntities)
			{
				e->TickUpdate();
			}
		}
		void TargetUpdate()
		{
			for (Entity* e : mEntities)
			{
				e->TargetUpdate();
			}
		}
	}
	const bool LayerAlgorithm(Entity* a, Entity* b)
	{
		return a->GetComponent<CIdentity>()->Layer < b->GetComponent<CIdentity>()->Layer;
	}
	void HackOnCreated(Entity* entity)
	{
		entity->OnCreated();
	}
	void DeleteEntity(Entity* entity)
	{
		if (!entity)
			return;
		const auto it = std::find(Scene::mEntities.begin(), Scene::mEntities.end(), entity);
		if(it != Scene::mEntities.end())
		{
			Scene::mEntities.erase(it);
			//I don't know why or how there are 2 destructors being called because that stupid library is classist and works for some other don't
			const entt::entity id = entity->GetID();
			entity->OnDestroyed();
			Scene::mRegistry.destroy(id);
		}
	}

}
