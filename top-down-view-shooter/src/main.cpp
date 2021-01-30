#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS 1

#include "imgui/imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define GLFW_INCLUDE_NONE
#include "glad/gl.h"
#include "GLFW/glfw3.h"

#include <cstdio>
#include <chrono>
#include <thread>
#include <array>

GLFWwindow* g_mainWindow = nullptr;
static bool g_wireframeMode = false;
constexpr auto * g_vertexShaderData = R"(
#version 330 core
layout (location = 0) in vec3 aPos;

void main()
{
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)";

constexpr auto * g_fragmentShaderData = R"(
#version 330 core
out vec4 aColor;

void main()
{
	aColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
)";

void glfwErrorCallback(int error, const char* description)
{
	fprintf(stderr, "GLFW error occured. Code: %d. Description: %s\n", error, description);
}

static bool CheckShader(GLint shader, const char * desc)
{
	GLint status = 0;		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	GLint logLength = 0;	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

	if (static_cast<GLboolean>(status) == GL_FALSE)
	{
		fprintf(stderr, "failed to compile: %s!\n", desc);
	}
	if (logLength > 0)
	{
		std::vector<char> log(logLength + 1);
		glGetShaderInfoLog(shader, logLength, NULL, log.data());
		fprintf(stderr, "%s\n", log.data());
	}
	return static_cast<GLboolean>(status) == GL_TRUE;
}

static bool CheckProgram(GLint program, const char * desc)
{
	GLint status = 0;		glGetProgramiv(program, GL_LINK_STATUS, &status);
	GLint logLength = 0;	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

	if (static_cast<GLboolean>(status) == GL_FALSE)
	{
		fprintf(stderr, "failed to link: %s!\n", desc);
	}
	if (logLength > 0)
	{
		std::vector<char> log(logLength + 1);
		glGetProgramInfoLog(program, logLength, NULL, log.data());
		fprintf(stderr, "%s\n", log.data());
	}
	return static_cast<GLboolean>(status) == GL_TRUE;
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

int main()
{
	glfwSetErrorCallback(glfwErrorCallback);
	
	if (glfwInit() == 0)
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	bool fullscreen = false;
	if (fullscreen)
	{
		g_mainWindow = glfwCreateWindow(1366, 768, "Top Down View Shooter", glfwGetPrimaryMonitor(), NULL);
	}
	else
	{
		g_mainWindow = glfwCreateWindow(800, 600, "Top Down View Shooter", NULL, NULL);
	}
	
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

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	float vertices[] = {
		-0.6,  0.5, 0.0,	// 0
		-0.5, -0.5, 0.0,	// 1
		-0.3, -0.5, 0.0,	// 2
		-0.1,  0.3, 0.0,	// 3
		 0.1,  0.3, 0.0,	// 4
		 0.3, -0.5, 0.0,	// 5
		 0.5, -0.5, 0.0,	// 6
		 0.6,  0.5, 0.0,	// 7
		 0.4,  0.5, 0.0,	// 8
		0.45, -0.4, 0.0,	// 9
		0.35, -0.4, 0.0,	// 10
		 0.1,  0.5, 0.0,	// 11
		-0.1,  0.5, 0.0,	// 12
	   -0.35, -0.4, 0.0,	// 13
	   -0.45, -0.4, 0.0,	// 14
		-0.4,  0.5, 0.0,	// 15
	};
	GLuint indices[] = {
		 0,  1, 15,
		15,  1, 14,
		14,  1,  2,
		 2, 14, 13,
		13,  2, 12,
		12,  2,  3,
		 3, 12, 11,
		11,  3,  4,
		 4, 11,  5,
		 5, 11, 10,
		10,  5,  6,
		 6, 10,  9,
		 9,  6,  8,
		 8,  7,  6,
	};
		
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &g_vertexShaderData, NULL);
	glCompileShader(vertexShader);
	CheckShader(vertexShader, "vertex shader");

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &g_fragmentShaderData, NULL);
	glCompileShader(fragmentShader);
	CheckShader(fragmentShader, "fragment shader");

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	CheckProgram(shaderProgram, "shader program");
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	std::chrono::duration<double> frameTime(0.0);
	std::chrono::duration<double> sleepAdjust(0.0);
	std::chrono::duration<double> timeUsed(0.0);
	
	int width, height;
	while (!glfwWindowShouldClose(g_mainWindow))
	{
		std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();

		glfwGetWindowSize(g_mainWindow, &width, &height);

		int bufferWidth, bufferHeight;
		glfwGetFramebufferSize(g_mainWindow, &bufferWidth, &bufferHeight);
		glViewport(0, 0, bufferWidth, bufferHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		{
			glUseProgram(shaderProgram);
			glPolygonMode(GL_FRONT_AND_BACK, g_wireframeMode ? GL_LINE : GL_FILL);
			glBindVertexArray(vao);
			glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();

		ImGui::NewFrame();
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

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	glfwTerminate();

	return 0;
}
