#include "CollisionSphere.h"

CollisionSphere::CollisionSphere()
{
	radius = 0.0f;
}

CollisionSphere::CollisionSphere(const glm::vec3& vec, float rd)
{
	center = vec;
	radius = rd;
}
