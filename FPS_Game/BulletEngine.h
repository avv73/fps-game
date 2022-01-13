#pragma once
#ifndef BULLETENGINE_H
#define BULLETENGINE_H

#include <glm/glm.hpp>
#include "Player.h"

class Bullet
{
public:
	Bullet(glm::vec3 pos, glm::vec3 dir);

	glm::vec3 position;
	glm::vec3 direction;
	bool clipped;
};

class BulletEngine
{
public:
	BulletEngine(float clipX, float clipZ);

	void Update(float delta);

	void Visualize();

	static void ScreenPosToWorldRay(int mouseX, int mouseY, int viewportWidth, int viewportHeight, glm::mat4 viewMatrix, glm::mat4 projMatrix, glm::vec3& outDir);
	
	void Shoot(glm::vec3 worldDirection, glm::vec3 origin);

private:
	vector<Bullet> shotBullets;

	const float ClipX;
	const float ClipZ;

	Model bulletModel;
	Shader* bulletShdr;

	const float BulletVelocity = 3.0f;
	const int ClipThreshold = 100000000;
	
	int clipCounter = 0;

	void FreeClippedBullets();
};



#endif
