#include "Player.h"
#include <stdio.h>
#include <math.h>

Player::Player(Camera* camera)
	: health_max(5), ammo_max(5)
{
	this->camera = camera;
	this->isInAir = false;
	this->yVelocity = 0.0f;

	this->health = 5;
	this->ammo = 5;
}

void Player::Move(glm::vec3 offset, float delta)
{
	camera->ProcessKeyboard(Camera::RIGHT, delta, offset.x);
	camera->ProcessKeyboard(Camera::FORWARD, delta, offset.z);

	plNode->UpdateBoundingPosition(camera->pos);
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

	plNode->UpdateBoundingPosition(camera->pos);
	ReloadTick(delta);
}

void Player::ReloadTick(float delta)
{
	if (ammo == 0)
	{
		reloadTicks += delta;
	}

	if (reloadTicks >= reloadMaxTicks)
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

	// increase orig a tiny bit, so it doesn't intersect with playernode
	//orig.x += 0.2 * direction.x;
	//orig.y += 0.2 * direction.y;
	//orig.z += 0.2 * direction.z;

	printf("Direction: %f %f %f\n", direction.x, direction.y, direction.z);

	bEngine->Shoot(direction, orig, camera->yaw - YAW, camera->pitch); // lazy

	ammo--;
}

void Player::SetPlayerNode(PlayerNode* plNode)
{
	this->plNode = plNode;
}

void Player::DecreaseHealth()
{
	if (health == 0)
	{
		// game over
		printf("GAME OVER!");
		//exit(1);
	}

	health--;
}

int Player::GetHealth()
{
	return health;
}

int Player::GetAmmo()
{
	return ammo;
}