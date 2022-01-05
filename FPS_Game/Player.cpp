#include "Player.h"
#include <stdio.h>

Player::Player(Camera* camera)
{
	this->camera = camera;
}

void Player::Move(glm::vec3 offset, float delta)
{
	camera->ProcessKeyboard(Camera::RIGHT, delta, offset.x);
	camera->ProcessKeyboard(Camera::FORWARD, delta, offset.z);


	if (offset.y == 1.0f)
	{
		// jump?
	}
}

void Player::Look(glm::vec2 motion)
{
	camera->ProcessMouseMovement(motion.x, -motion.y, true);
}

void Player::Shoot()
{
	//SceneGraph->Shoot(...)
}