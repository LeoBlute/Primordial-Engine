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
	//#Life cycle
	void Init();
	void Terminate();
	void CalculateMouseInput();

	//#Connection to window events nescessary to update input values
	void KeyEvent(int key, int scancode, int action, int mods);
	void MouseButtonEvent(int button, int action, int mods);
	void ScrollEvent(double xoffset, double yoffset);

	//#Getters and setter
	//#Setters are supposed to be used inside Inputs namespace
	void SetPressingKey(int key, bool pressing);
	const bool GetPressingKey(int key);
	void SetHoldingButton(int button, bool holding);
	const bool GetHoldingButton(int button);

	//#Input Events
	inline Event<int> GTKeyPressed;
	inline Event<int> GTKeyReleased;
	inline Event<int> GTKeyRepeated;
	inline Event<int> GTMouseButtonPressed;
	inline Event<int> GTMouseButtonReleased;
	inline Event<int> GTMouseButtonRepeated;
	inline Event<float, float> GTScroll;

	//#Input values
	static inline double GTCursorPosX;
	static inline double GTCursorPosY;

	//#Delta values
	static inline float GTCursorDeltaX;
	static inline float GTCursorDeltaY;

	//#Maps of holding inputs
	static inline std::unordered_map<int, bool> gtIsPressingKey;
	static inline std::unordered_map<int, bool> gtIsHoldingMouseButton;
}