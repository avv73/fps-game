#include "Camera.h"
#include "Player.h"
#include "Engine.h"


int main(int argc, char* argv[])
{
	// TODO:
	// Make zombie also shoot player.

	Camera* cam = new Camera();
	Player* player = new Player(cam);
	Engine* engine = new Engine(player);

	engine->Start();

	return 0;
}