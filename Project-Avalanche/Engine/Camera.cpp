#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
	: Forward(glm::vec3(0.0f, 0.0f, -1.0f)), Fov(FOV)
{
	Position = position;
	Up = up;
	Yaw = yaw;
	Pitch = pitch;
	updateCameraVectors();
}
Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
	: Forward(glm::vec3(0.0f, 0.0f, -1.0f)), Fov(FOV)
{
	Position = glm::vec3(posX, posY, posZ);
	WorldUp = glm::vec3(upX, upY, upZ);
	Yaw = yaw;
	Pitch = pitch;
	updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() 
{ 
	return glm::lookAt(Position, Position + Forward, WorldUp);
}

glm::mat4 Camera::GetProjectionMatrix()
{
	switch (viewMode)
	{
	case ViewMode::PERSPECTIVE:
		return glm::perspective(glm::radians(Fov), (float)nWidth / (float)nHeight, Near, Far);
	//case ViewMode::ORTHOGRAPHIC:
		//return glm::ortho(left, width, bottom, height, Near, Far);
	//	break;
	default:
		return glm::perspective(glm::radians(Fov), (float)nWidth / (float)nHeight, Near, Far);
	}
}

void Camera::setViewPort(int width, int height)
{
	nWidth = width;
	nHeight = height;
	glViewport(0, 0, width, height);
}

void Camera::fpsCameraLook(float deltaX, float deltaY, bool constrainPitch)
{
	deltaX *= MouseSensitivity;
	deltaY *= MouseSensitivity;

	Yaw += deltaX;
	Pitch += deltaY;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}

	updateCameraVectors();
}

void Camera::updateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(Pitch)) * cos(glm::radians(Yaw));
	front.y = sin(glm::radians(Pitch));
	front.z = cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
	Forward = glm::normalize(front);

	// Also re-calculate the Right and Up vector
	Right = glm::normalize(glm::cross(Forward, WorldUp));
	Up = glm::normalize(glm::cross(Right, Forward));
}

void Camera::lookAt(glm::vec3 target)
{
	Forward = glm::normalize(target - Position);
}