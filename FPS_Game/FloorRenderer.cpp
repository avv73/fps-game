#include "FloorRenderer.h"
#include <math.h>

#include <process.h>

TransformNode** FloorRenderer::floor_arr = NULL;

FloorRenderer::FloorRenderer()
{

}

void FloorRenderer::GenerateFloor(ModelNode* sample, int width, int height, glm::vec2 startPos)
{
	int xIter = startPos.x;
	int zIter = startPos.y;

	floor_arr_cnt = width * height * 2;
	floor_arr = new TransformNode*[floor_arr_cnt];

	int arrC = 0;

	for (float xDelta = xIter; xDelta < xIter + width; xDelta += 0.5)
	{
		for (float zDelta = zIter; zDelta < zIter + height; zDelta += 0.5)
		{
			TransformNode* currentTr = new TransformNode("floor_trans");
			currentTr->Translate(glm::vec3(xDelta, -1.5f, zDelta));
			currentTr->AddNode(sample);

			floor_arr[arrC++] = currentTr;
		}
	}

	worker1.arr_cnt = floor_arr_cnt;
	worker1.shr_arr = floor_arr;
}

void FloorRenderer::Visualize()
{
	_beginthread(WorkerVisualize, 0, &worker1);
}

void __stdcall FloorRenderer::WorkerVisualize(void* workerData)
{
	RenderWorker* info = (RenderWorker*)workerData;
	
	for (size_t i = 0; i < info->arr_cnt; i++)
	{
		info->shr_arr[i]->Visualize(glm::mat4(1.0f));
	}

}
