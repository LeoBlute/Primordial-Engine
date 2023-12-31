#pragma once

template<typename... Args>
class Event;

/* Printable keys */
#define INPUT_KEY_SPACE              32
#define INPUT_KEY_APOSTROPHE         39  /* ' */
#define INPUT_KEY_COMMA              44  /* , */
#define INPUT_KEY_MINUS              45  /* - */
#define INPUT_KEY_PERIOD             46  /* . */
#define INPUT_KEY_SLASH              47  /* / */
#define INPUT_KEY_0                  48
#define INPUT_KEY_1                  49
#define INPUT_KEY_2                  50
#define INPUT_KEY_3                  51
#define INPUT_KEY_4                  52
#define INPUT_KEY_5                  53
#define INPUT_KEY_6                  54
#define INPUT_KEY_7                  55
#define INPUT_KEY_8                  56
#define INPUT_KEY_9                  57
#define INPUT_KEY_SEMICOLON          59  /* ; */
#define INPUT_KEY_EQUAL              61  /* = */
#define INPUT_KEY_A                  65
#define INPUT_KEY_B                  66
#define INPUT_KEY_C                  67
#define INPUT_KEY_D                  68
#define INPUT_KEY_E                  69
#define INPUT_KEY_F                  70
#define INPUT_KEY_G                  71
#define INPUT_KEY_H                  72
#define INPUT_KEY_I                  73
#define INPUT_KEY_J                  74
#define INPUT_KEY_K                  75
#define INPUT_KEY_L                  76
#define INPUT_KEY_M                  77
#define INPUT_KEY_N                  78
#define INPUT_KEY_O                  79
#define INPUT_KEY_P                  80
#define INPUT_KEY_Q                  81
#define INPUT_KEY_R                  82
#define INPUT_KEY_S                  83
#define INPUT_KEY_T                  84
#define INPUT_KEY_U                  85
#define INPUT_KEY_V                  86
#define INPUT_KEY_W                  87
#define INPUT_KEY_X                  88
#define INPUT_KEY_Y                  89
#define INPUT_KEY_Z                  90
#define INPUT_KEY_LEFT_BRACKET       91  /* [ */
#define INPUT_KEY_BACKSLASH          92  /* \ */
#define INPUT_KEY_RIGHT_BRACKET      93  /* ] */
#define INPUT_KEY_GRAVE_ACCENT       96  /* ` */
#define INPUT_KEY_WORLD_1            161 /* non-US #1 */
#define INPUT_KEY_WORLD_2            162 /* non-US #2 */

namespace Inputs {
	enum Type
	{
		Pressed = 1,
		Releaesed = 0,
		Repeated = 2
	};

	#pragma region Life cycle
	void Init();
	void Terminate();
	void CalculateMouseInput();
	#pragma endregion

	#pragma region Connection to window events nescessary to update input values
	void KeyEventReceiver(const int key, const int scancode, const int action, const int mods);
	void MouseButtonEventReceiver(const int button, const int action, const int mods);
	void ScrollEventReceiver(const double xoffset, const double yoffset);
	#pragma endregion

	#pragma region Input Events
	inline Event<const int, const Inputs::Type> KeyEvents;
	inline Event<const int, const Inputs::Type> MouseButtonEvents;
	inline Event<const float, const float> ScrollEvent;
	#pragma endregion

	#pragma region Input values
	inline double CursorPosX;
	inline double CursorPosY;
	#pragma endregion

	#pragma region regionDelta values
	inline float CursorDeltaX;
	inline float CursorDeltaY;
	#pragma endregion

	#pragma region Maps of holding inputs
	inline std::unordered_map<int, bool> mIsPressingKey;
	inline std::unordered_map<int, bool> mIsHoldingMouseButton;
	#pragma endregion

	#pragma region Getters and setters
	//#Internal
	void SetPressingKey(const int key, const bool pressing) noexcept;
	const bool GetPressingKey(const int key) noexcept;
	//#Internal
	void SetHoldingButton(const int button, const bool holding) noexcept;
	const bool GetHoldingButton(const int button) noexcept;
	#pragma endregion
}