#include "pch.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#include "System/Window.hpp"
#include "System/Inputs.hpp"
#include "GUI/GUI.hpp"
#include "Rendering/Renderer2D.hpp"
#include "Rendering/Shader.hpp"
#include "entt/entt.hpp"
#include "ECS/Entity.hpp"
#include "Rendering/Texture.hpp"
#include "ECS/Components/CShapeRender.hpp"
#include "ECS/Components/CTextureRender.hpp"
#include "ECS/Components/CPhysicsBody.hpp"
#include "imgui/imgui.h"

inline bool isColliding = false;

class Other : public ECS::Entity
{
protected:
	void OnCreated() override
	{
		shape = AddComponent<CShapeRender>(Renderer2D::Shape::Quad, glm::vec4(0.2f, 0.4f, 1.0f, 1.0f));
	}
	void TargetUpdate() override
	{
		shape->Draw(Transform->position, Transform->rotation, Transform->scale);
	}
	void TickUpdate() override
	{

	}
public:
	using Entity::Entity;
	CShapeRender* shape;
};

class Col : public ECS::Entity
{
protected:
	void OnKeyEvent(int key, Inputs::Type type) override
	{
		if (key == INPUT_KEY_F && type == Inputs::Pressed && physicsBody->GetType() != CPhysicsBody::Static)
		{
			std::cout << "Not stoping" << std::endl;
			physicsBody->ApplyLinearImpulseAt(glm::vec2(0.0f, 300.0f), this->Transform->position);
		}
	}
	void OnCreated() override
	{
		shape = AddComponent<CShapeRender>(Renderer2D::Shape::Quad, glm::vec4(0.7f, 1.0f, 0.2f, 1.0f));
		CPhysicsBody::Stats stats;
		stats.density = 1.0f;
		stats.fixedRotation = false;
		stats.friction = 1.0f;
		stats.gravity = 1.0f;
		stats.isTrigger = false;
		stats.restitution = 0.3f;
		stats.restitutionThreshold = 1.0f;
		stats.type = CPhysicsBody::Type::Dynamic;
		physicsBody = AddComponent<CPhysicsBody>(this, stats);
	}
	void TargetUpdate() override
	{
		shape->Draw(Transform->position, Transform->rotation, Transform->scale);
		const glm::vec2 velocity = physicsBody->GetLinearVelocity();
		//std::cout << velocity.x << "," << velocity.y << std::endl;
		//std::cout << box->GetAngularVelocity() << std::endl;
	}
public:
	using Entity::Entity;
	CShapeRender* shape;
	CPhysicsBody* physicsBody;
};

class Player : public ECS::Entity
{
private:
protected:
	void OnCreated() override
	{
		texture = new Renderer2D::Texture("res/Images/f4r44t.png");
		renderer = AddComponent<CTextureRender>(Renderer2D::Shape::Quad, texture, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		CPhysicsBody::Stats stats;
		stats.density = 1.0f;
		stats.fixedRotation = true;
		stats.friction = 1.0f;
		stats.gravity = 1.0f;
		stats.isTrigger = false;
		stats.restitution = 0.3f;
		stats.restitutionThreshold = 1.0f;
		stats.type = CPhysicsBody::Type::Dynamic;
		physicsBody = AddComponent<CPhysicsBody>(this, stats);
	};
	void OnKeyEvent(int key, Inputs::Type type) override
	{
		if (key == INPUT_KEY_SPACE && type == Inputs::Pressed)
		{
			renderer->Shape = renderer->Shape ? Renderer2D::Quad : Renderer2D::Triangle;
		}
		if (key == INPUT_KEY_F && type == Inputs::Pressed)
		{
			physicsBody->ApplyLinearImpulse(glm::vec2(0.0f, 300.0f));
		}
	}
	void TargetUpdate() override
	{
		renderer->Draw(glm::vec2(1.0f), Transform->position, Transform->rotation, Transform->scale);
		isColliding = physicsBody->IsCollidingWith(other);
	};
	void TickUpdate() override
	{
		glm::vec2 velocity = glm::vec2(0.0f, 0.0f);
		if (Inputs::GetPressingKey(INPUT_KEY_W)) { velocity.y += speed; }
		if (Inputs::GetPressingKey(INPUT_KEY_A)) { velocity.x -= speed; }
		if (Inputs::GetPressingKey(INPUT_KEY_D)) { velocity.x += speed; }

		const glm::vec2 impulse = velocity * 10.0f;
		physicsBody->ApplyLinearImpulse(impulse);
	}
public:
	using Entity::Entity;
public:
	CTextureRender* renderer;
	CPhysicsBody* physicsBody;
	CPhysicsBody* other;
	Renderer2D::Texture* texture;
	constexpr static inline float speed = 3.0f;
};

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

	//GUI
	GUI::Init(Window::GetRawWindow());
	GUI::SetColorStyle(GUI::ColorStyle::Dark);

	//Scene
	ECS::Scene::Init();

	//Objs
	Player* player = ECS::CreateEntity<Player>("Player", 1, glm::vec2(-8.0f, 8.0f), 0.0f, glm::vec2(4.0f));
	//Col * col1 = ECS::CreateEntity<Col>("col01", 1, glm::vec2(1.0f, 8.0f), 37.0f, glm::vec2(4.0f));
	//Col* col2 = ECS::CreateEntity<Col>("Collision", 1, glm::vec2(3.0f, 50.0f), 37.0f, glm::vec2(4.0f));
	Col* floor = ECS::CreateEntity<Col>("Floor", 1, glm::vec2(0.0f, 0.0f), 0.0f, glm::vec2(100.0f, 1.0f));

	player->other = floor->physicsBody;
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

	Renderer2D::Terminate();

	Inputs::Terminate();
	Window::Terminate();

	return 0;
}