#include "Glad/glad.h"
#include "glm/glm.hpp"
#include "stb/stb_image.h"
#include "VUtils/DebugUtils.h"
#include "Texture.hpp"

namespace Renderer2D {
	Texture::Texture(const char* path)
	{

		//Adjust settings to load image correctly
		stbi_set_flip_vertically_on_load(true);

		//Stores temporary image data loaded image from file
		unsigned char* data = stbi_load(path, &mSize.x, &mSize.y, NULL, 4);

		if (data)
		{
			//Create texture with appropiate fromat and size
			glCreateTextures(GL_TEXTURE_2D, 1, &mID);
			glTextureStorage2D(mID, 1, GL_RGB8, mSize.x, mSize.y);

			//Set texture parameters
			glTextureParameteri(mID, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(mID, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTextureParameteri(mID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTextureParameteri(mID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			//Upload image to texture
			glTextureSubImage2D(mID, 0, 0, 0, mSize.x, mSize.y, GL_RGBA, GL_UNSIGNED_BYTE, data);

			//Delete temporary image data
			stbi_image_free(data);
		}
		else
		{
			DEBUG_ERROR("Texture path invalid");
		}
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &mID);
	}

	void Texture::Bind()
	{
		glBindTexture(GL_TEXTURE_2D, mID);
	}
}