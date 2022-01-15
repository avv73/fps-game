#include "Player.h"
#include <stdio.h>
#include <math.h>

Player::Player(Camera* camera)
	: health_max(5), ammo_max(5)
{
	this->camera = camera;
	this->yVelocity = yVelocity;
	this->isInAir = false;
	this->yVelocity = 0.0f;
}

void Player::Move(glm::vec3 offset, float delta)
{
	camera->ProcessKeyboard(Camera::RIGHT, delta, offset.x);
	camera->ProcessKeyboard(Camera::FORWARD, delta, offset.z);
}

void Player::Look(glm::vec2 motion)
{
	camera->ProcessMouseMovement(motion.x, -motion.y, true);
}

void Player::Jump()
{
	if (isInAir)
		return;

	yVelocity = 2.0f;
	isInAir = true;
}

void Player::UpdateGravity(float delta)
{
	camera->pos.y += yVelocity * delta;
	yVelocity -= Gravity * delta;
	
	if (camera->pos.y < 0.01f)
	{
		camera->pos.y = 0;
		yVelocity = 0.0f;
		isInAir = false;
	}

	ReloadTick();
}

void Player::ReloadTick()
{
	if (ammo == 0)
	{
		reloadTicks++;
	}

	if (reloadTicks == reloadMaxTicks)
	{
		reloadTicks = 0;
		ammo = ammo_max;
	}
}

void Player::Shoot(BulletEngine* bEngine)
{
	if (ammo == 0)
	{
		// wait for reload
		return;
	}

	glm::vec3 direction;
	glm::vec3 orig = camera->pos;

	BulletEngine::ScreenCenterToWorldRay(camera->GetViewMatrix(), camera->GetProjectionMatrix(), direction);

	printf("Direction: %f %f %f\n", direction.x, direction.y, direction.z);

	bEngine->Shoot(direction, orig, camera->yaw - YAW, camera->pitch); // lazy

	std::vector<Intersection*> inters;
	SceneGraph->TraverseIntersection(orig, direction, inters);

	if (inters.size() > 0)
	{
		printf("===Intersected!\n");
		std::vector<SceneNode*> path = inters[0]->intersectionPath;
		for (auto it = path.begin(); it != path.end(); ++it)
		{
			printf("->%s", (*it)->NodeName.c_str());
		}
		printf("\n");
	}

	ammo--;
}