#include "glm/glm.hpp"
#include "Entity.hpp"
#include "Scene.hpp"

namespace ECS
{
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

		auto it = std::find(scene->entities.begin(), scene->entities.end(), entity);
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