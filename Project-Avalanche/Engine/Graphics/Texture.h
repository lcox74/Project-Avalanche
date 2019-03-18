#ifndef TEXTURE_H
#define TEXTURE_H

class Texture
{
public:
	unsigned int id;

	Texture();
	Texture(const char* file);

	void LoadTexture(const char* file);

	void bind(int slot = 0);

	int width, height, nrChannels;
};

#endif