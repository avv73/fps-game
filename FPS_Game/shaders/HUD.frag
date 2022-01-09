#version 330

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D text_diffuse;

void main()
{
	vec4 recColor = texture(text_diffuse, TexCoords);

	FragColor = recColor;
}
