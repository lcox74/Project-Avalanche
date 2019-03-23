#pragma once

#include "glm/glm.hpp"

#include "../Engine/AcalancheEngine.h"

class Car : public Gameobject
{
public:
	Car() 
		: Gameobject() 
	{}

	Car(glm::vec3 pos,
		Camera &cam, 
		Model obj = Model("Assets/Models/TOYOTA AE86 PROJECT/Drifting Game Physics Concept.obj"), 
		Material mat = Material("Assets/Materials/Car/Car.mt"))
		: Gameobject(cam, obj, mat)
	{
		position = pos;
	}

	void Update(float deltaTime, GLFWwindow *window) override
	{
		float speed = 9.0f * deltaTime;
		float rotationSpeed = 1.7f * deltaTime;

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			position += speed * forward;
		} 
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			position -= speed * forward;
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			rotation -= glm::vec3(0, rotationSpeed, 0);
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			rotation += glm::vec3(0, rotationSpeed, 0);
		}
	}
};

