#pragma once
#ifndef CUBEMAPNODE_H
#define CUBEMAP_H

#include "SceneNode.h"

class CubemapNode : public ModelNode
{
public:
	CubemapNode(const std::string& top, const std::string& left, const std::string& right, const std::string& bottom, const std::string& front, const std::string& back);

	void Visualize(const glm::mat4& transform); // override
	void Shoot(const glm::vec3& orig, const glm::vec3& dir); // override
private:
	unsigned int textureID;
	vector<std::string> faces;

	Shader* skyboxShader;
	
	void LoadCubemap();
	void CreateCube(GLuint& VBO);

	GLuint VAO;
};


#endif