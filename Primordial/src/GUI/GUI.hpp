#pragma once

namespace GUI {
	enum ColorStyle
	{
		Classic = 0,
		Dark = 1,
		Light = 2
	};

	//#Class which should hold a element life cycle and attributes
	class Element
	{
	public:
		Element();
		~Element();
		virtual void Draw() = 0;
	};

	//#Life cycle
	void Init(void* window); //GLFWwindow nescessary for ImGui_ImplGlfw_InitForOpenGL
	void Terminate();
	void NewFrame();
	void Render();

	//#Attributes Settings and Gettings
	void SetColorStyle(ColorStyle colorStyle);

	//#Internal variable
	static inline std::vector<GUI::Element*> mElementsList;
}