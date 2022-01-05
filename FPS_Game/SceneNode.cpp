#include "SceneNode.h"
#include "ShaderLibrary.h"

SceneNode* SceneGraph = NULL;

// ===SceneNode===
SceneNode::SceneNode() : NodeName("") 
{ 
	if (SceneGraph == NULL)
	{
		SceneGraph = this;
	}
}

SceneNode::SceneNode(const std::string& name) : NodeName(name) 
{
	if (SceneGraph == NULL)
	{
		SceneGraph = this;
	}
}

// ===GroupNode===
GroupNode::GroupNode() : SceneNode() { } 

GroupNode::GroupNode(const std::string& name) : SceneNode(name) { }

void GroupNode::AddNode(SceneNode* sn)
{
	groups.push_back(sn);
}

void GroupNode::RemoveNode(SceneNode* sn)
{
	for (auto it = groups.begin(); it != groups.end(); )
	{
		if (*it == sn)
		{
			it = groups.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void GroupNode::Visualize(const glm::mat4& transform) // override
{
	for (auto it = groups.begin(); it != groups.end(); ++it)
	{
		(*it)->Visualize(transform);
	}
}

void GroupNode::Shoot(const glm::vec3& orig, const glm::vec3& dir) // override
{
	for (auto it = groups.begin(); it != groups.end(); ++it)
	{
		(*it)->Shoot(orig, dir);
	}
}

// ===TransformNode===
TransformNode::TransformNode()
{
	transform = glm::mat4(1.0f);
}

TransformNode::TransformNode(const std::string& tr) : GroupNode(tr)
{
	transform = glm::mat4(1.0f);
}

void TransformNode::Rotate(glm::vec3 rtV, float angle)
{
	transform = glm::rotate(transform, glm::radians(angle), rtV);
}

void TransformNode::Scale(glm::vec3 scV)
{
	transform = glm::scale(transform, scV);
}

void TransformNode::Translate(glm::vec3 trV)
{
	transform = glm::translate(transform, trV);
}

void TransformNode::SetTransform(glm::mat4 tr)
{
	transform = tr;
}

void TransformNode::Visualize(const glm::mat4& transform) // override
{
	// stack matrices
	glm::mat4 stackedTr = this->transform * transform;

	for (auto it = groups.begin(); it != groups.end(); ++it)
	{
		(*it)->Visualize(stackedTr);
	}
}

void TransformNode::Shoot(const glm::vec3& orig, const glm::vec3& dir) // override
{
	GroupNode::Shoot(orig, dir);
}

// ===ModelNode===
ModelNode::ModelNode() : SceneNode() { }

ModelNode::ModelNode(const std::string& name) : SceneNode(name) { }

ModelNode::ModelNode(const std::string& name, const std::string& path) : SceneNode(name)
{
	LoadModelFromFile(path);
	AutoLoadShader(name);
}

void ModelNode::SetShader(Shader* sd)
{
	this->sdr = sd;
}

void ModelNode::AutoLoadShader(const std::string& name)
{
	this->sdr = ShaderLibrary::GetInstance()->GetShader(name);
}

void ModelNode::LoadModelFromFile(const std::string& path)
{
	m.LoadModel(path);
	// create sphere
}

void ModelNode::Visualize(const glm::mat4& transform)
{
	sdr->use();
	sdr->setMat4("model", transform);
	glm::mat3 normalMat = glm::transpose(glm::inverse(transform));
	sdr->setMat3("normalMat", normalMat);
	//boundingSphere->Transform ??
	m.Draw(*sdr);
}

void ModelNode::Shoot(const glm::vec3& orig, const glm::vec3& dir)
{
	// traverse intersection...
}

