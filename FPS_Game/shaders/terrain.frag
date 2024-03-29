#version 330 core

struct Material {
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	bool specularSet;
	float shininess;
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
	vec3 ambient = texture(material.texture_diffuse1, TexCoords).rgb;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.texture_diffuse1, TexCoords).rgb;

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = spec * texture(material.texture_specular1, TexCoords).rgb;

	if (!material.specularSet)
	{
		specular = vec3(0.0f);
	}

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0f);
}