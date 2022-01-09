#pragma once
#include "SceneNode.h"
#include <glm/glm.hpp>
#include "Camera.h"
#ifndef PLAYER_H
#define PLAYER_H

class Player
{
public:
	Camera* camera;

	Player(Camera* camera);

	void Move(glm::vec3 offset, float delta);
	void Look(glm::vec2 motion);
	void Shoot();

	int health;
	const int health_max;
	int ammo;

private:
};


#endif
