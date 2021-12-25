#include "Engine.h"

#include <GL\glew.h>

#include <SDL.h>
#include <SDL_opengl.h>
#include <stdio.h>
#include <gl\GLU.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <iostream>

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

		SDL_Window* gWindow = SDL_CreateWindow("FPS Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, dMode.w, dMode.h,
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_MAXIMIZED);

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

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return success;
}

void Engine::CreateScene()
{
	// shaders, model & stuff..
	// create scene graph...
}

void Engine::Update()
{
	SDL_Event e;

	bool quit = false;
	while (!quit)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			switch (e.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				if (e.key.keysym.sym == SDLK_ESCAPE)
				{
					quit = true;
				}
				else
				{

				}


				break;
			}
		}
	}
}