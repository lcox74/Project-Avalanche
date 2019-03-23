#pragma once

#define GLM_ENABLE_EXPERIMENTAL
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

	glm::mat4 modelMat = glm::mat4(1.0f);

public:
	glm::vec3 position = glm::vec3(0, 0, 0),
			  forward = glm::vec3(0, 0, 1),
			  rotation = glm::vec3(0, 0, 0), 
			  scale = glm::vec3(1, 1, 1);

	Gameobject() { }
	Gameobject(Camera &cam, Model obj, Material mat) 
	{ 
		camera = &cam; 
		model = obj;
		material = mat;
	}

	virtual void Update (float deltaTime, GLFWwindow *window) = 0;

	void setCamera (Camera &cam) { camera = &cam; }
	void setMaterial (Material mat) { material = mat; }
	void setModel (Model mod) { model = mod; }

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

	void Draw(Shader s)
	{
		glm::mat4 M = glm::mat4(1.0f);
		glm::mat4 V = glm::mat4(1.0f);
		glm::mat4 P;

		M = ModelMatrix();
		V = camera->GetViewMatrix();
		P = camera->GetProjectionMatrix();

		s.bind();
		s.setMat4("model", M);
		s.setMat4("view", V);
		s.setMat4("projection", P);

		model.Draw(s);
	}

	glm::mat4 ModelMatrix()
	{
		Rotation = glm::quat(rotation);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::scale(model, scale);
		model *= glm::toMat4(Rotation);

		modelMat = model;

		forward.x = sin(rotation.y);
		forward.y = -tan(rotation.x);
		forward.z = cos(rotation.y);

		return model;
	}
};