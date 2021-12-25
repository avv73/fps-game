#pragma once
#ifndef ENGINE_H
#define ENGINE_H

#include "SceneNode.h"

class Engine
{
	void Start();
private:
	SceneNode* sceneGraph;
	
	void Update();
	bool Init();
	bool InitGL();
	void CreateScene();
};


#endif
