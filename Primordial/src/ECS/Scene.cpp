#include <iostream>
#include "glm/glm.hpp"
#include "VUtils/Events.hpp"
#include "box2d/box2d.h"
#include "Entity.hpp"
#include "Scene.hpp"
#include "System/Inputs.hpp"
#include "ECS/Components/CPhysicsBody.hpp"
#include "VUtils/DebugUtils.hpp"
#include "Components/CRenderer.hpp"
#include "Rendering/Renderer2D.hpp"

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

			class RaycastCallback : public b2RayCastCallback
			{
				virtual float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) override
				{
					if (fraction < closestFraction)
					{
						const glm::vec2 _point(point.x, point.y);
						const glm::vec2 _normal(normal.x, normal.y);
						CPhysicsBody* body = CPhysicsBody::GetFromBody(fixture->GetBody());
						result = { true, body, _point, _normal, fraction };
						//std::cout << body->GetOwner()->identity->Name << std::endl;
					}

					return 1.0f;
				}

			public:
				RaycastResult result{ false, NULL, glm::vec2(0.0f), glm::vec2(0.0f), 0.0f };
				float closestFraction = 1.0f;
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

			void Step(const float timestep)
			{
				const int velocityIterations = 6;
				const int positionIterations = 2;

				const auto view = mRegistry.view<CPhysicsBody>();
				for (const entt::entity id : view)
				{
					CPhysicsBody& body = view.get<CPhysicsBody>(id);
					if (body.mEnable)
					{
						CTransform* transform = body.mAssignedTransform;
						const glm::vec2 pos = transform->position;
						const glm::vec2 size = transform->scale;
						const b2Vec2 position = b2Vec2(pos.x, pos.y);
						const float angle = glm::radians(transform->rotation);
						constexpr float v_mult = 0.5f;
						b2Body* raw_body = static_cast<b2Body*>(body.mBody);
						b2Shape* shape = raw_body->GetFixtureList()->GetShape();
						static_cast<b2PolygonShape*>(shape)->SetAsBox(size.x * v_mult, size.y * v_mult);
						raw_body->SetTransform(position, angle);
					}
				}

				mWorld->Step(timestep, velocityIterations, positionIterations);

				for (const entt::entity id : view)
				{
					CPhysicsBody& body = view.get<CPhysicsBody>(id);
					if (body.mEnable)
					{
						CTransform* transform = body.mAssignedTransform;
						b2Body* raw_body = static_cast<b2Body*>(body.mBody);
						const b2Vec2 pos = raw_body->GetPosition();
						const glm::vec2 position = glm::vec2(pos.x, pos.y);
						const float angle = glm::degrees(raw_body->GetAngle());
						transform->position = position;
						transform->rotation = angle;
					}
				}
			}

			const RaycastResult Raycast(const glm::vec2& pointA, const glm::vec2& pointB)
			{
				RaycastCallback raycastor;
				const b2Vec2 _pointA(pointA.x, pointA.y);
				const b2Vec2 _pointB(pointB.x, pointB.y);
				mWorld->RayCast(&raycastor, _pointA, _pointB);

				return raycastor.result;
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
			mEntityMap.clear();
			Physics::Terminate();
		}
		void TickUpdate()
		{
			Physics::Step(1.0f / 60.0f);
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
			const auto view = mRegistry.view<CRenderer>();
			std::vector<CRenderer*> iterator;
			iterator.reserve(100);
			for (const entt::entity e : view)
			{
				iterator.emplace_back(&mRegistry.get<CRenderer>(e));
			}
			std::sort(iterator.begin(), iterator.end(), [](CRenderer* r1, CRenderer* r2) {
				return r1->layer < r2->layer;
				});

			for (CRenderer* r : iterator)
			{
				if (r->enable)
				{
					const CRenderer::Data data = r->data;
					const auto shape = static_cast<Renderer2D::Shape>(data.shape);
					const glm::vec2 position = r->mAssignedTransform->position;
					const float rotation = r->mAssignedTransform->rotation;
					const glm::vec2 size = r->mAssignedTransform->scale;
					Renderer2D::Draw(shape, data.texture, data.texRepetition, position,
						size, rotation, data.color);
				}
			}

		}
	}
	CIdentity* _HackToGetIdentity(ECS::Entity* e)
	{
		return e->GetComponent<CIdentity>();
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
			const UUID uuid = entity->identity->uuid;
			entity->OnDestroyed();
			Scene::mEntityMap.erase(uuid);
			Scene::mRegistry.destroy(id);
		}
	}

}