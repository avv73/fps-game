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
		printf("SDL could not initialized! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

		SDL_DisplayMode dMode;
		SDL_GetCurrentDisplayMode(0, &dMode);

		gWindow = SDL_CreateWindow("FPS Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, dMode.w, dMode.h,
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_MAXIMIZED);

		player->camera->SetProjectionMatrix(glm::radians(45.0f), dMode.w, dMode.h, 0.1f, 100.0f);

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
	TransformNode* trC = new TransformNode("cube_trans");
	TransformNode* trC2 = new TransformNode("cube_trans2");

	rootNode->AddNode(trC);
	rootNode->AddNode(trC2);

	trC->Translate(glm::vec3(1.0f, 0.0f, -3.0f));
	trC2->Translate(glm::vec3(1.0f, 4.0f, -4.0f));

	ModelNode* cube = new ModelNode("cube_test", "./models/cube/cube_test.obj");

	trC->AddNode(cube);
	trC2->AddNode(cube);

	rootNode->AddNode(trC);
	rootNode->AddNode(trC2);
	//cubeDeb = cube;
}

void Engine::Update()
{
	SDL_Event e;

	bool quit = false;
	while (!quit)
	{
		float currentFrame = SDL_GetTicks() / 1000.0f;
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

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
	ShaderLibrary::GetInstance()->SetGlobalLight(glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), player->camera->pos);

	SceneGraph->Visualize(glm::mat4(1.0f));
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
	actionVector = glm::vec3(0.0f);
}

void Engine::HandleMouseMotion(const SDL_MouseMotionEvent& motion)
{
	player->Look(glm::vec2(motion.x, motion.y));
}

void Engine::HandleMouseClick(const SDL_MouseButtonEvent& button)
{
	if (button.clicks != 1)
		return;

	switch (button.button)
	{
	case SDL_BUTTON_LEFT:
		player->Shoot();
		break;
	case SDL_BUTTON_RIGHT:
		break;
	}
}

void Engine::Close()
{
	ShaderLibrary::GetInstance()->UnloadShaders();
	// close program..
}

GLuint Engine::CreateCube(float width, GLuint& VBO)
{
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f


	};

	GLuint VAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); //the data comes from the currently bound GL_ARRAY_BUFFER
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	return VAO;
}

void Engine::DrawCube(GLuint vaoID)
{
	//glUseProgram(cubeShader.ID);
	glBindVertexArray(vaoID);

	//glDrawElements uses the indices in the EBO to get to the vertices in the VBO
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36); //36

	glBindVertexArray(0);
}