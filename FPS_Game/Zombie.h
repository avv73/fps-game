#pragma once
#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "SceneNode.h"
#include "Player.h"

class ZombieNode;

class Zombie
{
public:
	Zombie(TransformNode* trN, Player* n);

	void Update(float delta);
	void SetSceneNode(ZombieNode* zNode);

	void DecreaseHealth();
	int GetHealth();
private:

	TransformNode* transformN;
	ZombieNode* zombieN;
	Player* player;

	int health;

	glm::vec3 forwardVector;
	glm::vec3 zombiePos;
	glm::vec3 hand;

	void Shoot();
};

#endif