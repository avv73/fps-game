#include "Engine.h"

#include <GL\glew.h>

#include <SDL.h>
#include <SDL_opengl.h>
#include <stdio.h>
#include <gl\GLU.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtx\string_cast.hpp>

#include <iostream>
#include "ShaderLibrary.h"
#include "GLErrorLogger.h"
#include "Terrain.h"
#include "ZombieNode.h"

//#define FPS_COUNT

Engine::Engine(Player* pl)
	: player(pl)
{ }

void Engine::Start()
{
	bool status = Init();

	if (!status)
	{
		printf("Fatal error when initializing engine! Quitting...\n");
		return;
	}

	CreateScene();
	Update();
}

bool Engine::Init()
{
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not be initialized! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

		//SDL_SetHintWithPriority(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "1", SDL_HINT_OVERRIDE);

		/*SDL_DisplayMode dMode;
		SDL_GetCurrentDisplayMode(0, &dMode);
		printf("%d x %d\n", dMode.w, dMode.h);*/

		gWindow = SDL_CreateWindow("FPS Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0,
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_MAXIMIZED ); // SDL_WINDOW_MAXIMIZED

		/*int w, h;

		SDL_GetWindowSize(gWindow, &w, &h);
		printf("%d x %d\n", w, h);*/

		int w, h;
		SDL_GL_GetDrawableSize(gWindow, &w, &h);

		player->camera->SetProjectionMatrix(glm::radians(45.0f), w, h, 0.1f, 100.0f);
		
		SDL_SetRelativeMouseMode(SDL_TRUE);
		SDL_WarpMouseInWindow(gWindow, w / 2, h / 2);

		if (gWindow == NULL)
		{
			printf("Window creation error! SDL error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{

			SDL_GLContext gContext = SDL_GL_CreateContext(gWindow);
			if (gContext == NULL)
			{
				printf("OpenGL context cannot be created! SDL error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				if (SDL_GL_SetSwapInterval(1) < 0)
				{
					printf("Warning! Unable to set VSync. SDL error: %s\n", SDL_GetError());
				}

				if (!InitGL())
				{
					printf("Unable to inialize OpenGL!\n");
					success = false;
				}
			}
		}

		ShaderLibrary* shLib = ShaderLibrary::GetInstance();
		shLib->SetShaderPath("./shaders/");

		if (!shLib->LoadShaders())
		{
			printf("Error loading shaders!");
			success = false;
		}

		bulletEngine = new BulletEngine(250, 250);
	}

	return success;
}

bool Engine::InitGL()
{
	bool success = true;
	GLenum error = GL_NO_ERROR;

	glewInit();

	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		success = false;
		printf("Error initializing OpenGL! %s\n", gluErrorString(error));
	}

	glClearColor(0.72f, 0.27f, 0.27f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//cubeShader.Load("cube_vertex.vert", "cube_fragment.frag");

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	return success;
}

void Engine::CreateScene()
{
	// shaders, model & stuff..
	// create scene graph...
	//GLuint VBO;
	//cube = CreateCube(1.0f, VBO);

	GroupNode* rootNode = new GroupNode("root");

	// add player node
	PlayerNode* pl = new PlayerNode(player);
	player->SetPlayerNode(pl);

	rootNode->AddNode(pl);

	//ModelNode* zombie = new ModelNode("zombie", "./models/zombie/zombie.obj");
	TransformNode* trZombie = new TransformNode("zombie_transf");
	trZombie->translateVector = glm::vec3(1.0f, -1.0f, 3.0f);
	trZombie->rotateVector = glm::vec3(0.0f, 1.0f, 0.0f);
	trZombie->rotateAngleRad = glm::radians(-170.0f);

	Zombie* zombie = new Zombie(trZombie, player, bulletEngine);
	ZombieNode* zombieNode = new ZombieNode(zombie);

	zombie->SetSceneNode(zombieNode);

	ModelNode* crate = new ModelNode("crate", "./models/crate/Wooden Crate.obj");
	TransformNode* trCrate = new TransformNode("crate_transf");
	TransformNode* trCrate2 = new TransformNode("crate_transf2");
	TransformNode* trCrate3 = new TransformNode("crate_transf3");
	TransformNode* trCrate4 = new TransformNode("crate_transf4");

	GroupNode* crates = new GroupNode("crates");

	trZombie->AddNode(zombieNode);

	trCrate->translateVector = glm::vec3(3.0f, -1.0f, 10.0f);
	trCrate->scaleVector = glm::vec3(0.3f, 0.3f, 0.3f);
	trCrate->AddNode(crate);

	trCrate2->translateVector = glm::vec3(3.0f, -1.0f, -5.0f);
	trCrate2->scaleVector = glm::vec3(0.3f, 0.3f, 0.3f);
	trCrate2->AddNode(crate);

	trCrate3->translateVector = glm::vec3(-4.0f, -1.0f, -6.0f);
	trCrate3->scaleVector = glm::vec3(0.3f, 0.3f, 0.3f);
	trCrate3->AddNode(crate);

	trCrate4->translateVector = glm::vec3(0.0f, -1.0f, 12.0f);
	trCrate4->scaleVector = glm::vec3(0.3f, 0.3f, 0.3f);
	trCrate4->AddNode(crate);

	crates->AddNode(trCrate);
	crates->AddNode(trCrate2);
	crates->AddNode(trCrate3);
	crates->AddNode(trCrate4);

	Terrain* terrain = new Terrain(glm::vec2(-20, -20), 40);
	//Terrain* terrain = new Terrain(glm::vec2(0, 0), 2);

	rootNode->AddNode(terrain);
	rootNode->AddNode(trZombie);
	rootNode->AddNode(crates);

	//cubeDeb = cube;

	skybox = new CubemapNode("./skybox/top.jpg", "./skybox/left.jpg", "./skybox/right.jpg", "./skybox/bottom.jpg", "./skybox/front.jpg", "./skybox/back.jpg");
	hudRenderer = new HUDRenderer(player);

	zombies.push_back(zombie);
}

void Engine::Update()
{
	SDL_Event e;

	float startTime = SDL_GetTicks() / 1000.0f;
	int frames = 0;

	bool quit = false;
	while (!quit)
	{
		float currentFrame = SDL_GetTicks() / 1000.0f;
		frames++;

		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

#ifdef FPS_COUNT
		if (currentFrame - startTime >= 1.0)
		{
			printf("%f ms/frame\n", 1000.0f / float(frames));
			frames = 0;
			startTime += 1.0f;
		}
#endif
		while (SDL_PollEvent(&e) != 0)
		{
			switch (e.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_MOUSEMOTION:
				currentMouseX = e.motion.x;
				currentMouseY = e.motion.y;
				HandleMouseMotion(e.motion);
				
				SDL_WarpMouseInWindow(gWindow, 500, 500);
				break;
			case SDL_MOUSEBUTTONDOWN:
				HandleMouseClick(e.button);
				break;
			}
		}

		int count;

		const Uint8* keystates = SDL_GetKeyboardState(&count);
		HandleKeyDown(keystates);

		UpdateActions();

		Render();

		SDL_GL_SwapWindow(gWindow);
	}

	// close();
}

void Engine::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 view = player->camera->GetViewMatrix();
	glm::mat4 proj = player->camera->GetProjectionMatrix();

	ShaderLibrary::GetInstance()->SetPVGlobal(proj, view);
	ShaderLibrary::GetInstance()->SetGlobalLight(glm::vec3(-100.0f, 100.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), player->camera->pos);

	skybox->Visualize();
	SceneGraph->Visualize(glm::mat4(1.0f));
	bulletEngine->Visualize();
	hudRenderer->Visualize();
}

