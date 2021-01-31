#version 330 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inColor;

out vec3 vertexColor;
uniform float t;

void main()
{
	gl_Position = vec4(inPos.x, inPos.y, inPos.z, 1.0f);
	float rt = inColor.r * t;
	float rb = inColor.b * (1.0f - t);
	float r = (rt > 0.5f ? rt : 0.0f) + (rb > 0.5f ? rb : 0.0f);
	float b = 1.0f - r;
	vertexColor = vec3(r, 0.0f, b);
}
