#pragma once

#include "glm/glm.hpp"

class Light
{
public:
	glm::vec3 position, direction;
	glm::vec3 colour;

	Light(glm::vec3 pos = glm::vec3(0, 0, 0), glm::vec3 col = glm::vec3(1, 1, 1), glm::vec3 dir = glm::vec3(0, -1, 0))
		: position(pos), direction(dir), colour(col) { }
};