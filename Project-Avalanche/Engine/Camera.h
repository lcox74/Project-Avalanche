#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <iostream>

enum ViewMode
{
	ORTHOGRAPHIC,
	PERSPECTIVE
};

// Default Values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SENSITIVITY = 0.1f;
const float FOV = 45.0f;

class Camera
{
public:
	glm::vec3 Position;
	glm::vec3 Forward;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	float Yaw;
	float Pitch;
	float Roll; // Not Implemented Yet

	float MouseSensitivity = 0.1f;

	float Fov;
	float Near = 0.1f;
	float Far = 1000.0f;

	ViewMode viewMode = ViewMode::PERSPECTIVE;

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix(float width, float height, float left = 0.0f, float bottom = 0.0f);

	// In-built Camera Movement
	void fpsCameraLook(float deltaX, float deltaY, bool constrainPitch = true);

	void updateCameraVectors();

};

#endif