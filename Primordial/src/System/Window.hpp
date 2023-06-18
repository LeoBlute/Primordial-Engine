#pragma once

struct GLFWwindow;
struct GLFWmonitor;

template<typename... Args>
class Event;

namespace Window{
	enum CursorMode
	{
		Normal = 0x00034001,
		Hidden = 0x00034002,
		Disabled = 0x00034003
	};

	//Useless
	enum CallbackType
	{
		Error = 0,
		Monitor = 1,
		WindowPos = 2,
		WindowSize = 3,
		WindowClose = 4,
		WindowRefresh = 5,
		WindowFocus = 6,
		FramebufferSize = 8,
		WindowIconify = 7,
		Character = 9,
		Key = 10,
		MouseButton = 11,
		CursorPos = 12,
		CursorEnter = 13,
		Scroll = 14
	};

	enum MouseButton
	{
		Left = 0,
		Right = 1,
		Middle = 2,
		Last = 7
	};

	enum Mode
	{
		Windowed,
		Bordeless,
		Fullscreen
	};

	//#Events here are associated glfw callbacks
	inline Event<int, int> GTPosEvent; //(int xpos, int ypos)
	inline Event<int, int> GTResizeEvent; //(int width, int height)
	inline Event GTCloseEvent; //(none)
	inline Event GTRefreshEvent; //(none)
	inline Event<bool> GTFocusEvent; //(int focused)
	inline Event<int, int> GTFramebufferSizeEvent; //(int width, int height)
	inline Event<bool> GTIconifyEvent; //(int iconified)
	inline Event<unsigned int> GTCharEvent; //(unsigned int codepoint)
	inline Event<int, int, int, int> GTKeyEvent; //(int key, int scancode, int action, int mods)
	inline Event<int, int, int> GTMouseButtonEvent; //(int button, int action, int mods)
	inline Event<double, double> GTCursorPosEvent; //(double xpos, double ypos)
	inline Event<bool> GTCursorEnterEvent; //(Window* window, bool entered)
	inline Event<double, double> GTScrollEvent; //(double xoffset, double yoffset)

	//#Internal variables, intended to not be used outside of namespace
	inline GLFWwindow* mWindow;

	//Internal and External variables
	inline unsigned short int TargetFPS = 60;
	inline unsigned short int TargetTickRate = 60;

	//#Life cycle
	void Init(const char* title, int width, int height);
	void Terminate();
	void ProcessEvents();
	void SwapBuffer();
	const bool ShouldClose();
	void MakeContextCurrent();

	//#Getters and setters related to the window
	void SetTitle(const char* title);
	void SetMode(Mode mode);
	void* GetProcAdress();
	void Quit();
	const glm::ivec2 GetScreenSize();
	const glm::ivec2 GetSize();
	void SetSize(const glm::ivec2& size);
	const glm::ivec2 GetPos();
	void SetPos(const glm::ivec2& pos);
	const bool GetIsOnFocus();
	void SetFocus();
	const double GetTime();

	//#Getters and setters not related to the window
	const int GetCursorMode();
	void SetVSync(bool value);
	void SetCursorMode(CursorMode mode);
	const glm::dvec2 GetCursorPos();

	//#Callbacks
	void ErrorCallback(int error_code, const char* description);
	void MonitorCallback(GLFWmonitor* monitor, int event);
	inline void WindowPosCallback(GLFWwindow* window, int xpos, int ypos)
	{
		GTPosEvent.Invoke(xpos, ypos);
	}
	inline void WindowResizeCallback(GLFWwindow* window, int width, int height)
	{
		GTResizeEvent.Invoke(width, height);
	}
	inline void WindowCloseCallback(GLFWwindow* window)
	{
		GTCloseEvent.Invoke();
	}
	inline void WindowRefreshCallback(GLFWwindow* window)
	{
		GTRefreshEvent.Invoke();
	}
	inline void WindowFocusCallback(GLFWwindow* window, int focused)
	{
		GTFocusEvent.Invoke(focused);
	}
	inline void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		GTFramebufferSizeEvent.Invoke(width, height);
	}
	inline void WindowIconifyCallback(GLFWwindow* window, int iconified)
	{
		GTIconifyEvent.Invoke(iconified);
	}
	inline void WindowCharCallback(GLFWwindow* window, unsigned int codepoint)
	{
		GTCharEvent.Invoke(codepoint);
	}
	inline void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		GTKeyEvent.Invoke(key, scancode, action, mods);
	}
	inline void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		GTMouseButtonEvent.Invoke(button, action, mods);
	}
	inline void CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
	{
		GTCursorPosEvent.Invoke(xpos, ypos);
	}
	inline void CursorEnterCallback(GLFWwindow* window, int entered)
	{
		GTCursorEnterEvent.Invoke(entered);
	}
	inline void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		GTScrollEvent.Invoke(xoffset, yoffset);
	}

};