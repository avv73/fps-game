#version 330 core

struct Material {
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	float shininess;
	bool specularSet;
};

struct Light {
	vec3 position;
	vec3 diffuse;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
	vec3 result = texture(material.texture_diffuse1, TexCoords).rgb;
	FragColor = vec4(result, 1.0f);
}