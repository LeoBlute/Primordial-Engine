#pragma once

class Texture
{
public:
	Texture(const char* path);
	~Texture();

	void Bind(unsigned int slot = 0);

	unsigned int GetID() const { return mID; };

private:
	unsigned int mID;
	int mWidth;
	int mHeight;
};