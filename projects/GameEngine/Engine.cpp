#include "Engine.h"



Engine::Engine()
{
}


Engine::~Engine()
{
}

void Engine::display(float deltaTime)
{
	++FrameCount;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawScene();
	glutSwapBuffers();
}

void Engine::displayWrapper()
{
}
