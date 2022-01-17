#include "Zombie.h"
#include "ZombieNode.h"
#include <cmath>

Zombie::Zombie(TransformNode* trN, Player* n, BulletEngine* bulletEngine)
{
	transformN = trN;
	player = n;

	zombiePos = trN->translateVector;
	forwardVector = n->camera->pos - zombiePos;

	health = 30;
	shootYaw = -110.0 - YAW;

	this->bulletEngine = bulletEngine;
}

void Zombie::SetSceneNode(ZombieNode* zombieN)
{
	this->zombieN = zombieN;
}

void Zombie::DecreaseHealth()
{
	if (health <= 0)
		return;
	health--;
}

int Zombie::GetHealth()
{
	return health;
}

void Zombie::Update(float delta)
{
	if (health <= 0)
		return;

	shootTicks += delta;
	lockTicks += delta;

	if (lockTicks >= lockTicksMax)
	{
		previousForwardVector = forwardVector;
		lockTicks = 0;
	}

	if (shootTicks >= shootTicksMax)
	{
		Shoot();
		shootTicks = 0;
	}

	glm::vec3 currentVector = player->camera->pos - zombiePos;

	float angle = glm::dot(forwardVector, currentVector) / (glm::length(currentVector) * glm::length(forwardVector));

	if (angle > 1.0f)
	{
		angle = 1.0f;
	}
	
	if (angle < -1.0f)
	{
		angle = -1.0f;
	}

	glm::vec3 hand = glm::cross(forwardVector, currentVector);

	angle = acos(angle);

	if (hand.y >= 0.001f)
	{
		transformN->rotateAngleRad += angle;
		shootYaw += glm::degrees(angle);
	}
	else if (hand.y < 0.001f)
	{
		transformN->rotateAngleRad -= angle;
		shootYaw -= glm::degrees(angle);
	}

	forwardVector = currentVector;

	//printf("%f\n", angle);
	
}

void Zombie::Shoot()
{
	bulletEngine->Shoot(previousForwardVector, zombiePos, shootYaw, 0.0f);
}