void Engine::HandleKeyDown(const Uint8* keystates)
{
	if (keystates[SDL_GetScancodeFromKey(SDLK_w)])
	{
		actionVector.z += 1.0f;
	}
	if (keystates[SDL_GetScancodeFromKey(SDLK_s)])
	{
		actionVector.z -= 1.0f;
	}
	if (keystates[SDL_GetScancodeFromKey(SDLK_a)])
	{
		actionVector.x -= 1.0f;
	}
	if (keystates[SDL_GetScancodeFromKey(SDLK_d)])
	{
		actionVector.x += 1.0f;
	}
	if (keystates[SDL_GetScancodeFromKey(SDLK_SPACE)])
	{
		actionVector.y += 1.0f;
	}
	if (keystates[SDL_GetScancodeFromKey(SDLK_ESCAPE)])
	{
		Close();
	}
}

void Engine::UpdateActions()
{
	player->Move(actionVector, deltaTime);
	if (actionVector.y == 1.0f)
		player->Jump();

	actionVector = glm::vec3(0.0f);

	player->UpdateGravity(deltaTime); 
	for (auto it = zombies.begin(); it != zombies.end(); ++it)
	{
		(*it)->Update(deltaTime);
	}

	bulletEngine->Update(deltaTime);
}

void Engine::HandleMouseMotion(const SDL_MouseMotionEvent& motion)
{
	player->Look(glm::vec2(motion.xrel, motion.yrel));
}

void Engine::HandleMouseClick(const SDL_MouseButtonEvent& button)
{
	if (button.clicks != 1)
		return;

	switch (button.button)
	{
	case SDL_BUTTON_LEFT:
		player->Shoot(bulletEngine);
		break;
	case SDL_BUTTON_RIGHT:
		break;
	}
}

void Engine::Close()
{
	ShaderLibrary::GetInstance()->UnloadShaders();
	// close program..
	exit(0);
}