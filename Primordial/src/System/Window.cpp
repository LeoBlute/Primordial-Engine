#include "VUtils/DebugUtils.hpp"
#include "VUtils/Events.hpp"
#include "glm/glm.hpp"
#include "Window.hpp"
#include "GLFW/include/glfw3.h"

void Window::Init(const char* title, int width, int height)
{
	//Init window system
	if(!glfwInit()) [[unlikely]]
	{
		DEBUG_ERROR("GLFW could not initialize");
	}

	//use opengl 4.6
	glfwWindowHint(GLFW_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_VERSION_MINOR, 6);

#ifdef DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif // DEBUG

	//Create glfwWindow
	mWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);

	//Callback setting
	glfwSetErrorCallback(ErrorCallback);
	glfwSetMonitorCallback(MonitorCallback);
	glfwSetWindowPosCallback(mWindow, WindowPosCallback);
	glfwSetWindowSizeCallback(mWindow, WindowResizeCallback);
	glfwSetWindowCloseCallback(mWindow, WindowCloseCallback);
	glfwSetWindowRefreshCallback(mWindow, WindowRefreshCallback);
	glfwSetWindowFocusCallback(mWindow, WindowFocusCallback);
	glfwSetFramebufferSizeCallback(mWindow, FramebufferSizeCallback);
	glfwSetWindowIconifyCallback(mWindow, WindowIconifyCallback);
	glfwSetCharCallback(mWindow, WindowCharCallback);
	glfwSetKeyCallback(mWindow, KeyCallback);
	glfwSetMouseButtonCallback(mWindow, MouseButtonCallback);
	glfwSetCursorPosCallback(mWindow, CursorPosCallback);
	glfwSetCursorEnterCallback(mWindow, CursorEnterCallback);
	glfwSetScrollCallback(mWindow, ScrollCallback);
}

void Window::Terminate()
{
	//Destroy window
	glfwDestroyWindow(mWindow);
	//Terminate window system
	glfwTerminate();
}

void Window::SetTitle(const char* title)
{
	glfwSetWindowTitle(mWindow, title);
}

void Window::SetMode(Mode mode)
{
	switch (mode)
	{
	case Window::Windowed:
		break;
	case Window::Bordeless:
		break;
	case Window::Fullscreen:
		break;
	}
}

void* Window::GetProcAdress()
{
	return glfwGetProcAddress;
}

void Window::Quit()
{
	glfwSetWindowShouldClose(mWindow, 1);
}

void Window::SetVSync(bool value)
{
	glfwSwapInterval(value);
}

const glm::ivec2 Window::GetScreenSize()
{
	const GLFWvidmode* vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	return glm::ivec2(vidmode->width, vidmode->height);
}

const glm::ivec2 Window::GetSize()
{
	glm::ivec2 size;
	glfwGetWindowSize(mWindow, &size.x, &size.y);
	return glm::ivec2(size.x, size.y);
}

void Window::SetSize(const glm::ivec2& size)
{
	glfwSetWindowSize(mWindow, size.x, size.y);
}

const glm::ivec2 Window::GetPos()
{
	glm::ivec2 pos;
	glfwGetWindowPos(mWindow, &pos.x, &pos.y);
	return glm::ivec2(pos.x, pos.y);
}

void Window::SetPos(const glm::ivec2& pos)
{
	glfwSetWindowPos(mWindow, pos.x, pos.y);
}

const bool Window::GetIsOnFocus()
{
	return glfwGetWindowAttrib(mWindow, GLFW_FOCUSED);
}

void Window::SetFocus()
{
	glfwFocusWindow(mWindow);
}

const double Window::GetTime()
{
	return glfwGetTime();
}

const int Window::GetCursorMode()
{
	return glfwGetInputMode(mWindow, GLFW_CURSOR);
}

void Window::SetCursorMode(CursorMode mode)
{
	glfwSetInputMode(mWindow, GLFW_CURSOR, mode);
}

const glm::dvec2 Window::GetCursorPos()
{
	double xpos, ypos;
	glfwGetCursorPos(Window::mWindow, &xpos, &ypos);
	return glm::dvec2(xpos, ypos);
}

void Window::ProcessEvents()
{
	glfwPollEvents();
}

void Window::SwapBuffer()
{
	glfwSwapBuffers(mWindow);
}

const bool Window::ShouldClose()
{
	return glfwWindowShouldClose(mWindow);
}

void Window::MakeContextCurrent()
{
	glfwMakeContextCurrent(mWindow);
}

void Window::ErrorCallback(int error_code, const char* description)
{
	std::string error = " Error code:" + error_code;
	error.append(";\ ");
	error.append("Description:");
	error.append(description);
	DEBUG_FILE_ERROR(error);
}

void Window::MonitorCallback(GLFWmonitor* monitor, int event)
{
	
}