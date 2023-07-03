#include <unordered_map>
#include "glm/glm.hpp"
#include "VUtils/Events.hpp"
#include "Window.hpp"
#include "Inputs.hpp"
#include "GLFW/include/glfw3.h"

void Inputs::Init()
{
	Window::KeyEvent.AddFunc(KeyEventReceiver);
	Window::MouseButtonEvent.AddFunc(MouseButtonEventReceiver);
	Window::ScrollEvent.AddFunc(ScrollEventReceiver);
}

void Inputs::Terminate()
{
}

void Inputs::CalculateMouseInput()
{
	//Calculate cursorAxis{the speed which the mouse in a scale from -1 to 1}
	glm::vec2 screenSize = Window::GetScreenSize();
	glm::vec2 mousePos = Window::GetCursorPos();
	CursorDeltaX = float(mousePos.x - CursorPosX) / screenSize.x;
	CursorDeltaY = float(CursorPosY - mousePos.y) / screenSize.y;

	//Get current cursor position
	//Values are updated now as they are used previously has a mean to not create two CursorLasPos variables
	CursorPosX = mousePos.x;
	CursorPosY = mousePos.y;
}

void Inputs::KeyEventReceiver(const int key, const int scancode, const int action, const int mods)
{
	const Type type = static_cast<const Inputs::Type>(action);
	KeyEvents.Invoke(key, type);
	switch (type)
	{
	case Inputs::Pressed:
		Inputs::SetPressingKey(key, 1);
		break;
	case Inputs::Releaesed:
		Inputs::SetPressingKey(key, 0);
		break;
	default:
		break;
	}
}

void Inputs::MouseButtonEventReceiver(const int button, const int action, const int mods)
{
	const Type type = static_cast<const Inputs::Type>(action);
	MouseButtonEvents.Invoke(button, type);
	switch (type)
	{
	case Inputs::Pressed:
		Inputs::SetHoldingButton(button, 1);
		break;
	case Inputs::Releaesed:
		Inputs::SetHoldingButton(button, 0);
		break;
	default:
		break;
	}
}

void Inputs::ScrollEventReceiver(const double xoffset, const double yoffset)
{
	ScrollEvent.Invoke(static_cast<const float>(xoffset), static_cast<const float>(yoffset));
}

void Inputs::SetPressingKey(const int key, const bool pressing) noexcept
{
	//If key already exists in map change value if not add key and value
	const auto it = mIsPressingKey.find(key);
	if (it != mIsPressingKey.end())
	{
		mIsPressingKey[key] = pressing;
	}
	else
	{
		mIsPressingKey.emplace(key, pressing);
	}
}

const bool Inputs::GetPressingKey(const int key) noexcept
{
	return mIsPressingKey[key];

	//*Safer code just in case
	//auto it = mIsPressingKey.find(key);
	//if (it != mIsPressingKey.end())
	//{
	//	return mIsPressingKey[key];
	//}
	//else
	//{
	//	return 0;
	//}
}

void Inputs::SetHoldingButton(const int button, const bool holding) noexcept
{
	//If button already exists in map change value if not add button and value
	const auto it = mIsHoldingMouseButton.find(button);
	if (it != mIsHoldingMouseButton.end())
	{
		mIsHoldingMouseButton[button] = holding;
	}
	else
	{
		mIsHoldingMouseButton.emplace(button, holding);
	}
}

const bool Inputs::GetHoldingButton(const int button) noexcept
{
	return mIsHoldingMouseButton[button];

	//*Safer code just in case
	//auto it = mIsHoldingMouseButton.find(button);
	//if (it != mIsHoldingMouseButton.end())
	//{
	//	return mIsHoldingMouseButton[button];
	//}
	//else
	//{
	//	return 0;
	//}
}
