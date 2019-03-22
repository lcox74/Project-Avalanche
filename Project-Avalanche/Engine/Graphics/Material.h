#ifndef MATERIAL_H
#define MATERIAL_H

#include "Shader.h"
#include "Texture.h"
#include "../Light.h"

#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
class Material
{
public:
	std::string dir = "Assets/Materials/_Default/";
	Material(const char* file = "Assets/Materials/_Default/_Default.mt", Shader shader = "Assets/Shaders/_Default.shader");

	void LoadMaterial(const char* file = "Assets/Materials/_Default/_Default.mt");
	void LoadShader(const char* file = "Assets/Shaders/_Default.shader");
	void bind(glm::vec3 cameraPos, glm::mat4 M, glm::mat4 V, glm::mat4 P, Light lights[5]);

	//virtual void customBind() = 0;

	Shader shader;

private:
	std::map<std::string, glm::vec3> colours;
	std::map<std::string, float> values;
	std::map<std::string, Texture> textures;
};

#endif