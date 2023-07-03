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
	inline Event<const int, const int> PosEvent; //(int xpos, int ypos)
	inline Event<const int, const int> ResizeEvent; //(int width, int height)
	inline Event<> CloseEvent; //(none)
	inline Event<> RefreshEvent; //(none)
	inline Event<const bool> FocusEvent; //(int focused)
	inline Event<const int, const int> FramebufferSizeEvent; //(int width, int height)
	inline Event<const bool> IconifyEvent; //(int iconified)
	inline Event<const unsigned int> CharEvent; //(unsigned int codepoint)
	inline Event<const int, const int, const int, const int> KeyEvent; //(int key, int scancode, int action, int mods)
	inline Event<const int, const int, const int> MouseButtonEvent; //(int button, int action, int mods)
	inline Event<const double, const double> CursorPosEvent; //(double xpos, double ypos)
	inline Event<const bool> CursorEnterEvent; //(Window* window, bool entered)
	inline Event<const double, const double> ScrollEvent; //(double xoffset, double yoffset)

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
};