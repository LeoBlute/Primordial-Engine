#include <iostream>
#include "glm/glm.hpp"
#include "VUtils/Events.hpp"
#include "box2d/box2d.h"
#include "Entity.hpp"
#include "Scene.hpp"
#include "System/Inputs.hpp"
#include "Rendering/Renderer2D.hpp"
#include "ECS/Components/CCollision.hpp"
#include "VUtils/DebugUtils.hpp"

namespace ECS
{
	static inline void KeyEventsHandle(const int key, const Inputs::Type type)
	{
		for (Entity* entity : Scene::mEntities)
		{
			entity->OnKeyEvent(key, type);
		}
	}
	static inline void MouseButtonEventsHandle(const int button, const Inputs::Type type)
	{
		for (Entity* entity : Scene::mEntities)
		{
			entity->OnMouseButtonEvent(button, type);
		}
	}
	static inline void ScrollEventsHandle(const float xoffset, const float yoffset)
	{
		for (Entity* entity : Scene::mEntities)
		{
			entity->OnScrollEvent(xoffset, yoffset);
		}
	}

	namespace Scene {
		namespace Physics {
			void Init()
			{
				const b2Vec2 non_existant(0.0f, -50.0f);
				mWorld = new b2World(non_existant);
			}
			void Terminate()
			{
				if (mWorld->GetBodyCount() > 0)
				{
					DEBUG_TODO("Include safety deletion since the idiot did not");
				}
				delete mWorld;
			}

			void Update(const float timestep)
			{
				const int velocityIterations = 6;
				const int positionIterations = 2;

				auto view = mRegistry.view<CCollision>();
				for (entt::entity id : view)
				{
					CCollision& col = view.get<CCollision>(id);
					col.PreStep();
				}

				mWorld->Step(timestep, velocityIterations, positionIterations);

				for (entt::entity id : view)
				{
					CCollision& col = view.get<CCollision>(id);
					col.PostStep();
				}

				//printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
			}
			
			const glm::vec2 GetGravity()
			{
				const b2Vec2 value = mWorld->GetGravity();
				return glm::vec2(value.x, value.y);
			}
			void SetGravity(const glm::vec2& value)
			{
				const b2Vec2 f_value(value.x, value.y);
				mWorld->SetGravity(f_value);
			}
		}

		void Init()
		{
			constexpr size_t capacity = static_cast<const size_t>(100);
			mEntities.reserve(capacity);
			Inputs::KeyEvents.AddFunc(KeyEventsHandle);
			Inputs::MouseButtonEvents.AddFunc(MouseButtonEventsHandle);
			Inputs::ScrollEvent.AddFunc(ScrollEventsHandle);
			Physics::Init();
		}
		void Terminate()
		{
			for (Entity* e : mEntities)
			{
				e->OnDestroyed();
			}
			mRegistry.clear();
			Physics::Terminate();
		}
		void TickUpdate()
		{
			Physics::Update(1.0f / 60.0f);
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
