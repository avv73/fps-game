#version 330 core
layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aColor;
layout (location = 1) in vec2 aTextureCoords;

uniform mat4 ortho;

uniform float render_offset;

out vec2 TexCoords;

void main()
{
	//Color = aColor;
	TexCoords = aTextureCoords;

	vec3 position = vec3(aPos.x + render_offset, aPos.y, aPos.z);

	gl_Position = ortho * vec4(position, 1.0);
}