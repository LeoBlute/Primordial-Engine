#pragma once

#include <vector>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "GUI.hpp"

void GUI::Init(void* window)
{
	GLFWwindow* _window = static_cast<GLFWwindow*>(window);
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;
	// Setup Dear ImGui style

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(_window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
}

void GUI::Terminate()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void GUI::NewFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void GUI::Render()
{
	//Despite calling "Draw" this just read all the elements attributes for the rendering part
	for (GUI::Element* element : mElementsList)
	{
		element->Draw();
	}

	//Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	//Allow elements(imgui windows) to be dragged outside of the main Window context bounds
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}

void GUI::SetColorStyle(ColorStyle colorStyle)
{
	switch (colorStyle)
	{
	case Classic:
		ImGui::StyleColorsClassic();
		break;
	case Dark:
		ImGui::StyleColorsDark();
		break;
	case Light:
		ImGui::StyleColorsLight();
		break;
	}
}

GUI::Element::Element()
{
	GUI::mElementsList.push_back(this);
}

GUI::Element::~Element()
{
	auto it = std::find(GUI::mElementsList.begin(), GUI::mElementsList.end(), this);
	if (it != GUI::mElementsList.end())
	{
		GUI::mElementsList.erase(it);
	}
}
