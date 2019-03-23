#include "Texture.h"

#include <glad/glad.h>

#include <algorithm>

#ifndef STB_IMAGE_IMPLEMENTATION
	#define STB_IMAGE_IMPLEMENTATION
#endif
#include "stb_image.h"

Texture::Texture(TextureType _type)
{
	type = _type;
}
Texture::Texture(const char* file, TextureType _type)
{
	LoadTexture(file, _type);
}

void Texture::LoadTexture(const char* file, TextureType _type)
{
	type = _type;
	path = file;

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	
	// Setting Image parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	//stbi_set_flip_vertically_on_load(true);

	unsigned char *data = stbi_load(file, &width, &height, &nrChannels, 4);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		
		//int num_mipmaps = 1 + floor(log2(std::max(width, height)));
		//glTexStorage2D(GL_TEXTURE_2D, num_mipmaps, GL_RGBA8, width, height);

		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture: " << file << std::endl;
	}
	stbi_image_free(data);
}

void Texture::bind(int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, id);
}