#pragma once

#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <time.h>

#include "Dependencies\glew.h"
#include "Dependencies\wglew.h"
#include "Dependencies\glm/glm.hpp"
#include "Dependencies\glm/gtc/matrix_transform.hpp"
#include "Dependencies\glm/gtx/euler_angles.hpp"

class Renderer
{
public:
	Renderer(int windowSizeX, int windowSizeY);
	~Renderer();

	GLuint CreatePngTexture(char * filePath);
	GLuint CreateBmpTexture(char * filePath);
	   
	void Test();
	void Lecture2();
	void Lecture2_2();
	void Lecture2_3(int num);
	void Lecture4();

	void CreateGridMesh(void);
	void DrawGridMesh(void);
private:
	void Initialize(int windowSizeX, int windowSizeY);
	bool ReadFile(char* filename, std::string *target);
	void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
	GLuint CompileShaders(char* filenameVS, char* filenameFS);
	void CreateVertexBufferObjects(); 
	unsigned char * Renderer::loadBMPRaw(const char * imagepath, unsigned int& outWidth, unsigned int& outHeight);

	bool m_Initialized = false;
	
	unsigned int m_WindowSizeX = 0;
	unsigned int m_WindowSizeY = 0;

	float m_uTime = 0;
	float m_uTimeDir = -1.f;

	GLuint m_VBORect = 0;
	GLuint m_VBORectColor = 0;
	GLuint m_SolidRectShader = 0;
	
	GLuint m_VBOTri = 0;
	GLuint m_SolidTriShader = 0;

	GLuint m_VBOGen = 0;
	int m_nGen = 0;
	GLuint m_ParticleShader = 0;

	GLuint m_VBOGridMesh = 0;
	int m_nGridMesh = 0;
};

