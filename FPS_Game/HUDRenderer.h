#pragma once
#ifndef HUDRENDERER_H
#define HUDRENDERER_H

#include "Player.h"

class HUDRenderer
{
public:
	HUDRenderer(Player* pl);

	void Visualize();
private:
	Player* pl;
	glm::mat4 orthoMat;

	GLuint VAO;
	GLuint VBO;

	Shader* shd;

	void CreateHUD();
};


#endif