#include "Material.h"

Material::Material(const char* file, Shader shader)
{
	this->shader = shader;

	std::string tempDir = std::string(file);
	dir = tempDir.substr(0, tempDir.find_last_of("\\/"));

	LoadMaterial(file);
}

void Material::LoadMaterial(const char* file)
{
	std::ifstream stream(file);

	std::string line;

	while (getline(stream, line))
	{
		if (line.c_str()[0] == '#' || line.c_str()[0] == '\n' || line.c_str()[0] == ' ')
			continue;

		std::istringstream ss(line);

		std::string variableType;
		variableType += line.c_str()[0];
		variableType += line.c_str()[1];
		variableType += line.c_str()[2];

		std::string variableName;

		if (variableType == "vec")
		{
			float x = 255.0f, y = 255.0f, z = 255.0f;
			ss >> variableType >> variableName >> x >> y >> z;
			colours[variableName] = glm::vec3(x / 255.0f, y / 255.0f, z / 255.0f);
		} 
		else if (variableType == "val")
		{
			float x;
			ss >> variableType >> variableName >> x;
			values[variableName] = x;
		}
		else if (variableType == "tex")
		{
			std::string x;
			ss >> variableType >> variableName >> x;

			x = x.substr(1, x.size() - 2);
			std::string tempPath = dir;
			tempPath += "/";
			tempPath += x;

			textures[variableName] = Texture(tempPath.c_str());
		}
	}
}

void Material::LoadShader(const char* file) { this->shader = Shader(file); }

void Material::bind(glm::vec3 cameraPos, glm::mat4 M, glm::mat4 V, glm::mat4 P, Light lights[5])
{
	// Bind Shader
	shader.bind();

	// MVP
	shader.setMat4("model", M);
	shader.setMat4("view", V);
	shader.setMat4("projection", P);

	shader.setVec3("camPos", cameraPos);

	shader.setVec3("lights[0].position", lights[0].position);
	shader.setVec3("lights[0].colour", lights[0].colour);
	shader.setVec3("lights[1].position", lights[1].position);
	shader.setVec3("lights[1].colour", lights[1].colour);
	shader.setVec3("lights[2].position", lights[2].position);
	shader.setVec3("lights[2].colour", lights[2].colour);
	shader.setVec3("lights[3].position", lights[3].position);
	shader.setVec3("lights[3].colour", lights[3].colour);
	shader.setVec3("lights[4].position", lights[4].position);
	shader.setVec3("lights[4].colour", lights[4].colour);

	int slot = 0;
	for (auto const& x : textures)
	{
		textures[x.first].bind(slot);
		shader.setInt(x.first, slot);

		slot++;
	}

	for (auto const& x : colours)
		shader.setVec3(x.first, x.second);

	for (auto const& x : values)
		shader.setFloat(x.first, x.second);

	// For any additional binds
	//customBind();
}