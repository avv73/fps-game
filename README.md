# FPS Game
<p align="center"><img src="game.png" width="600" height="400"></p>
A simple first-person shooter (FPS) C++ game I made as a coursework for my university computer graphics course. 

The objective is to kill the zombie, but the zombie shoots the player at fixed intervals of time, so the player should hide! The player also can shoot him, but should mind his remaining health & ammo.

## Libraries used
* OpenGL Core 3.3
* GLM 0.9.9
* Assimp 3
* SDL2

## Functionality
- [X] - **Bullet simulation** - Simulates bullets movement through the scene & bullet collisions with objects.
- [X] - **Ammuniation/health HUD** - HUD shows remaining health and ammo of the player.
- [X] - **Player shooting** - The player can shoot at the zombie; bullet direction, velocity, yaw, pitch is calculated accordingly.
- [X] - **Locking and shooting** - Zombie rotates as the player moves through the scene; at fixed intervals will lock the player's position and shoot at him.
- [X] - **Gravity** - Player can jump.

## Configuration
Scene configuration can be modified in the `void Engine::CreateScene()` method. Keep in mind how the scene graph is set up; each node should be linked to root node manually.

## Additional functionality to be added/fixed
I would really appreciate any contributions to the game - many of the things are implemented very straightforward (tried to reuse as much code from the lectures as I can) and if you have any ideas, you can always hit me up. 

- [ ] - **Custom math library** - Should improve performance somewhat.
- [ ] - **Improved scene graph implementation**
- [ ] - **Code/class refactoring**
- [ ] - **Use bounding box instead of sphere** - I am currently using sphere colliders, there is implementation of AAB bounding volumes
- [ ] - **Physics** - Collision detection with objects, etc.


