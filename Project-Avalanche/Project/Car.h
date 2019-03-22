#pragma once

#include "glm/glm.hpp"

#include "../Engine/AcalancheEngine.h"

class Car : public Gameobject
{
public:
	Car(glm::vec3 pos,
		Camera &cam, 
		Model obj = Model("Assets/Models/TOYOTA AE86 PROJECT/Drifting Game Physics Concept.obj"), 
		Material mat = Material("Assets/Materials/Car/Car.mt"))
		: Gameobject(cam, obj, mat)
	{
		position = pos;
	}

	void Update(float deltaTime) override 
	{
		rotation += glm::vec3(0, deltaTime, 0);
	}
};

