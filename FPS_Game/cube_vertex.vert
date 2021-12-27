#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 Color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	vec3 FragPos = vec3(model * vec4(aPos, 1.0));
	Color = aColor;

	gl_Position = proj * view * vec4(FragPos, 1.0);
}