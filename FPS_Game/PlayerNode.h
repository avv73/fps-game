#pragma once
#ifndef PLAYERNODE_H
#define PLAYERNODE_H

#include "SceneNode.h"

class Player;

class PlayerNode : public ModelNode
{
public:
	PlayerNode(Player* pl);

	void Visualize(const glm::mat4& transform); // override
	void TraverseIntersection(const glm::vec3& orig, const glm::vec3& dir, std::vector<Intersection*>& hits, bool isShot); //override

	void UpdateBoundingPosition(const glm::vec3& pos);
private:
	Player* pl;
};

#endif