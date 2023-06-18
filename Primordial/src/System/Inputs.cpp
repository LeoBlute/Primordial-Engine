#include <unordered_map>
#include "glm/glm.hpp"
#include "VUtils/Events.hpp"
#include "Window.hpp"
#include "Inputs.hpp"
#include "GLFW/include/glfw3.h"

void Inputs::Init()
{
	Window::GTKeyEvent.AddFunc(KeyEvent);
	Window::GTMouseButtonEvent.AddFunc(MouseButtonEvent);
	Window::GTScrollEvent.AddFunc(ScrollEvent);
}

void Inputs::Terminate()
{
}

void Inputs::CalculateMouseInput()
{
	//Calculate cursorAxis{the speed which the mouse in a scale from -1 to 1}
	glm::vec2 screenSize = Window::GetScreenSize();
	glm::vec2 mousePos = Window::GetCursorPos();
	GTCursorDeltaX = float(mousePos.x - GTCursorPosX) / screenSize.x;
	GTCursorDeltaY = float(GTCursorPosY - mousePos.y) / screenSize.y;

	//Get current cursor position
	//Values are updated now as they are used previously has a mean to not create two CursorLasPos variables
	GTCursorPosX = mousePos.x;
	GTCursorPosY = mousePos.y;
}

void Inputs::KeyEvent(int key, int scancode, int action, int mods)
{
	switch (action)
	{
		//Key pressed
	case GLFW_PRESS:
		GTKeyPressed.Invoke(key);
		Inputs::SetPressingKey(key, 1);
		break;
		//Key released
	case GLFW_RELEASE:
		GTKeyReleased.Invoke(key);
		Inputs::SetPressingKey(key, 0);
		break;
		//Key repeated
	case GLFW_REPEAT:
		GTKeyRepeated.Invoke(key);
		break;
	default:
		break;
	}
}

void Inputs::MouseButtonEvent(int button, int action, int mods)
{
	switch (action)
	{
		//button pressed
	case GLFW_PRESS:
		GTMouseButtonPressed.Invoke(button);
		Inputs::SetHoldingButton(button, 1);
		break;
		//button released
	case GLFW_RELEASE:
		GTMouseButtonReleased.Invoke(button);
		Inputs::SetHoldingButton(button, 0);
		break;
		//button repeated
	case GLFW_REPEAT:
		GTMouseButtonRepeated.Invoke(button);
		break;
	default:
		break;
	}
}

void Inputs::ScrollEvent(double xoffset, double yoffset)
{
	GTScroll.Invoke(static_cast<float>(xoffset), static_cast<float>(yoffset));
}

void Inputs::SetPressingKey(int key, bool pressing)
{
	//If key already exists in map change value if not add key and value
	const auto it = gtIsPressingKey.find(key);
	if (it != gtIsPressingKey.end())
	{
		gtIsPressingKey[key] = pressing;
	}
	else
	{
		gtIsPressingKey.emplace(key, pressing);
	}
}

const bool Inputs::GetPressingKey(int key)
{
	return gtIsPressingKey[key];

	//*Safer code just in case
	//auto it = gtIsPressingKey.find(key);
	//if (it != gtIsPressingKey.end())
	//{
	//	return gtIsPressingKey[key];
	//}
	//else
	//{
	//	return 0;
	//}
}

void Inputs::SetHoldingButton(int button, bool holding)
{
	//If button already exists in map change value if not add button and value
	const auto it = gtIsHoldingMouseButton.find(button);
	if (it != gtIsHoldingMouseButton.end())
	{
		gtIsHoldingMouseButton[button] = holding;
	}
	else
	{
		gtIsHoldingMouseButton.emplace(button, holding);
	}
}

const bool Inputs::GetHoldingButton(int button) noexcept
{
	return gtIsHoldingMouseButton[button];

	//*Safer code just in case
	//auto it = gtIsHoldingMouseButton.find(button);
	//if (it != gtIsHoldingMouseButton.end())
	//{
	//	return gtIsHoldingMouseButton[button];
	//}
	//else
	//{
	//	return 0;
	//}
}
