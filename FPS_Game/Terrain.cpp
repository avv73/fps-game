#include "Terrain.h"
#include "ShaderLibrary.h"

Terrain::Terrain(glm::vec2 startPoint, int size)
	: ModelNode("terrain")
{
	m = GenerateTerrain(startPoint, size);
	sdr = ShaderLibrary::GetInstance()->GetShader("terrain");
}

void Terrain::Shoot(const glm::vec3& orig, const glm::vec3& dir)
{
}

void Terrain::Visualize(const glm::mat4& transform)
{
	sdr->use();
	sdr->setMat4("model", transform);

	m.Draw(*sdr);
}

Model Terrain::GenerateTerrain(glm::vec2 startPoint, int size)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	vertices.reserve(size * size);

	for (int x = startPoint.x; x < startPoint.x + size; x++)
	{
		for (int z = startPoint.y; z < startPoint.y + size; z++)
		{
			Vertex to_add;

			to_add.Position = glm::vec3(x, -1.0f , z);
			to_add.Normal = glm::vec3(0.0f);

			glm::vec2 texcoord;

			if (x % 2 == 0)
				texcoord.y = 1;
			else
				texcoord.y = 0;

			if (z % 2 == 0)
				texcoord.x = 0;
			else
				texcoord.x = 1;

			to_add.TexCoords = texcoord;
			to_add.Tangent = glm::vec3(0.0f);
			to_add.Bitangent = glm::vec3(0.0f);

			vertices.push_back(to_add);
		}
	}

	indices.reserve(size * size * 6); // terrain vertices per tile

	for (int x = 0; x < size-1; x++)
	{
		for (int z = 0; z < size-1; z++)
		{
			unsigned int offset = x * size + z;
			indices.push_back(offset + 0);
			indices.push_back(offset + 1);
			indices.push_back(offset + size);
			indices.push_back(offset + 1);
			indices.push_back(offset + size + 1);
			indices.push_back(offset + size);
		}
	}

	// calculate normals??

	// load texture
	Texture tex;
	GLuint texid;
	if (Model::LoadTexture("./models/terrain_tex.jpg", texid))
	{
		tex.id = texid;
		tex.type = "material.texture_diffuse";
		tex.path = "./models/terrain_tex.jpg";
	}
	else 
	{
		printf("ERROR: Terrain generation - unable to load texture!\n");
	}

	vector<Texture> textures_m;

	textures_m.push_back(tex);

	Mesh modelMesh(vertices, indices, textures_m);
	Model terrainModel;

	terrainModel.meshes.push_back(modelMesh);

	return terrainModel;
}