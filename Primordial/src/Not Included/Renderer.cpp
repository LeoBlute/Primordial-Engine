#include "pch.h"
#include "Renderer.h"
#include "Shader.h"
#include "RenderingObject.h"
#include "Texture.h"
#include "Camera.h"
#include "System/Window.h"
#include "Glad/glad.h"

//Test values
std::vector<float>* vertices = new std::vector<float>{
	-1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
	1.0f, -1.0f, 1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	// Back face
	-1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
	-1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, -1.0f, 0.0f, 1.0f,
	1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
	// Top face
	-1.0f, 1.0f, -1.0f, 0.0f, 1.0f,
	-1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
	// Bottom face
	-1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
	1.0f, -1.0f, -1.0f, 0.0f, 1.0f,
	1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
	-1.0f, -1.0f, 1.0f, 1.0f, 0.0f,
	// Right face
	1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
	// Left face
	-1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
	-1.0f, -1.0f, 1.0f, 1.0f, 0.0f,
	-1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, -1.0f, 0.0f, 1.0f
};

//Test values
std::vector<unsigned int>* indices = new std::vector<unsigned int>{
	// Front face
	0, 1, 2,
	2, 3, 0,
	// Back face
	4, 5, 6,
	6, 7, 4,
	// Top face
	8, 9, 10,
	10, 11, 8,
	// Bottom face
	12, 13, 14,
	14, 15, 12,
	// Right face
	16, 17, 18,
	18, 19, 16,
	// Left face
	20, 21, 22,
	22, 23, 20
};

float lastX = WINDOW_WIDTH / 2;
float lastY = WINDOW_HEIGHT / 2;
bool firstMouse = true;

Shader* Renderer::mShader = nullptr;

Camera* Renderer::mCamera = nullptr;

unsigned int Renderer::mVAO = 0;

std::vector<RenderingObject*> Renderer::Objects;

void Renderer::Start(Window* window)
{
    //Checks if passed window exists
	if (!window)
	{
		DEBUG_ERROR("Window passed does not exists");
	}

    //Checks if GLAD has started exists
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		DEBUG_ERROR("GLAD could not start");
	}

#ifdef DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	int context_flags = 0;
	glGetIntegerv(GL_CONTEXT_FLAGS, &context_flags);

	if (context_flags == GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		DEBUG_INFO("Debug Context has been created");
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(DebugCallback, NULL);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);

	}
	else
	{
		DEBUG_INFO("Debug Context has not been created");
	}

#endif // DEBUG

	//Shader setup
	mShader = new Shader("res/Shaders/Vertex.shader", "res/Shaders/Fragment.shader");

	mCamera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));

	glEnable(GL_DEPTH_TEST);

	//Vertex Array Object setup
	glCreateVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	RenderingObject* ro = new RenderingObject(vertices, indices, "res/Images/bWVka.jpg");

	//Shader attributes setup
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	ResizeCallback(window->GetMemberWindow(), window->GetWidth(), window->GetHeight());
}

void Renderer::End()
{
	delete mShader;
	delete mCamera;
	//delete RenderingObjects inside Objects
	for (auto& object : Objects)
	{
		delete object;
	}

	glDeleteVertexArrays(1, &mVAO);
}

void Renderer::Render(Window* window)
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Set shader every frame
	mShader->Use();

	glm::mat4 view = glm::mat4(0.5f);
	view = mCamera->GetViewMatrix();
	mShader->SetMat4("uView", view);

	int width = window->GetWidth();
	int height = window->GetHeight();

	glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)width / (float)height, 0.01f, 100.0f);
	mShader->SetMat4("uProjection", projection);

	for(auto& object : Objects)
	{
		if (!object)
		{

		}

		glm::mat4 transform = glm::mat4(0.5f);
		transform = glm::translate(transform, object->GetPosition());
		transform = glm::rotate(transform, glm::radians(float(window->GetTime()) * 30.0f), glm::vec3(1.0f, 0.3f, 0.5f));
		mShader->SetMat4("uTransform", transform);

		object->Bind();
		mShader->SetVec4("uColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		glDrawElements(GL_TRIANGLES, object->GetIndicesCount(), GL_UNSIGNED_INT, 0);
	}
}

void Renderer::AddRO(RenderingObject* ro)
{
	Objects.push_back(ro);
}

void Renderer::ResizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void Renderer::ProcessInput(Window* window)
{
	const float deltaTime = 0.002f;
	const float cameraSpeed = 2.0f * deltaTime;

	if (window->IsKeyPressed(GLFW_KEY_W))
		mCamera->SetPosition(mCamera->GetPosition() + (mCamera->Forward() * cameraSpeed ) );
	if (window->IsKeyPressed(GLFW_KEY_S))
		mCamera->SetPosition(mCamera->GetPosition() - (mCamera->Forward() * cameraSpeed));
	if (window->IsKeyPressed(GLFW_KEY_A))
		mCamera->SetPosition(mCamera->GetPosition() - (mCamera->Right() * cameraSpeed));
	if (window->IsKeyPressed(GLFW_KEY_D))
		mCamera->SetPosition(mCamera->GetPosition() + (mCamera->Right() * cameraSpeed));

	if (window->IsKeyPressed(GLFW_KEY_ESCAPE))
		window->SetCursorMode(CursorMode::Normal);

	if (window->IsMouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE))
		window->SetCursorMode(CursorMode::Disabled);
}

void Renderer::CursorCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (glfwGetInputMode(window, GLFW_CURSOR) == CursorMode::Normal)
	{
		firstMouse = true;
		return;
	}

	if (firstMouse) // initially set to true
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	const float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	float MouseSensitivity = 0.3f;

	float yaw = mCamera->GetYaw() + (xoffset * MouseSensitivity);
	float pitch = mCamera->GetPitch() + (yoffset * MouseSensitivity);
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	mCamera->SetYaw(yaw);
	mCamera->SetPitch(pitch);
}

void Renderer::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	
}

#ifdef DEBUG
void Renderer::DebugCallback(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message, const void* userParam)
{
	DEBUG_CRIT("GL_ERROR!");
	std::cout << message << std::endl;
}
#endif // DEBUG