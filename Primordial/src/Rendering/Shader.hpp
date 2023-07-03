#pragma once

namespace Renderer2D {
	class Shader
	{
	private:
		unsigned int mID;
	public:
		unsigned int GetId() { return mID; };

		Shader(const char* vertexShaderPath, const char* fragmentShaderPath);
		~Shader();

		void Use();

		void SetBool(const char* name, bool value);
		void SetVec2(const char* name, const glm::vec2& value);
		void SetVec3(const char* name, const glm::vec3& value);
		void SetVec4(const char* name, const glm::vec4& value);
		void SetMat4(const char* name, const glm::mat4& value);

	private:
		static const unsigned int CreateShader(const char* code, int type);
	};
}