#include "SceneNode.h"
#include "ShaderLibrary.h"

SceneNode* SceneGraph = NULL;

std::vector<SceneNode*> SceneNode::intersectPath;

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

void GroupNode::TraverseIntersection(const glm::vec3& orig, const glm::vec3& dir, std::vector<Intersection*>& hits) // override
{
	intersectPath.push_back(this);
	for (auto it = groups.begin(); it != groups.end(); ++it)
	{
		(*it)->TraverseIntersection(orig, dir, hits);
	}
	intersectPath.pop_back();
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

void TransformNode::TraverseIntersection(const glm::vec3& orig, const glm::vec3& dir, std::vector<Intersection*>& hits) // override
{
	intersectPath.push_back(this);
	for (auto it = groups.begin(); it != groups.end(); ++it)
	{
		(*it)->TraverseIntersection(orig, dir, hits);
	}
	intersectPath.pop_back();
}

// ===ModelNode===
ModelNode::ModelNode() : SceneNode() { }

ModelNode::ModelNode(const std::string& name) : SceneNode(name) { }

ModelNode::ModelNode(const std::string& name, const std::string& path) : SceneNode(name)
{
	LoadModelFromFile(path);
	AutoLoadShader(name);
}

ModelNode::~ModelNode()
{
	if (sphere != NULL)
		delete sphere;
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
	sphere = new BoundingSphere(this, m);
}

void ModelNode::Visualize(const glm::mat4& transform)
{
	sdr->use();
	sdr->setMat4("model", transform);
	glm::mat3 normalMat = glm::transpose(glm::inverse(transform));
	sdr->setMat3("normalMat", normalMat);
	sphere->Transform(transform); // compromise, assume transform will not change when traversing for intersect since last visualize call
	m.Draw(*sdr);
}

void ModelNode::TraverseIntersection(const glm::vec3& orig, const glm::vec3& dir, std::vector<Intersection*>& hits)
{
	// traverse intersection...
	Intersection* hit = new Intersection();
	if (sphere->CollidesWithRay(orig, dir, *hit))
	{
		hit->intersectionPath = intersectPath;
		hits.push_back(hit);
	}
	else
		delete hit;
}

// TODO:
// 1. Check Terrain class, should not create a boundingsphere since it is too big and will cause problems, also unneccesary.
// 2. Check if ray will intersect the cube correctly => either
//   2.1. raycast is incorrect, if ray doesn't intersect
//   2.2. projectile visualization is incorrect

