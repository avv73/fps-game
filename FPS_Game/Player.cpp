#include "Player.h"
#include <stdio.h>

Player::Player(Camera* camera)
{
	this->camera = camera;
}

void Player::Move(glm::vec3 offset, float delta)
{
	if (offset.x == 1.0f)
	{
		camera->ProcessKeyboard(Camera::RIGHT, delta);
	}
	else if (offset.x == -1.0f)
	{
		camera->ProcessKeyboard(Camera::LEFT, delta);
	}

	if (offset.y == 1.0f)
	{
		// jump?
	}
	
	if (offset.z == 1.0f)
	{
		camera->ProcessKeyboard(Camera::FORWARD, delta);
	}
	else if (offset.z == -1.0f)
	{
		camera->ProcessKeyboard(Camera::BACKWARD, delta);
	}
}

void Player::Look(glm::vec2 motion)
{
	if (firstMouse)
	{
		lastX = motion.x;
		lastY = motion.y;
		firstMouse = false;
	}
	else
	{
		camera->ProcessMouseMovement(motion.x - lastX, lastY - motion.y);
		lastX = motion.x;
		lastY = motion.y;
	}
}

void Player::Shoot()
{
	//SceneGraph->Shoot(...)
}