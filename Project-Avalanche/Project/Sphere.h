#pragma once

#include "glm/glm.hpp"

#include "../Engine/AcalancheEngine.h"

class Sphere : public Gameobject
{
public:
	Sphere(glm::vec3 pos,
		Camera &cam,
		Model obj = Model("Assets/Models/Sphere.blend"),
		Material mat = Material("Assets/Materials/_Default/_Default.mt"))
		: Gameobject(cam, obj, mat)
	{
		position = pos;
	}

	void Update(float deltaTime, GLFWwindow *window) override
	{
		//rotation += glm::vec3(0, deltaTime, 0);
	}
};

