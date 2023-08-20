#include "pch.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#include "System/Window.hpp"
#include "System/Inputs.hpp"
#include "System/Audio.hpp"
#include "GUI/GUI.hpp"
#include "Rendering/Renderer2D.hpp"
#include "Rendering/Shader.hpp"
#include "entt/entt.hpp"
#include "ECS/Entity.hpp"
#include "Rendering/Texture.hpp"
#include "ECS/Components/CPhysicsBody.hpp"
#include "ECS/Components/CRenderer.hpp"
#include "ECS/Components/Component.hpp"
#include "imgui/imgui.h"

inline bool isColliding = false;

class CT : public Component
{
	COMPONENT_IMPLEMENT(CT)
protected:
	virtual void OnCreated() override
	{
		DEBUG_INFO("OnCreated");
	}
	virtual void OnDestroyed() override
	{
		DEBUG_INFO("OnDestroyed");
	}
	
};

class Other : public ECS::Entity
{
protected:
	void OnCreated() override
	{
		CRenderer::ShapeDef rdef;
		rdef.color = glm::vec4(0.2f, 0.4f, 1.0f, 1.0f);
		rdef.shape = CRenderer::Quad;
		renderer = AddRenderer(rdef, 0);
	}
public:
	using Entity::Entity;
	CRenderer* renderer;
};


class Col : public ECS::Entity
{
protected:
	void OnKeyEvent(int key, Inputs::Type type) override
	{
		if (key == INPUT_KEY_F && type == Inputs::Pressed && physicsBody->GetType() != CPhysicsBody::Static)
		{
			std::cout << "Not stoping" << std::endl;
			physicsBody->ApplyLinearImpulseAt(glm::vec2(0.0f, 300.0f), this->transform->position);
		}
	}
	void OnCreated() override
	{
		CRenderer::ShapeDef rdef;
		rdef.color = glm::vec4(0.7f, 1.0f, 0.2f, 1.0f);
		rdef.shape = CRenderer::Quad;
		renderer = AddRenderer(rdef, 0);

		CPhysicsBody::Stats stats;
		stats.density = 1.0f;
		stats.fixedRotation = false;
		stats.friction = 1.0f;
		stats.gravity = 1.0f;
		stats.isTrigger = false;
		stats.restitution = 0.3f;
		stats.restitutionThreshold = 1.0f;
		stats.type = CPhysicsBody::Type::Dynamic;
		physicsBody = AddPhysicsBody(stats);
	}
	void OnTargetUpdate() override
	{
		const glm::vec2 velocity = physicsBody->GetLinearVelocity();
	}
public:
	using Entity::Entity;
	CRenderer* renderer;
	CPhysicsBody* physicsBody;
};

class Player : public ECS::Entity
{
private:
protected:
	void OnCreated() override
	{
		texture = new Renderer2D::Texture("res/Images/f4r44t.png");
		CRenderer::TexDef rdef;
		rdef.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		rdef.shape = CRenderer::Quad;
		rdef.texRepetition = glm::vec2(1.0f, 1.0f);
		rdef.texture = texture;
		renderer = AddRenderer(rdef, 1);

		CPhysicsBody::Stats stats;
		stats.density = 1.0f;
		stats.fixedRotation = true;
		stats.friction = 1.0f;
		stats.gravity = 1.0f;
		stats.isTrigger = false;
		stats.restitution = 0.0f;
		stats.restitutionThreshold = 1.0f;
		stats.type = CPhysicsBody::Type::Dynamic;
		physicsBody = AddPhysicsBody(stats);

		AddComponent<CT>();

		floor = ECS::Scene::GetEntitiesOrComponentsByName<ECS::Entity>("Floor")[0];
		aSource = new Audio::Source("res/Audio/bleep.mp3");
	};
	void OnDestroyed() override
	{
		delete aSource;
		delete texture;
	}
	void OnKeyEvent(int key, Inputs::Type type) override
	{
		if (key == INPUT_KEY_R && type == Inputs::Pressed)
		{
			const glm::vec2 endPoint(transform->position - glm::vec2(0.0f, 4.0f));
			const RaycastResult result =
				ECS::Scene::Physics::Raycast(transform->position, floor->transform->position);
			if(result.hasHit)
				std::cout << (result.physicsBody->GetAssignedIdentity()->name) << std::endl;
		}
		if (key == INPUT_KEY_F && type == Inputs::Pressed)
		{
			renderer->data.shape = renderer->data.shape ? CRenderer::Quad : CRenderer::Triangle;
		}
		if (key == INPUT_KEY_Q && type == Inputs::Pressed)
		{
			Audio::PlayFromSource(aSource);
		}
		if ((key == INPUT_KEY_SPACE || key == INPUT_KEY_W) &&
			type == Inputs::Pressed && physicsBody->IsCollidingWith(floor))
		{
			physicsBody->ApplyLinearImpulse(glm::vec2(0.0f, 300.0f));
		}
	}
	void OnTargetUpdate() override
	{
		const glm::vec2 endPoint(transform->position - glm::vec2(0.0f, 4.0f));
		isColliding = physicsBody->IsCollidingWith(floor);
	};
	void OnTickUpdate() override
	{
		glm::vec2 velocity = glm::vec2(0.0f, 0.0f);
		if (Inputs::GetPressingKey(INPUT_KEY_A)) { velocity.x -= speed; }
		if (Inputs::GetPressingKey(INPUT_KEY_D)) { velocity.x += speed; }

		const glm::vec2 impulse = velocity * 10.0f;
		physicsBody->ApplyLinearImpulse(impulse);
	}
public:
	using Entity::Entity;
public:
	//CTextureRender* renderer;
	CPhysicsBody* physicsBody;
	ECS::Entity* floor = NULL;
	Renderer2D::Texture* texture;
	CRenderer* renderer;
	Audio::Source* aSource;
	constexpr static inline float speed = 3.0f;
};


