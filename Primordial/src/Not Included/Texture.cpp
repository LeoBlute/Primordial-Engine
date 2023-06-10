#include "pch.h"
#include "Texture.h"

Texture::Texture(const char* path)
{
	int format = GL_RGB;

	int channels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path, &mWidth, &mHeight, &channels, 0);

	if (data)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &mID);
		glTextureStorage2D(mID, 1, GL_RGB8, mWidth, mHeight);

		glTextureParameteri(mID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(mID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureParameteri(mID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(mID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureSubImage2D(mID, 0, 0, 0, mWidth, mHeight, format, GL_UNSIGNED_BYTE, data);
		//glGenerateTextureMipmap(mID);
	}
	else
	{
		DEBUG_CRIT("Could not load data or path is invalid");
	}

	stbi_image_free(data);
}

void Texture::Bind(unsigned int slot)
{
	glBindTextureUnit(slot, mID);
}

Texture::~Texture()
{
	glDeleteTextures(1, &mID);
}
