#include <iostream>
#include "glm/glm.hpp"
#include "VUtils/Events.hpp"
#include "box2d/box2d.h"
#include "Entity.hpp"
#include "Scene.hpp"
#include "System/Inputs.hpp"
#include "Rendering/Renderer2D.hpp"
#include "ECS/Components/CPhysicsBody.hpp"
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
			inline b2World* mWorld;

			class ContactListener : public b2ContactListener
			{
				void BeginContact(b2Contact* contact) override
				{
					CPhysicsBody* a = CPhysicsBody::GetFromBody(contact->GetFixtureA()->GetBody());
					CPhysicsBody* b = CPhysicsBody::GetFromBody(contact->GetFixtureB()->GetBody());

					a->SetCollidingWith(b, true);
					b->SetCollidingWith(a, true);
				}

				void EndContact(b2Contact* contact) override
				{
					CPhysicsBody* a = CPhysicsBody::GetFromBody(contact->GetFixtureA()->GetBody());
					CPhysicsBody* b = CPhysicsBody::GetFromBody(contact->GetFixtureB()->GetBody());

					a->SetCollidingWith(b, false);
					b->SetCollidingWith(a, false);
				}
			};
			inline ContactListener* contactor;

			void Init()
			{
				const b2Vec2 pressure(0.0f, -50.0f);
				mWorld = new b2World(pressure);
				contactor = new ContactListener();
				mWorld->SetContactListener(contactor);
			}
			void Terminate()
			{
				if (mWorld->GetBodyCount() > 0)
				{
					DEBUG_TODO("Include safety deletion since the idiot did not");
				}
				delete mWorld;
				delete contactor;
			}

			void Update(const float timestep)
			{
				const int velocityIterations = 6;
				const int positionIterations = 2;

				const auto view = mRegistry.view<CPhysicsBody>();
				for (const entt::entity id : view)
				{
					CPhysicsBody& col = view.get<CPhysicsBody>(id);
					col.PreStep();
				}

				mWorld->Step(timestep, velocityIterations, positionIterations);

				for (const entt::entity id : view)
				{
					CPhysicsBody& col = view.get<CPhysicsBody>(id);
					col.PostStep();
				}
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
			void* GetRawWorld()
			{
				return mWorld;
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
	CIdentity* _HackToGetIdentity(ECS::Entity* e)
	{
		return e->GetComponent<CIdentity>();
	}
	const bool LayerAlgorithm(Entity* a, Entity* b)
	{
		return a->GetComponent<CIdentity>()->Layer < b->GetComponent<CIdentity>()->Layer;
	}
	void _HackOnCreated(Entity* entity)
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
