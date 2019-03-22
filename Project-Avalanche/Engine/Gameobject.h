#pragma once

#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtc/quaternion.hpp"

#include <iostream>

#include "Graphics/Material.h"
#include "Graphics/Model.h"

#include "Camera.h"

class Gameobject
{
protected:
	glm::quat Rotation;

	Material material;
	Model model;

	Camera *camera;

public:
	glm::vec3 position = glm::vec3(0, 0, 0), 
			  rotation = glm::vec3(0, 0, 0), 
			  scale = glm::vec3(1, 1, 1);

	Gameobject(Camera &cam, Model obj, Material mat) 
	{ 
		camera = &cam; 
		model = obj;
		material = mat;
	}
	virtual ~Gameobject() { std::cout << "Object Destroyed" << std::endl; }

	virtual void Update (float deltaTime) = 0;

	void Draw(Light lights[5])
	{
		glm::mat4 M = glm::mat4(1.0f);
		glm::mat4 V = glm::mat4(1.0f);
		glm::mat4 P;

		M = ModelMatrix();
		V = camera->GetViewMatrix();
		P = camera->GetProjectionMatrix();

		material.bind(camera->Position, M, V, P, lights);
		model.Draw(material.shader);
	}

	glm::mat4 ModelMatrix()
	{
		Rotation = glm::quat(rotation);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::scale(model, scale);
		model *= glm::toMat4(Rotation);
		return model;
	}
};