#include "CollisionPlane.h"

#include <glm/glm.hpp>

CollisionPlane::CollisionPlane(const glm::vec3 & n, const glm::vec3 * ps)
{
	normal = n;
	for (size_t i = 0; i < POINTS_CNT; i++)
	{
		pnts[i] = ps[i];
	}
}