#include <iostream>
#include "glm/glm.hpp"
#include "VUtils/Events.hpp"
#include "Entity.hpp"
#include "Scene.hpp"
#include "System/Inputs.hpp"

namespace ECS
{
	static inline void KeyPressedHandle(int key)
	{
		for (Entity* entity : mActiveScene->entities)
		{
			entity->OnKeyPressed(key);
		}
	}
	static inline void KeyReleasedHandle(int key)
	{
		for (Entity* entity : mActiveScene->entities)
		{
			entity->OnKeyReleased(key);
		}
	}
	static inline void KeyRepeatedHandle(int key)
	{
		for (Entity* entity : mActiveScene->entities)
		{
			entity->OnKeyRepeated(key);
		}
	}
	static inline void MousePressedHandle(int button)
	{
		for (Entity* entity : mActiveScene->entities)
		{
			entity->OnMousePressed(button);
		}
	}
	static inline void MouseReleaseHandle(int button)
	{
		for (Entity* entity : mActiveScene->entities)
		{
			entity->OnMouseReleased(button);
		}
	}
	static inline void MouseRepeatedHandle(int button)
	{
		for (Entity* entity : mActiveScene->entities)
		{
			entity->OnMouseRepeated(button);
		}
	}
	Scene* CreateScene()
	{
		Scene* scene = new Scene();
		scene->entities.reserve(100);
		mActiveScene = scene;
		Inputs::GTKeyPressed.AddFunc(KeyPressedHandle);
		Inputs::GTKeyReleased.AddFunc(KeyReleasedHandle);
		Inputs::GTKeyRepeated.AddFunc(KeyRepeatedHandle);
		Inputs::GTMouseButtonPressed.AddFunc(MousePressedHandle);
		Inputs::GTMouseButtonReleased.AddFunc(MouseReleaseHandle);
		Inputs::GTMouseButtonRepeated.AddFunc(MouseRepeatedHandle);
		return scene;
	}
	void EndScene(Scene* scene)
	{
		//Cleans entities and components
		for (Entity* entity : scene->entities)
		{
			entity->OnDestroyed();
		}
		scene->Registry.clear();
		delete scene;
	}
	void TickUpdateScene(Scene* scene)
	{
		for (Entity* e : scene->entities)
		{
			e->TickUpdate();
		}
	}
	void TargetUpdateScene(Scene* scene)
	{
		for (Entity* e : scene->entities)
		{
			e->TargetUpdate();
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
	void DeleteEntity(Scene* scene, Entity* entity)
	{
		if (!scene || !entity)
			return;

		const auto it = std::find(scene->entities.begin(), scene->entities.end(), entity);
		if(it != scene->entities.end())
		{
			scene->entities.erase(it);
			//I don't know why or how there are 2 destructors being called because that stupid library is classist and works for some other don't
			const entt::entity id = entity->GetID();
			entity->OnDestroyed();
			scene->Registry.destroy(id);
		}
	}

}