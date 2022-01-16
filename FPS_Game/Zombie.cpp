#include "Zombie.h"
#include "ZombieNode.h"

Zombie::Zombie(TransformNode* trN, Player* n)
{
	transformN = trN;
	player = n;

	zombiePos = trN->translateVector;
	forwardVector = n->camera->pos - zombiePos;

	health = 30;
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

	if (hand.y > 0.001f)
		transformN->rotateAngleRad += angle;
	else if (hand.y < 0.001f)
		transformN->rotateAngleRad -= angle;

	forwardVector = currentVector;

	//printf("%f\n", angle);
	
}