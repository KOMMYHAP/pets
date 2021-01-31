#version 330 core
out vec4 aColor;

in vec3 color;
in vec2 texCoord;

uniform sampler2D ourTexture;

void main()
{
	aColor = texture(ourTexture, texCoord);
}
