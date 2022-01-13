#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Model.h"
#include "Shader.h"

#ifndef SCENENODE_H
#define SCENENODE_H

class SceneNode
{
public:
	SceneNode();
	SceneNode(const std::string& name);

	virtual void Visualize(const glm::mat4& transform) = 0;
	virtual void Shoot(const glm::vec3& orig, const glm::vec3& dir) = 0;

	const std::string NodeName;
};

extern SceneNode* SceneGraph;

class GroupNode : public SceneNode
{
public:
	GroupNode();
	GroupNode(const std::string& name);

	void AddNode(SceneNode* sn);
	void RemoveNode(SceneNode* sn);

	void Visualize(const glm::mat4& transform); // override
	void Shoot(const glm::vec3& orig, const glm::vec3& dir); // override
protected:
	std::vector<SceneNode*> groups;
};

class TransformNode : public GroupNode
{
public:
	TransformNode();
	TransformNode(const std::string& name);

	void Rotate(glm::vec3 rotV, float angle);
	void Scale(glm::vec3 scV);
	void Translate(glm::vec3 trV);

	void SetTransform(glm::mat4 tr);

	void Visualize(const glm::mat4& transform); // override
	void Shoot(const glm::vec3& orig, const glm::vec3& dir); // override
private:
	glm::mat4 transform;
};

class ModelNode : public SceneNode
{
public:
	ModelNode();
	ModelNode(const std::string& name);
	ModelNode(const std::string& name, const std::string& path);

	void SetShader(Shader* sd);

	void Visualize(const glm::mat4& transform); // override
	void Shoot(const glm::vec3& orig, const glm::vec3& dir); // override

protected:
	Model m;
	Shader* sdr;
	//BoundingSphere* sphere = NULL;
private:
	void LoadModelFromFile(const std::string& path);

	void AutoLoadShader(const std::string& name);
};

#endif
