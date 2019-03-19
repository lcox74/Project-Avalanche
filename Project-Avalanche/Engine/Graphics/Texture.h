#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <string>

enum TextureType
{
	DIFFUSE,
	SPECULAR,
	NORMAL,
	HEIGHT
};

class Texture
{
public:
	unsigned int id;
	TextureType type;
	std::string path;

	Texture(TextureType _type = TextureType::DIFFUSE);
	Texture(const char* file, TextureType _type = TextureType::DIFFUSE);

	void LoadTexture(const char* file, TextureType _type = TextureType::DIFFUSE);

	void bind(int slot = 0);

	int width, height, nrChannels;
};

#endif