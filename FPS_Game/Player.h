#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "SceneNode.h"
#include <glm/glm.hpp>
#include "Camera.h"
#include "BulletEngine.h"
class Player
{
public:
	Camera* camera;

	Player(Camera* camera);


	void Move(glm::vec3 offset, float delta);
	void Jump();
	void Look(glm::vec2 motion);
	void Shoot(BulletEngine* bEngine);

	void UpdateGravity(float delta);

	int health;
	const int health_max;
	int ammo;

private:
	float yVelocity;
	
	const float Gravity = 5.0f;

	bool isInAir;
};


#endif
