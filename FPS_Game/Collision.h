#pragma once
#ifndef COLLISION_H
#define COLLISION_H

#include <glm/glm.hpp>

struct CollisionHit
{
	float dist;
	glm::vec3 point;
};

class Collision
{
public:
	/// <summary>
	/// Finds collisions between a sphere and a ray.
	/// </summary>
	/// <param name="xc">Sphere center</param>
	/// <param name="yc">Sphere center</param>
	/// <param name="zc">Sphere center</param>
	/// <param name="xd">Ray direction</param>
	/// <param name="yd">Ray direction</param>
	/// <param name="zd">Ray direction</param>
	/// <param name="xs">Ray origin</param>
	/// <param name="ys">Ray origin</param>
	/// <param name="zs">Ray origin</param>
	/// <param name="r">Sphere radius</param>
	/// <param name="hit">Info about intersection, if any</param>
	/// <returns>Whether there was intersection or not.</returns>
	static bool RaySphere(float xc, float yc, float zc, float xd, float yd, float zd, float xs, float ys, float zs, float r, CollisionHit* hit = NULL);
	/// <summary>
	/// Finds collisions between a plane and a ray.
	/// </summary>
	/// <param name="nx">Plane normal</param>
	/// <param name="ny">Plane normal</param>
	/// <param name="nz">Plane normal</param>
	/// <param name="x0">First point plane</param>
	/// <param name="y0">First point plane</param>
	/// <param name="z0">First point plane</param>
	/// <param name="xs">Ray origin</param>
	/// <param name="ys">Ray origin</param>
	/// <param name="zs">Ray origin</param>
	/// <param name="xd">Ray direction</param>
	/// <param name="yd">Ray direction</param>
	/// <param name="zd">Ray direction</param>
	/// <param name="p1">Plane point</param>
	/// <param name="p2">Plane point</param>
	/// <param name="p3">Plane point</param>
	/// <param name="p4">Plane point</param>
	/// <param name="hit">Info about intersection, if any</param>
	/// <returns>Whether there was intersection or not</returns>
	static bool RayPlane(float nx, float ny, float nz, float x0, float y0, float z0, float xs, float ys, float zs, float xd, float yd, float zd, 
		const glm::vec3 p1, const glm::vec3 p2, const glm::vec3 p3, const glm::vec3 p4, CollisionHit* hit = NULL);
	static float TriangleArea(const glm::vec3 p1, const glm::vec3 p2, const glm::vec3 p3);

	/// <summary>
	/// Finds collisions between two spheres.
	/// </summary>
	/// <param name="c1">First sphere center, altered if intersected</param>
	/// <param name="r1">First sphere radius</param>
	/// <param name="c2">Second sphere center</param>
	/// <param name="r2">Second sphere radius</param>
	/// <returns>Whether there was intersection or not.</returns>
	static bool SphereSphere(glm::vec3& c1, float r1, const glm::vec3& c2, float r2);

	static float PointDistance(const glm::vec3 c1, const glm::vec3 c2);
	static float PointDistanceSquare(const glm::vec3 c1, const glm::vec3 c2);

	/// <summary>
	/// Finds collisions between a plane and a sphere.
	/// </summary>
	/// <param name="sp">Sphere center. Altered when intersected.</param>
	/// <param name="vn">Normal of plane.</param>
	/// <param name="p1">Plane points</param>
	/// <param name="p2">Plane points</param>
	/// <param name="p3">Plane points</param>
	/// <param name="p4">Plane points</param>
	/// <param name="r">Sphere radius</param>
	/// <returns>Whether there was intersection or not.</returns>
	static bool SpherePlane(glm::vec3& sp, const glm::vec3 vn, const glm::vec3 p1, const glm::vec3 p2, const glm::vec3 p3, const glm::vec3 p4, float r);
};


#endif