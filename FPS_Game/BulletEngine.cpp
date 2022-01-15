#include "BulletEngine.h"
#include "ShaderLibrary.h"
#include <math.h>

BulletEngine::BulletEngine(float clipX, float clipZ)
	: ClipX(clipX), ClipZ(clipZ)
{
	bulletShdr = ShaderLibrary::GetInstance()->GetShader("bullet");
	bulletModel.LoadModel("./models/bullet_new/bulletNewObj.obj");
	shotBullets.reserve(100);
}

void BulletEngine::ScreenCenterToWorldRay(glm::mat4 viewMatrix, glm::mat4 projMatrix, glm::vec3& outDir)
{
	glm::vec4 lray_NDC(0.0f, 0.0f, 1.0f, 1.0f);

	glm::mat4 inverseProj = glm::inverse(projMatrix);
	glm::mat4 inverseView = glm::inverse(viewMatrix);

	glm::vec4 lray_camera = glm::vec4(inverseProj * lray_NDC);
	lray_camera.z = -1;
	lray_camera.w = 0;
	glm::vec4 lray_world = inverseView * lray_camera;

	outDir = glm::normalize(lray_world);
}

void BulletEngine::Shoot(glm::vec3 worldDirection, glm::vec3 origin, float yaw, float pitch)
{
	Bullet blt;
	blt.direction = worldDirection;
	blt.position = origin;
	blt.clipped = false;
	blt.yaw = yaw;
	blt.pitch = pitch;

	shotBullets.push_back(blt);
}

void BulletEngine::Update(float delta)
{
	for (auto it = shotBullets.begin(); it != shotBullets.end(); ++it)
	{
		if ((*it).clipped)
			continue;

		(*it).position += (*it).direction * delta * BulletVelocity;
		if (fabs((*it).position.x) > ClipX || fabs((*it).position.z) > ClipZ)
		{
			(*it).clipped = true;
		}
	}

	// TODO: perform collision detection?

	clipCounter++;
	if (clipCounter >= ClipThreshold)
	{
		FreeClippedBullets();
		clipCounter = 0;
	}
}

void BulletEngine::FreeClippedBullets()
{
	for (auto it = shotBullets.begin(); it != shotBullets.end(); ++it)
	{
		if (!(*it).clipped)
			continue;
		shotBullets.erase(it);
	}
}

void BulletEngine::Visualize()
{
	// TODO: Visualize all bullets in the vector, translate?
	for (auto it = shotBullets.begin(); it != shotBullets.end(); ++it)
	{
		bulletShdr->use();
		glm::mat4 transformM = glm::translate(glm::mat4(1.0f), (*it).position);
		transformM = glm::rotate(transformM, glm::radians(-(*it).yaw), glm::vec3(0.0f, 1.0f, 0.0f)); 
		transformM = glm::rotate(transformM, glm::radians((*it).pitch), glm::vec3(1.0f, 0.0f, 0.0f));
		bulletShdr->setMat4("model", transformM);
		bulletModel.Draw(*bulletShdr);
	}
}