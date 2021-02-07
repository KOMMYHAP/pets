#version 330 core
out vec4 aColor;

in vec3 color;
in vec2 texCoord;

uniform sampler2D u_texture1;
uniform sampler2D u_texture2;
uniform float u_visibility;

void main()
{
	vec4 colorBox = texture(u_texture1, texCoord);
	vec4 colorSmile = texture(u_texture2, texCoord);
	vec4 mixedSmile = mix(colorSmile, vec4(color, 1.0f), u_visibility);
	vec4 mixedBox = mix(colorBox, mixedSmile, 0.3f);
	aColor = mixedBox;
}
