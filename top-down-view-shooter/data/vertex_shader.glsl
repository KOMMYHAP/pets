#version 330 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inColor;

out vec3 vertexColor;

uniform float offsetX;
uniform vec3 colorT;

void main()
{
	gl_Position = vec4(inPos.x + offsetX, inPos.y, inPos.z, 1.0);
	vertexColor = vec3(inColor.r * colorT.r, inColor.g * colorT.g, inColor.b * colorT.b);
}
