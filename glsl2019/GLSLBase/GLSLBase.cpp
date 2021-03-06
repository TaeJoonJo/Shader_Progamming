/*
Copyright 2018 Lee Taek Hee (Korea Polytech University)

This program is free software: you can redistribute it and/or modify
it under the terms of the What The Hell License. Do it plz.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY.
*/

#include "stdafx.h"
#include <iostream>
#include "Dependencies\glew.h"
#include "Dependencies\freeglut.h"

#include "Renderer.h"

Renderer *g_Renderer = NULL;

int g_WindowSizeX = 1024;
int g_WindowSizeY = 1024;
int g_number;
float g_fnumber = 0.f;

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.f, 0.f, 1.0f);
	//g_Renderer->FillAll(0.2f);

	//g_Renderer->DrawMultiTexture();
	g_fnumber += .04f;
	//g_Renderer->DrawNumber(static_cast<int>(g_fnumber));
	//g_Renderer->DrawSpriteSequence((g_fnumber));
	//g_Renderer->DrawGridMesh();
	//g_Renderer->DrawCube();
	//g_Renderer->DrawTexture(.4f, .4f, -.8f, -.8f);
	//g_Renderer->TestFBO();

	g_Renderer->DrawCheckerBoard();

	//g_Renderer->Test();
	//g_Renderer->Lecture2_2();
	//g_Renderer->Lecture4();
	//g_Renderer->Lecture5();
	//g_Renderer->Lecture6();
	//g_Renderer->Lecture7();
	
	//g_Renderer->Texture();
	
	glutSwapBuffers();
}

void Idle(void)
{
	RenderScene();
}

void MouseInput(int button, int state, int x, int y)
{
	RenderScene();
}

void KeyInput(unsigned char key, int x, int y)
{
	RenderScene();
}

void SpecialKeyInput(int key, int x, int y)
{
	RenderScene();
}

int main(int argc, char **argv)
{
	// Initialize GL things
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(600, 0);
	glutInitWindowSize(g_WindowSizeX, g_WindowSizeY);
	glutCreateWindow("GLSL KPU");

	glewInit();
	if (glewIsSupported("GL_VERSION_4_6"))
	{
		std::cout << " GL Version is 4.6\n ";
	}
	else
	{
		std::cout << "GLEW 4.6 not supported\n ";
	}

	// Initialize Renderer
	g_Renderer = new Renderer(g_WindowSizeX, g_WindowSizeY);

	glutDisplayFunc(RenderScene);
	glutIdleFunc(Idle);
	glutKeyboardFunc(KeyInput);
	glutMouseFunc(MouseInput);
	glutSpecialFunc(SpecialKeyInput);

	glutMainLoop();

	delete g_Renderer;

    return 0;
}

