#version 330 core
out vec4 aColor;

in vec3 vertexColor;

void main()
{
	aColor = vec4(vertexColor, 1.0f); //vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
