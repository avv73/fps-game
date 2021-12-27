#include "Collision.h"
#include <math.h>

bool Collision::RaySphere(float xc, float yc, float zc, float xd, float yd, float zd, float xs, float ys, float zs, float r, CollisionHit* hit)
{
	float b = 2 * (xd * (xs - xc) + yd * (ys - yc) + zd * (zs - zc));
	float c = xs * xs - 2 * xs * xc + xc * xc + ys * ys - 2 * ys * yc + yc * yc + zs * zs - 2 * zs * zc + zc * zc - r * r;
	float disc = (b * b - 4 * c);
	if (disc < 0)
		return false;
	else {
		if (hit != NULL)
		{
			float calcDist = (-b + sqrt(disc)) / 2;

			hit->dist = calcDist;

			float x = xs + calcDist * xd;
			float y = ys + calcDist * yd;
			float z = zs + calcDist * zd;

			glm::vec3 p(x, y, z);

			hit->point = p;
		}
		return true;
	}
}

float Collision::TriangleArea(const glm::vec3 p1, const glm::vec3 p2, const glm::vec3 p3)
{
	float a = sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y) + (p2.z - p1.z) * (p2.z - p1.z));
	float b = sqrt((p3.x - p2.x) * (p3.x - p2.x) + (p3.y - p2.y) * (p3.y - p2.y) + (p3.z - p2.z) * (p3.z - p2.z));
	float c = sqrt((p1.x - p3.x) * (p1.x - p3.x) + (p1.y - p3.y) * (p1.y - p3.y) + (p1.z - p3.z) * (p1.z - p3.z));
	float s = (a + b + c) / 2;
	return sqrt(s * ((s - a) * (s - b) * (s - c)));
}

float Collision::PointDistance(const glm::vec3 c1, const glm::vec3 c2)
{
	glm::vec3 vec(c2.x - c1.x, c2.y - c1.y, c2.z - c1.z);
	return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

float Collision::PointDistanceSquare(const glm::vec3 c1, const glm::vec3 c2)
{
	glm::vec3 vec(c2.x - c1.x, c2.y - c1.y, c2.z - c1.z);
	return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
}

bool Collision::RayPlane(float nx, float ny, float nz, float x0, float y0, float z0, float xs, float ys, float zs, float xd, float yd, float zd,
	const glm::vec3 p1, const glm::vec3 p2, const glm::vec3 p3, const glm::vec3 p4, CollisionHit* hit)
{
	if ((xd * nx + yd * ny + zd * nz) == 0)
	{
		return false;
	}
	float t = ((x0 * nx + y0 * ny + z0 * nz - nx * xs - ny * ys - nz * zs) / (xd * nx + yd * ny + zd * nz));
	if (t < 0)
	{
		return false;
	}

	float x = xs + t * xd;
	float y = ys + t * yd;
	float z = zs + t * zd;
	glm::vec3 i(x, y, z);

	if ((std::abs(TriangleArea(p1, p2, p3) - (TriangleArea(p1, p2, i) + TriangleArea(p2, p3, i) + TriangleArea(p1, p3, i))) < 0.3) ||
		std::abs(TriangleArea(p1, p3, p4) - (TriangleArea(p1, p3, i) + TriangleArea(p3, p4, i) + TriangleArea(p1, p4, i))) < 0.3)
	{
		if (hit != NULL)
		{
			hit->dist = t;
			hit->point.x = x;
			hit->point.y = y;
			hit->point.z = z;
		}
		return true;
	}
	
	return false;	
}

bool Collision::SpherePlane(glm::vec3& sp, const glm::vec3 vn, const glm::vec3 p1, const glm::vec3 p2, const glm::vec3 p3, const glm::vec3 p4, float r)
{
	CollisionHit h1, h2;

	h1.dist = 0;
	h2.dist = 0;

	if (RayPlane(vn.x, vn.y, vn.z, p1.x, p1.y, p1.z, sp.x, sp.y, sp.z, -vn.x, -vn.y, -vn.z, p1, p2, p3, p4, &h1) ||
		RayPlane(-vn.x, -vn.y, -vn.z, p1.x, p1.y, p1.z, sp.x, sp.y, sp.z, vn.x, vn.y, vn.z, p1, p2, p3, p4, &h2))
	{
		if (h1.dist > r || h2.dist > r)
		{
			return false;
		}

		float dist = 0;

		if (h1.dist > 0)
		{
			dist = h1.dist;
		}
		else 
		{
			dist = h2.dist;
		}

		sp.x = sp.x + vn.x * (r - dist);
		sp.y = sp.y + vn.y * (r - dist);
		sp.z = sp.z + vn.z * (r - dist);

		return true;
	}

	return false;

}

bool Collision::SphereSphere(glm::vec3& c1, float r1, const glm::vec3& c2, float r2)
{
	float dist = PointDistanceSquare(c1, c2);
	if (dist <= (r1 + r2) * (r1 + r2))
	{
		float a = sqrt(dist) - (r1 + r2);
		glm::vec3 i(c2 - c1);
		glm::normalize(i);
		c1 = c1 + i * a;
		return true;
	}

	return false;
}