#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
	: front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY)
{
	pos = position;
	worldUp = up;
	this->yaw = yaw;
	this->pitch = pitch;
	updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
	: front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY)
{
	pos = glm::vec3(posX, posY, posZ);
	worldUp = glm::vec3(upX, upY, upZ);
	this->yaw = yaw;
	this->pitch = pitch;
	updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(pos, pos + front, up);
}

void Camera::ProcessKeyboard(CameraMovement direction, float deltaT)
{
	float vel = deltaT * movementSpeed;
	switch (direction)
	{
	case FORWARD:
		pos += front * vel;
		break;
	case BACKWARD:
		pos -= front * vel;
		break;
	case LEFT:
		pos -= right * vel;
		break;
	case RIGHT:
		pos += right * vel;
		break;
	}
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset, bool constraintPitch)
{
	xOffset *= mouseSensitivity;
	yOffset *= mouseSensitivity;

	yaw += xOffset;
	pitch += yOffset;

	if (constraintPitch)
	{
		if (pitch > 89.9f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}

	updateCameraVectors();
}

glm::mat4 Camera::GetProjectionMatrix()
{
	float aspectRatio = aspectX / aspectY;
	return glm::perspective(camZoom, aspectRatio, nearPlane, farPlane);
}

void Camera::SetProjectionMatrix(float camZoom, int wSize, int hSize, float nearPlane, float farPlane)
{
	int r = gcm(wSize, hSize);
	aspectX = wSize / r;
	aspectY = hSize / r;

	this->camZoom = camZoom;
	this->nearPlane = nearPlane;
	this->farPlane = farPlane;
}

int Camera::gcm(int a, int b)
{
	return (b == 0) ? a : gcm(b, a % b);
}

void Camera::updateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp)); 
	up = glm::normalize(glm::cross(right, front));
}
