// Project-Avalanche.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <glad/glad.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Engine/Camera.h"

#include "Engine/Graphics/Shader.h"
#include "Engine/Graphics/Texture.h"
#include "Engine/Graphics/Model.h"

// Initialising Functions
void framebuffer_size_callback (GLFWwindow* window, int width, int height);
void mouse_callback (GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput (GLFWwindow *window);

const unsigned int winWIDTH = 800;
const unsigned int winHEIGHT = 600;

bool wireframeMode = false;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

// Mouse
float lastX = 400, lastY = 300;
bool firstMouse = true;
float yaw, pitch;
float fov = 45.0f;

Camera Cam;

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

	Shader modelShader("Assets/Shaders/Basic.shader");
	Model monkeyModel("Assets/Models/Map.obj");
	

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

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection;

		//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		projection = Cam.GetProjectionMatrix((float)winWIDTH, (float)winHEIGHT);
		view = Cam.GetViewMatrix();

		model = glm::translate(model, glm::vec3(3.0f, -0.35f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));

		modelShader.bind();

		modelShader.setMat4("projection", projection);
		modelShader.setMat4("view", view);
		modelShader.setMat4("model", model);

		modelShader.setVec3("viewPos", Cam.Position);
		modelShader.setVec3("light.position", glm::vec3(3.0f, 3.5f, 2.0f));

		modelShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
		modelShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
		modelShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		modelShader.setFloat("material.shininess", 32.0f);

		modelShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
		modelShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f); // darken the light a bit to fit the scene
		modelShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

		monkeyModel.Draw(modelShader);
		
		
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