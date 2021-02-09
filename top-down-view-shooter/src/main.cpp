#define _CRT_SECURE_NO_WARNINGS

#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS 1
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define GLFW_INCLUDE_NONE
#include "glad/gl.h"
#include "GLFW/glfw3.h"

#include <cstdio>
#include <chrono>
#include <thread>
#include <algorithm>

#include <crtdbg.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// #define GLM_FORCE_MESSAGES
#define GLM_FORCE_SSE42
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

static GLFWwindow* g_mainWindow = nullptr;
static GLuint g_width, g_height;
static bool g_wireframeMode = false;
static glm::vec2 g_mouseMove;

void glfwErrorCallback(int error, const char* description)
{
	fprintf(stderr, "GLFW error occured. Code: %d. Description: %s\n", error, description);
}

static void HandleArrowKey(int key, bool pressed)
{
	float speed = pressed ? 7.0f : 0.0f;
	g_mouseMove.x =
		key == GLFW_KEY_LEFT ?	-speed :
		key == GLFW_KEY_RIGHT ?	+speed :
		g_mouseMove.x;

	g_mouseMove.y =
		key == GLFW_KEY_UP ?	-speed :
		key == GLFW_KEY_DOWN ?	+speed :
		g_mouseMove.y;
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

	if (action == GLFW_RELEASE || action == GLFW_PRESS)
	{
		switch (key)
		{
			case GLFW_KEY_UP:
			case GLFW_KEY_DOWN:
			case GLFW_KEY_LEFT:
			case GLFW_KEY_RIGHT:
				HandleArrowKey(key, action == GLFW_PRESS);
				break;
			default:
				break;
		}
	}
}

static bool ImguiDragMat(const char * title, const glm::mat4&)
{
	// glm::translate()
	return false;
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
		g_width = 1000;
		g_height = 800;
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
			 0.9f,  0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	10.0f, 10.0f,		// top right
			 0.9f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	10.0f, 0.0f,		// bottom right
			-0.1f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,		// bottom left
			-0.1f,  0.5f, 0.0f,		1.0f, 1.0f, 0.0f,	0.0f, 10.0f		// top left
		};
		GLuint indices[] = {
			0, 1, 3,
			1, 2, 3,
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
		unsigned char *smileData = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
		auto p1 = std::unique_ptr<unsigned char, decltype(&stbi_image_free)>(smileData, &stbi_image_free);

		unsigned char *boxData = stbi_load("wall.jpg", &width, &height, &nrChannels, 0);
		auto p2 = std::unique_ptr<unsigned char, decltype(&stbi_image_free)>(boxData, &stbi_image_free);

		if (not smileData || not boxData)
		{
			fprintf(stderr, "Failed to load images!\n");
			return -1;
		}

		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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
		glm::mat4 trans(1.0f);
		shader->SetMat4("u_transform", glm::value_ptr(trans));
		shader->SetFloat("u_colorT", 0.3f);
	}

	std::chrono::duration<double> frameTime(0.0);
	std::chrono::duration<double> sleepAdjust(0.0);
	std::chrono::duration<double> timeUsed(0.0);
	std::chrono::duration<double> totalTimeUsed(0.0);

	glm::vec3 translate{};
	glm::mat4 model(1.0f);
	glm::mat4 view(1.0f);
	glm::mat4 projection(1.0f);
	g_mouseMove  = glm::vec2(0.0f);

	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	float visibility = 0.5f;
	while (!glfwWindowShouldClose(g_mainWindow))
	{
		std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		{ // update
			if (g_mouseMove.x != 0.0f || g_mouseMove.y != 0.0f)
			{
				double x = 0.0f;
				double y = 0.0f;
				glfwGetCursorPos(g_mainWindow, &x, &y);
				x += g_mouseMove.x;
				y += g_mouseMove.y;
				glfwSetCursorPos(g_mainWindow, x, y);
			}
		}

		{ // render
			glPolygonMode(GL_FRONT_AND_BACK, g_wireframeMode ? GL_LINE : GL_FILL);
			glBindVertexArray(vao[0]);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textures[0]);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, textures[1]);

			shader->Use();
			shader->SetFloat("u_visibility", visibility);

			glm::mat4 trans(1.0f);
			trans = projection * view * model * glm::translate(trans, translate);
			shader->SetMat4("u_transform", glm::value_ptr(trans));

			glDrawElements(GL_TRIANGLES, indices_size[0], GL_UNSIGNED_INT, 0);
		}

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();

		ImGui::NewFrame();
		// ImGui::DragFloat("Smile", &visibility, 0.00f, 0.0f, 1.0f);
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
