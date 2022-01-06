#pragma once
#ifndef FLOORRENDERER_H
#define FLOORRENDERER_H

#include "SceneNode.h"

struct RenderWorker {
	int arr_cnt;
	TransformNode** shr_arr;
};

class FloorRenderer 
{
public:
	FloorRenderer();
	void GenerateFloor(ModelNode* sample, int width, int height, glm::vec2 startPos);
	void Visualize();

	static TransformNode** floor_arr;
private:
	int floor_arr_cnt;
	RenderWorker worker1;

	static void __stdcall WorkerVisualize(void* workerData);
};
 // TODO: Optimze, keep floor rendering away from scene graph - make it in another structure (array?)
// if no performance imporvement => multithread
#endif