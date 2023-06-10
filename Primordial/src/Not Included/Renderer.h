#pragma once

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

class Shader;
class RenderingObject;
class Camera;
class Window;

class Renderer
{
public:
	static void Start(Window* window);
	static void End();

	static void Render(Window* window);

	static void AddRO(RenderingObject* ro);

	static void ResizeCallback(GLFWwindow* window, int width, int height);
	static void ProcessInput(Window* window);
	static void CursorCallback(GLFWwindow* window, double xpos, double ypos);
	static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	
private:
	static std::vector<RenderingObject*> Objects;
	static Shader* mShader;
	static Camera* mCamera;

#ifdef DEBUG
	static void DebugCallback(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message, const void* userParam);
#endif // DEBUG

	static unsigned int mVAO;
};