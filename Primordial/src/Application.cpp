#include "pch.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#include "System/Window.hpp"
#include "System/Inputs.hpp"
#include "GUI/GUI.hpp"
#include "imgui/imgui.h"
#include "Rendering/Renderer2D.hpp"
#include "Rendering/Shader.hpp"
#include "entt/entt.hpp"
#include "ECS/Entity.hpp"
#include "Rendering/Texture.hpp"
#include "ECS/Components/CShapeRender.hpp"
#include "ECS/Components/CTextureRender.hpp"

class Other : public ECS::Entity
{
protected:
	void OnCreated() override
	{
		shape = AddComponent<CShapeRender>(Renderer2D::Shape::Quad, glm::vec4(0.2f, 0.4f, 1.0f, 1.0f));
	}
	void TargetUpdate() override
	{
		shape->Draw(transform->position, transform->rotation, transform->scale);
	}
	void TickUpdate() override
	{

	}
public:
	using Entity::Entity;
	CShapeRender* shape;
};

class Player : public ECS::Entity
{
private:
	constexpr static inline bool CheckIntersect(const float left, const float right, const float top, const float botton,
		const float other_left, const float other_right, const float other_top, const float other_botton) noexcept
	{
		if (left < other_right && 
			right > other_left &&
			top > other_botton &&
			botton < other_top)
			return true;

		return false;
	}
protected:
	void OnCreated() override
	{
		texture = new Renderer2D::Texture("res/Images/f4r44t.png");
		renderer = AddComponent<CTextureRender>(Renderer2D::Shape::Quad, texture, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	};
	void OnKeyPressed(int key) override
	{
		
	}
	void TargetUpdate() override
	{
		renderer->Draw(glm::vec2(1.0f), transform->position, transform->rotation, transform->scale);
	};
	void TickUpdate() override
	{
		glm::vec2 velocity = glm::vec2(0.0f, 0.0f);
		if (Inputs::GetPressingKey(INPUT_KEY_W)) { velocity.y += speed; }
		if (Inputs::GetPressingKey(INPUT_KEY_S)) { velocity.y -= speed; }
		if (Inputs::GetPressingKey(INPUT_KEY_A)) { velocity.x -= speed; }
		if (Inputs::GetPressingKey(INPUT_KEY_D)) { velocity.x += speed; }

		//Const values
		CTransform* other_transform = other->GetComponent<CTransform>();
		constexpr float multiplier =  50.0f * 0.5f;
		//half widths and heights
		const float width        = transform->scale.x * multiplier;
		const float height       = transform->scale.y * multiplier;
		const float other_width  = other_transform->scale.x * multiplier;
		const float other_height = other_transform->scale.y * multiplier;

		const float other_top    = other_transform->position.y + other_height;
		const float other_botton = other_transform->position.y - other_height;
		const float other_left   = other_transform->position.x - other_width;
		const float other_right  = other_transform->position.x + other_width;

		const glm::vec2 previous_position = transform->position;
		const glm::vec2 other_position = other_transform->position;

		//Mutable values and calculation
		transform->position.x += velocity.x;

		glm::vec2 position = transform->position;

		float top    = position.y + height;
		float botton = position.y - height;
		float left   = position.x - width;
		float right  = position.x + width;

		const float pos_x_differ = position.x - previous_position.x;
		if (pos_x_differ != 0.0f &&
			CheckIntersect(left, right, top, botton, other_left, other_right, other_top, other_botton))
		{
			if      (pos_x_differ > 0.0f) //Intersected when moving to right
			{
				transform->position.x = other_left - width;
			}
			else if (pos_x_differ < 0.0f) //Intersected when moving to left
			{
				transform->position.x = other_right + width;
			}
		}

		transform->position.y += velocity.y;

		position = transform->position;

		top    = position.y + height;
		botton = position.y - height;
		left   = position.x - width;
		right  = position.x + width;

		const float pos_y_differ = position.y - previous_position.y;
		if (pos_y_differ != 0.0f &&
			CheckIntersect(left, right, top, botton, other_left, other_right, other_top, other_botton))
		{
			if      (pos_y_differ > 0.0f)
			{
				transform->position.y = other_botton - height;
			}
			else if (pos_y_differ < 0.0f)
			{
				transform->position.y = other_top + height;
			}
		}
		//Pain is strength
	}
public:
	using Entity::Entity;
public:
	CTextureRender* renderer;
	Renderer2D::Texture* texture; 
	Other* other;
	constexpr static inline float speed = 10.0f;
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
	Window::GTFramebufferSizeEvent.AddFunc([](int w, int h) { Renderer2D::SetViewport(0, 0, w, h); });

	//Inputs
	Inputs::Init();

	//GUI
	GUI::Init(Window::mWindow);
	GUI::SetColorStyle(GUI::ColorStyle::Dark);

	//Scene
	ECS::Scene* scene = ECS::CreateScene();

	//Objs
	Player* player = ECS::CreateEntity<Player>(scene, "Player", 1, glm::vec2(-400.0f, 0.0f), glm::vec2(4.0f));
	Other* other = ECS::CreateEntity<Other>(scene, "Other", 0, glm::vec2(0.0f), glm::vec2(6.0f, 7.5f));
	player->other = other;

	//This is the main loop, all visible has it was supposed to be
	double nowTime = 0.0;
	double frameDiffer = 0.0;
	double last_update = 0.0;
	double last_target_update = 0.0;
	while (!Window::ShouldClose())
	{
		//Timing for target and tick update
		nowTime = Window::GetTime();
		const double delta_time = (nowTime - last_update);
		frameDiffer += (delta_time) / (1.0 / double(Window::TargetTickRate));
		last_update = nowTime;

		Inputs::CalculateMouseInput();

		//*Tick update updates by a fixed Tick-per-second value inside Window
		//*It is framerate independent
		//*Intended for game logic
		while (frameDiffer >= 1.0)
		{
			frameDiffer--;
			ECS::TickUpdateScene(scene);
		}

		//*Target update updates by a fixed Frame-per-second value inside Window
		//*It is framerate dependent has it is only limited when the application generates more than the intended frames
		//*Intended for rendering
		if ((nowTime - last_target_update) >= 1.0 / static_cast<double>(Window::TargetFPS))
		{
			last_target_update = nowTime;
			//Rendering
			Renderer2D::Clear(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), Renderer2D::BitMask::ColorBufferBit);
			ECS::TargetUpdateScene(scene);
		}
		
		//GUI handling
		GUI::NewFrame();
		
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		//std::cout << viewport->Pos.x << "," << viewport->Pos.y << std::endl;
		
		GUI::Render();
		Window::MakeContextCurrent();
		
		Window::ProcessEvents();
		Window::SwapBuffer();
	}

	ECS::EndScene(scene);

	GUI::Terminate();

	Renderer2D::Terminate();

	Inputs::Terminate();
	Window::Terminate();

	return 0;
}