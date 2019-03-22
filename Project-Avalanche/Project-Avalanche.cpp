// Project-Avalanche.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <glad/glad.h>
#include <glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Engine/AcalancheEngine.h"

#include "Project/Car.h"
#include "Project/Sphere.h"

// Initialising Functions
void framebuffer_size_callback (GLFWwindow* window, int width, int height);
void mouse_callback (GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput (GLFWwindow *window);

const unsigned int winWIDTH = 800;
const unsigned int winHEIGHT = 600;

bool wireframeMode = false;

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

// Mouse
float lastX = 400, lastY = 300;
bool firstMouse = true;


Camera Cam;

Light lights[5] = {
	Light(glm::vec3(1.5f,  2.3f,  5.2f), glm::vec3(1, 1, 1)),
	Light(glm::vec3(-1.4f,  1.3f,  5.3f), glm::vec3(1, 1, 1)),
	Light(glm::vec3(-0.7f,  2.0f,  5.3f), glm::vec3(1, 1, 1)),
	Light(glm::vec3(2.9f,  2.8f,  1.0f), glm::vec3(1, 1, 1)),
	Light(glm::vec3(1.5f,  0.4f, -2.9f), glm::vec3(1, 1, 1))
};

int main()
{
	// GLFW Setup
	// --------------------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // This is for Mac OS (Make a macro for it)
#endif

	// GLFW Window
	GLFWwindow* window = glfwCreateWindow(winWIDTH, winHEIGHT, "Avalanche", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// GLAD Setup
	// -------------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600); // Initialise Viewport
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glEnable(GL_MULTISAMPLE); // Anti-Aliasing
	glEnable(GL_DEPTH_TEST);

	glDepthFunc(GL_LESS);

	Cam = Camera(glm::vec3(0, 0, 3));
	Cam.nWidth = winWIDTH;
	Cam.nHeight = winHEIGHT;

	//Model sphere("Assets/Models/Sphere.blend");
	//Model car("Assets/Models/TOYOTA AE86 PROJECT/Drifting Game Physics Concept.obj");

	//Material mat("Assets/Materials/_Default/_Default.mt");
	//Material carMat("Assets/Materials/Car/Car.mt");

	Car player(glm::vec3(1.0f, 0.0f, 9.0f), Cam);
	Sphere ball(glm::vec3(4.0f, 0.0f, 2.0f), Cam);

	// Main Engine Loop
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Input
		processInput(window);

		// Rendering here
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glPolygonMode(GL_FRONT_AND_BACK, wireframeMode ? GL_LINE : GL_FILL);

		player.Update(deltaTime);
		player.Draw(lights);

		ball.Update(deltaTime);
		ball.Draw(lights);
		
		// Check events and swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

// Resize Event Handling
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - (float)ypos;
	lastX = (float)xpos;
	lastY = (float)ypos;

	Cam.fpsCameraLook(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (Cam.Fov >= 1.0f && Cam.Fov <= 45.0f)
		Cam.Fov -= (float)yoffset;
	if (Cam.Fov <= 1.0f)
		Cam.Fov = 1.0f;
	if (Cam.Fov >= 45.0f)
		Cam.Fov = 45.0f;
}

// Input Event Handling
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		Cam.Position += cameraSpeed * Cam.Forward;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		Cam.Position -= cameraSpeed * Cam.Forward;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		Cam.Position -= glm::normalize(glm::cross(Cam.Forward, Cam.Up)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		Cam.Position += glm::normalize(glm::cross(Cam.Forward, Cam.Up)) * cameraSpeed;
}