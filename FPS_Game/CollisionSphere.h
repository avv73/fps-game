#pragma once
#ifndef COLLISIONSPHERE_H
#define COLLISIONSPHERE_H

#include <glm/glm.hpp>

class CollisionSphere 
{
public:
	float radius;
	glm::vec3 center;
	CollisionSphere();
	CollisionSphere(const glm::vec3& vec, float rd);
};


#endif