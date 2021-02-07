#define _CRT_SECURE_NO_WARNINGS
#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS 1
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#include "imgui/imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define GLFW_INCLUDE_NONE
#include "glad/gl.h"
#include "GLFW/glfw3.h"

#include <cstdio>
#include <chrono>
#include <thread>

#include <crtdbg.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// #define GLM_FORCE_MESSAGES
#define GLM_FORCE_SSE42
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.h"

static GLFWwindow* g_mainWindow = nullptr;
static GLuint g_width, g_height;
static bool g_wireframeMode = false;

void glfwErrorCallback(int error, const char* description)
{
	fprintf(stderr, "GLFW error occured. Code: %d. Description: %s\n", error, description);
}

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
	if (ImGui::GetIO().WantCaptureKeyboard)
	{
		return;
	}

	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(g_mainWindow, GL_TRUE);
			break;
		case GLFW_KEY_SPACE:
			g_wireframeMode = !g_wireframeMode;
			break;
		default:
			break;
		}
	}
}

int main(int argc, char** argv)
{
	// Enable memory-leak reports
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG));
	
	glfwSetErrorCallback(glfwErrorCallback);

	if (glfwInit() == 0)
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	bool fullscreen = false;
	GLFWmonitor * monitor = NULL;
	if (fullscreen)
	{
		g_width = 1366;
		g_height = 768;
		monitor = glfwGetPrimaryMonitor();
	}
	else
	{
		g_width = 800;
		g_height = 600;
	}
	g_mainWindow = glfwCreateWindow(g_width, g_height, "Top Down View Shooter", monitor, NULL);

	if (g_mainWindow == nullptr)
	{
		fprintf(stderr, "Failed to open GLFW g_mainWindow.\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(g_mainWindow);

	// Load OpenGL functions using glad
	int version = gladLoadGL(glfwGetProcAddress);
	printf("GL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
	printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));

	glfwSetKeyCallback(g_mainWindow, KeyCallback);

	IMGUI_CHECKVERSION();
	auto context = ImGui::CreateContext();

	bool success = ImGui_ImplGlfw_InitForOpenGL(g_mainWindow, false);
	if (!success)
	{
		printf("ImGui_ImplGlfw_InitForOpenGL failed.\n");
		assert(false);
	}
	
	success = ImGui_ImplOpenGL3_Init();
	if (!success)
	{
		printf("ImGui_ImplOpenGL3_Init failed.\n");
		assert(false);
	}
	
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	
	glViewport(0, 0, g_width, g_height);

	GLint nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	printf("Maximum number of vertex attributes supported: %d\n", nrAttributes);

	GLuint indices_size[2] = {};
	GLuint vao[2] = {};
	GLuint vbo[2] = {};
	GLuint ebo[2] = {};
	GLuint textures[2] = {};
	glGenVertexArrays(2, vao);
	glGenBuffers(2, vbo);
	glGenBuffers(2, ebo);
	glGenTextures(2, textures);

	auto shader = Shader::Load("vertex_shader.glsl", "fragment_shader.glsl");
	if (!shader)
	{
		fprintf(stderr, "Failed to create shader!\n");
		return -1;
	}

	{ // binding data for vao[0]
		float vertices[] = {
			// positions			// colors
			 0.0f, -0.8f, 0.0f,		1.0f, 0.0f, 0.0f,	0.5f, 0.5f,		// top right
			 0.8f, 0.8f, 0.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,		// bottom right
			-0.8f, 0.8f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,		// bottom left
			 // 0.8f,  0.8f, 0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,		// top right
			 // 0.8f, -0.8f, 0.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,		// bottom right
			// -0.8f, -0.8f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,		// bottom left
			// -0.8f,  0.8f, 0.0f,		1.0f, 1.0f, 0.0f,	0.0f, 1.0f		// top left
		};
		GLuint indices[] = {
			0, 1, 2,
			// 0, 1, 3,
			// 1, 2, 3,
		};
		indices_size[0] = sizeof(indices) / sizeof(indices[0]);

		glBindVertexArray(vao[0]);
		
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(0));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
		
		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char *smileData = stbi_load("fiberglass.jpg", &width, &height, &nrChannels, 0);
		auto p1 = std::unique_ptr<unsigned char, decltype(&stbi_image_free)>(smileData, &stbi_image_free);
		
		unsigned char *boxData = stbi_load("floor_tile.png", &width, &height, &nrChannels, 0);
		auto p2 = std::unique_ptr<unsigned char, decltype(&stbi_image_free)>(boxData, &stbi_image_free);
		
		if (not smileData || not boxData)
		{
			fprintf(stderr, "Failed to load images!\n");
			return -1;
		}
		
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, boxData);
		GLenum e = glGetError();
		if (e != GL_NO_ERROR)
		{
			fprintf(stderr, "Failed to load texture's data: error code %d\n", e);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
		
		glBindTexture(GL_TEXTURE_2D, textures[1]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, smileData);
		e = glGetError();
		if (e != GL_NO_ERROR)
		{
			fprintf(stderr, "Failed to load texture's data: error code %d\n", e);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
		
		shader->Use();
		shader->SetInt("u_texture1", 0);
		shader->SetInt("u_texture2", 1);
	}
	
	std::chrono::duration<double> frameTime(0.0);
	std::chrono::duration<double> sleepAdjust(0.0);
	std::chrono::duration<double> timeUsed(0.0);
	std::chrono::duration<double> totalTimeUsed(0.0);

	auto Draw = [&shader, indices_size](const glm::mat4 & trans)
	{
		shader->SetMat4("u_transform", glm::value_ptr(trans));
		glDrawElements(GL_TRIANGLES, indices_size[0], GL_UNSIGNED_INT, 0);
	};

	glm::vec3 origin(0.0f, -0.4f, 0.0f);
	glm::vec3 offset1 (0.0f, 1.2f, 0.0f);
	glm::vec3 offset2 (-0.8f, -0.4f, 0.0f);
	glm::vec3 offset3 (0.8f, -0.4f, 0.0f);
	glm::vec3 scale (0.5f);
	std::function<void(glm::vec3, glm::mat4, int)> DrawRec = 
	[&Draw, &DrawRec, &offset1, &offset2, &offset3, &scale](glm::vec3 translate, glm::mat4 matrix, int depth)
	{
		if (depth <= 0)
		{
			return;
		}

		--depth;
		matrix = glm::translate(matrix, translate);
		matrix = glm::scale(matrix, scale);
		DrawRec(offset1, matrix, depth);
		DrawRec(offset2, matrix, depth);
		// matrix = glm::scale(matrix, glm::vec3(-1.0f, 1.0f, 1.0f));
		DrawRec(offset3, matrix, depth);
		
		Draw(matrix);
	};
	
	int depth = 7;
	float smileVisibility = 0.5f;
	float colorT = 0.0f;
	shader->Use();
	shader->SetFloat("u_visibility", smileVisibility);
	
	while (!glfwWindowShouldClose(g_mainWindow))
	{
		std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
		
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		{
			glPolygonMode(GL_FRONT_AND_BACK, g_wireframeMode ? GL_LINE : GL_FILL);
			glBindVertexArray(vao[0]);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textures[0]);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, textures[1]);
			shader->Use();
			float t = static_cast<float>(totalTimeUsed.count());
			colorT = sin(5 * t) / 2.0f + 0.5f;
			smileVisibility = colorT / 2.0f + 0.5f;
			shader->SetFloat("u_colorT", colorT);
			shader->SetFloat("u_visibility", smileVisibility);
			DrawRec(origin, glm::mat4(1.0f), depth);
		}

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		
		ImGui::NewFrame();
		ImGui::DragFloat3("Origin", glm::value_ptr(origin), 0.1f, -10.0f, 10.0f, "%.2f");
		ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.01f, 0.01f, 10.0f, "%.2f");
		ImGui::DragFloat3("Offset 1", glm::value_ptr(offset1), 0.1f, -10.0f, 10.0f, "%.2f");
		ImGui::DragFloat3("Offset 2", glm::value_ptr(offset2), 0.1f, -10.0f, 10.0f, "%.2f");
		ImGui::DragFloat3("Offset 3", glm::value_ptr(offset3), 0.1f, -10.0f, 10.0f, "%.2f");
		
		ImGui::DragInt("Depth", &depth, 1, 1, 10);
		ImGui::DragFloat("Smile", &smileVisibility, 0.01f, 0.0f, 1.0f);
		ImGui::Text("Time Used: %.2f ms", timeUsed * 100);
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(g_mainWindow);
		glfwPollEvents();

		// Throttle to cap at 60Hz. This adaptive using a sleep adjustment. This could be improved by
		// using mm_pause or equivalent for the last millisecond.
		std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
		std::chrono::duration<double> target(1.0 / 60.0);
		timeUsed = t2 - t1;
		std::chrono::duration<double> sleepTime = target - timeUsed + sleepAdjust;
		if (sleepTime > std::chrono::duration<double>(0))
		{
			std::this_thread::sleep_for(sleepTime);
		}

		std::chrono::steady_clock::time_point t3 = std::chrono::steady_clock::now();
		frameTime = t3 - t1;

		// Compute the sleep adjustment using a low pass filter
		sleepAdjust = 0.9 * sleepAdjust + 0.1 * (target - frameTime);
		totalTimeUsed += timeUsed;
	}

	shader.reset();
	glDeleteVertexArrays(2, vao);
	glDeleteBuffers(2, vbo);
	glDeleteBuffers(2, ebo);

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext(context);
	glfwTerminate();

	return 0;
}
