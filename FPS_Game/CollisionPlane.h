#pragma once
#ifndef COLLISIONPLANE_H
#define COLLISIONPLANE_H

#include <glm/glm.hpp>

class CollisionPlane
{
public:
	static const int POINTS_CNT = 4;
	glm::vec3 pnts[POINTS_CNT];
	glm::vec3 normal;
	CollisionPlane(const glm::vec3& n, const glm::vec3* ps);
};


#endif