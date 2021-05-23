#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 color;
out vec2 texCoord;

uniform mat4 u_transform;
uniform float u_colorT;

void main()
{
	gl_Position = u_transform * vec4(aPos, 1.0f);
	color = u_colorT * aColor;
	texCoord = aTexCoord;
}