//Press Q to test sound
//Press R to test Raycasting
//Press F to test rendering shapes
int main(int argc, char* argv)
{
	//Window system initialized with settings
	Window::Init("Main", WINDOW_WIDTH, WINDOW_HEIGHT);
	Window::SetCursorMode(Window::CursorMode::Normal);
	Window::TargetFPS = 60;
	Window::TargetTickRate = 60;
	Window::MakeContextCurrent();

	Renderer2D::Init(Window::GetProcAdress());

	//VSync, Can only set VSync after opengl setup
	Window::SetVSync(true);

	//Setting renderer initial viewport and creates a lamda so that it's change every time the window is resized);
	Renderer2D::SetViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	Window::FramebufferSizeEvent.AddFunc([](int w, int h) { Renderer2D::SetViewport(0, 0, w, h); });

	//Inputs
	Inputs::Init();

	//Audio
	Audio::Init();

	//GUI
	GUI::Init(Window::GetRawWindow());
	GUI::SetColorStyle(GUI::ColorStyle::Dark);

	//Scene
	ECS::Scene::Init();

	//Objs
	Col* floor = ECS::CreateEntity<Col>("Floor", glm::vec2(0.0f, 0.0f), 0.0f, glm::vec2(100.0f, 1.0f));
	Player* player = ECS::CreateEntity<Player>("Player", glm::vec2(-8.0f, 8.0f), 0.0f, glm::vec2(4.0f));

	floor->physicsBody->SetType(CPhysicsBody::Static);

	//This is the main loop, all visible has it was supposed to be
	double now_time = 0.0;
	double frame_differ = 0.0;
	double last_update = 0.0;
	double last_target_update = 0.0;
	while (!Window::ShouldClose())
	{
		//Timing for target and tick update
		now_time = Window::GetTime();
		const double delta_time = (now_time - last_update);
		frame_differ += (delta_time) / (1.0 / double(Window::TargetTickRate));
		last_update = now_time;

		Inputs::CalculateMouseInput();

		//*Tick update updates by a fixed Tick-per-second value inside Window
		//*It is framerate independent
		//*Intended for game logic
		while (frame_differ >= 1.0)
		{
			frame_differ--;
			ECS::Scene::TickUpdate();
		}

		//*Target update updates by a fixed Frame-per-second value inside Window
		//*It is framerate dependent has it is only limited when the application generates more than the intended frames
		//*Intended for rendering
		if ((now_time - last_target_update) >= 1.0 / static_cast<double>(Window::TargetFPS))
		{
			last_target_update = now_time;
			//Rendering
			Renderer2D::Clear(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), Renderer2D::BitMask::ColorBufferBit);
			ECS::Scene::TargetUpdate();
		}
		
		//GUI handling
		GUI::NewFrame();
		
		{
			ImGui::Begin("Is player colliding with floor");
			ImGui::Checkbox("IS??:", &isColliding);
			ImGui::End();
		}

		GUI::Render();
		Window::MakeContextCurrent();
		
		Window::ProcessEvents();
		Window::SwapBuffer();
	}

	ECS::Scene::Terminate();

	GUI::Terminate();

	Audio::Terminate();

	Renderer2D::Terminate();

	Inputs::Terminate();
	Window::Terminate();

	DEBUG_INFO("Prorperly Quit");

	return 0;
}