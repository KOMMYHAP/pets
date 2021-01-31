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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Shader.h"

GLFWwindow* g_mainWindow = nullptr;
GLuint g_width, g_height;
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
	ImGui::CreateContext();

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
	GLuint texture;
	glGenVertexArrays(2, vao);
	glGenBuffers(2, vbo);
	glGenBuffers(2, ebo);
	glGenTextures(1, &texture);

	Shader shader;
	try
	{
		shader = std::move(Shader::Load("vertex_shader.glsl", "fragment_shader.glsl"));
	}
	catch (const std::runtime_error & e)
	{
		fprintf(stderr, e.what());
		return -1;
	}

	{ // binding data for vao[0]
		float vertices[] = {
			// positions			// colors
			 0.5f,  0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,		// top right
			 0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,		// bottom right
			-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,		// bottom left
			-0.5f,  0.5f, 0.0f,		1.0f, 1.0f, 0.0f,	0.0f, 1.0f		// top left
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
		
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		int width, height, nrChannels;
		unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
		if (!data)
		{
			fprintf(stderr, "failed to load image");
			return -1;
		}
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}

	std::chrono::duration<double> frameTime(0.0);
	std::chrono::duration<double> sleepAdjust(0.0);
	std::chrono::duration<double> timeUsed(0.0);

	float speed = 1.0f;
	while (!glfwWindowShouldClose(g_mainWindow))
	{
		std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
		
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		{
			glPolygonMode(GL_FRONT_AND_BACK, g_wireframeMode ? GL_LINE : GL_FILL);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture);
			shader.Use();
			glBindVertexArray(vao[0]);
			glDrawElements(GL_TRIANGLES, indices_size[0], GL_UNSIGNED_INT, 0);
		}

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		
		ImGui::NewFrame();
		ImGui::DragFloat("Speed", &speed, 0.1f, 0.1f, 3.0f);
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
	}

	glDeleteVertexArrays(2, vao);
	glDeleteBuffers(2, vbo);
	glDeleteBuffers(2, ebo);

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	glfwTerminate();

	return 0;
}
