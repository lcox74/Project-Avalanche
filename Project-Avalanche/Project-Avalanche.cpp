// Project-Avalanche.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <glad/glad.h>
#include <glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"

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

void CameraFollowTarget(Car target, float height, float distance, float t);

const unsigned int winWIDTH = 800;
const unsigned int winHEIGHT = 600;

bool wireframeMode = false;
bool normalsMode = false;
bool unlockCam = false;

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

// Mouse
float lastX = 400, lastY = 300;
bool firstMouse = true;

Camera Cam;

Light lights[5] = {
	Light(glm::vec3(1.5f,  3.3f,  5.2f), glm::vec3(1, 1, 1)),
	Light(glm::vec3(-1.4f,  4.3f,  5.3f), glm::vec3(1, 1, 1)),
	Light(glm::vec3(-0.7f,  3.0f,  5.3f), glm::vec3(1, 1, 1)),
	Light(glm::vec3(2.9f,  2.8f,  1.0f), glm::vec3(1, 1, 1)),
	Light(glm::vec3(1.5f,  3.4f, -2.9f), glm::vec3(1, 1, 1))
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

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glEnable(GL_MULTISAMPLE); // Anti-Aliasing

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// ImGui Setup
	// -------------------------------------------
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	Cam = Camera(glm::vec3(0, 0, 3));
	Cam.nWidth = winWIDTH;
	Cam.nHeight = winHEIGHT;

	//Model sphere("Assets/Models/Sphere.blend");
	//Model car("Assets/Models/TOYOTA AE86 PROJECT/Drifting Game Physics Concept.obj");

	//Material mat("Assets/Materials/_Default/_Default.mt");
	//Material carMat("Assets/Materials/Car/Car.mt");

	Sphere ball(glm::vec3(0.0f, -0.1f, 0.0f), Cam, Model("Assets/Models/Cube.obj"));
	ball.scale = glm::vec3(40, 0.1, 40);
	Car player = Car(glm::vec3(1.0f, 0.0f, 9.0f), Cam);
	
	Shader normalShader("Assets/Shaders/normal_visualization.shader");

	std::vector<float> deltaTimeVec;
	size_t maxTimeSmooth = 60;

	// Main Engine Loop
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Input
		processInput(window);

		if (!unlockCam)
			CameraFollowTarget(player, 4.5f, 10, 5.0f * deltaTime);

		// Rendering here
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glPolygonMode(GL_FRONT_AND_BACK, wireframeMode ? GL_LINE : GL_FILL);

		player.Update(deltaTime, window);
		player.Draw(lights);
		if (normalsMode)
			player.Draw(normalShader);

		ball.Update(deltaTime, window);
		ball.Draw(lights);
		if (normalsMode)
			ball.Draw(normalShader);

		// ImGui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		// 3. Show another simple window.
		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Check events and swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

		deltaTimeVec.push_back(deltaTime);
		if (deltaTimeVec.size() >= maxTimeSmooth)
		{
			deltaTimeVec.front() = std::move(deltaTimeVec.back());
			deltaTimeVec.pop_back();
		}

		double average = 0.0;
		for (size_t i = 0; i < deltaTimeVec.size(); i++)
			average += deltaTimeVec[i];
		average /= deltaTimeVec.size();

		std::string tempSt = "Avalanche ENGINE [FPS : " + std::to_string((int) floor(1.0f / average)) + "]";
		glfwSetWindowTitle(window, tempSt.c_str());
	}

	glfwTerminate();
	return 0;
}

glm::vec3 lerp(glm::vec3 a, glm::vec3 b, float t)
{
	return (1.0f - t) * a + t * b;
}

void CameraFollowTarget(Car target, float height, float distance, float t)
{
	glm::vec3 temp = target.position + glm::vec3(0, height, 0);
	temp -= target.forward * distance;
	Cam.Position = lerp(Cam.Position, temp, t);

	glm::vec3 tempLookAt = target.position + (target.forward * distance / 2.0f);

	Cam.lookAt(tempLookAt);
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

	if (unlockCam)
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

	float cameraSpeed = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) ? 3.8f * deltaTime * 2.5f : 3.8f * deltaTime;
	
	if (unlockCam)
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			Cam.Position += cameraSpeed * Cam.Forward;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			Cam.Position -= cameraSpeed * Cam.Forward;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			Cam.Position -= glm::normalize(glm::cross(Cam.Forward, Cam.Up)) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			Cam.Position += glm::normalize(glm::cross(Cam.Forward, Cam.Up)) * cameraSpeed;
	}

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		unlockCam = true;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		unlockCam = false;
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		wireframeMode = true;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		wireframeMode = false;
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		normalsMode = true;
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		normalsMode = false;
}