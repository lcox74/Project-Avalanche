#pragma once

#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtc/quaternion.hpp"

#include "Graphics/Shader.h"

class Gameobject
{
	glm::vec3 position, rotation, scale;

	Gameobject();
	//virtual ~Gameobject();

	virtual void Draw ();
	virtual void Update (float deltaTime);

	glm::mat4 ModelMatrix()
	{
		Rotation = glm::quat(rotation);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::scale(model, scale);
		model *= glm::toMat4(Rotation);
		return model;
	}

private:
	glm::quat Rotation;

	Shader shader;
};