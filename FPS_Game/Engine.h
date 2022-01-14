#pragma once
#ifndef ENGINE_H
#define ENGINE_H

#include <GL\glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include "SceneNode.h"
#include "Player.h"
#include "Shader.h"
#include "CubemapNode.h"
#include "HUDRenderer.h"
#include "BulletEngine.h"

class Engine
{
public:
	Engine(Player* pl);

	void Start();
private:
	SDL_Window* gWindow;

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	glm::vec3 actionVector = glm::vec3(0.0f);

	Player* player;
	CubemapNode* skybox;
	HUDRenderer* hudRenderer;
	BulletEngine* bulletEngine;
	//FloorRenderer* floorRenderer;

	float currentMouseX;
	float currentMouseY;
	
	void Update();
	void UpdateActions();

	bool Init();
	bool InitGL();

	void Close();

	void CreateScene();

	void HandleKeyDown(const Uint8* keystates);
	void HandleMouseMotion(const SDL_MouseMotionEvent& motion);
	void HandleMouseClick(const SDL_MouseButtonEvent& button);

	void Render();

	// DEBUG
	GLuint CreateCube(float width, GLuint& VBO);
	void DrawCube(GLuint vaoID);
	//Shader cubeShader;
	//GLuint cube;

	ModelNode* cubeDeb;

	bool deb = true;
};


#endif
