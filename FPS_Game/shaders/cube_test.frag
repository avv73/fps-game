#version 330 core

struct Material {
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
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

uniform vec3 ViewPos;
uniform Material material;
uniform Light light;

void main()
{
//	vec3 ambient = texture(material.texture_diffuse1, texcoords).rgb;
//
//	vec3 norm = normalize(normal);
//	vec3 lightdir = normalize(light.position - fragpos);
//	float diff = max(dot(norm, lightdir), 0.0);
//	vec3 diffuse = light.diffuse * diff * texture(material.texture_diffuse1, texcoords).rgb;
//
//	vec3 viewdir = normalize(viewpos - fragpos);
//	vec3 reflectdir = reflect(-lightdir, norm);
//	float spec = pow(max(dot(viewdir, reflectdir), 0.0), material.shininess);
//	vec3 specular = spec * texture(material.texture_specular1, texcoords).rgb;
//
//	vec3 result = ambient + diffuse + specular;
//	fragcolor = vec4(result, 1.0f);
	FragColor = vec4(texture(material.texture_diffuse1, TexCoords).rgb, 1.0f);
}