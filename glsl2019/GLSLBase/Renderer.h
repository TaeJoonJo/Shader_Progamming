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
	void Lecture5();
	void Lecture6();
	void Lecture6_Shader(int num, bool israndom, GLuint *pvbo, int *pnvbo, float x, float y, float z);

	void Lecture7();

	void CreateGridMesh(void);
	void DrawGridMesh(void);

	void FillAll(float falpha);

	void Texture();

	void CreateCheckerBoardTexture();
	void DrawCheckerBoard();

	void DrawMultiTexture();

	void DrawNumber(int number);

	void DrawSpriteSequence(GLfloat number);

	void CreateSimpleCube();
	void DrawCube();

	void InitializeMatrix();

	void DrawTexture(float x, float y, float sx, float sy, GLuint textureID);
	void DrawHDRTexture(float x, float y, float sx, float sy, GLuint textureID);
	GLuint CreateFBO(int sx, int sy, GLuint* tex, bool isHDR = false);
	void TestFBO();

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
private:
	GLuint m_ParticleTexture = 0;
	GLuint m_ParticleTexture1 = 0;
	GLuint m_ParticleTexture2 = 0;

	GLuint m_RGBTexture = 0;

	GLuint m_LegoTexture1 = 0;
	GLuint m_LegoTexture2 = 0;
	GLuint m_LegoTexture3 = 0;
	GLuint m_LegoTexture4 = 0;
	GLuint m_LegoTexture5 = 0;

	GLuint m_SuperTexture1 = 0;
	GLuint m_SuperTexture2 = 0;
	GLuint m_SuperTexture3 = 0;
	GLuint m_SuperTexture4 = 0;
	GLuint m_SuperTexture5 = 0;

	GLuint m_CheckerBoardTexture = 0;

	GLuint m_NumbersTexture = 0;

	GLuint m_SpriteTexture = 0;

	GLuint m_FlagTexture = 0;

	GLuint m_HeightmapTextrue = 0;
	GLuint m_GrassTextrue = 0;
private:
	float m_uTime = 0;
	float m_uTimeDir = 1.f;
	float m_uUTime = 0;

	glm::vec3 m_v3CameraPos;
	glm::vec3 m_v3CameraLookAt;
	glm::vec3 m_v3CameraUp;

	glm::mat4 m_m4OrthoProj;
	glm::mat4 m_m4PersProj;

	glm::mat4 m_m4View;
	glm::mat4 m_m4ViewProj;
private:
	GLuint m_VBORect = 0;
	GLuint m_VBORectColor = 0;
	GLuint m_SolidRectShader = 0;
	
	GLuint m_SandBoxShader = 0;

	GLuint m_FillAllShader = 0;

	GLuint m_VBOCheckerBoard = 0;
	GLuint m_CheckerBoardShader = 0;

	GLuint m_VBOTexture = 0;
	GLuint m_TextureShader = 0;

	GLuint m_VBOTri = 0;
	GLuint m_SolidTriShader = 0;

	GLuint m_VBOGen = 0;
	int m_nGen = 0;
	GLuint m_ParticleShader = 0;

	int m_nPos = 0;
	GLuint m_VBOpos = 0;
	GLuint m_PosShader = 0;

	GLuint m_VBOGridMesh = 0;
	int m_nGridMesh = 0;

	GLuint m_NumberShader = 0;

	GLuint m_SpriteShader = 0;

	GLuint m_GridMeshShader = 0;

	GLuint m_VBOCube = 0;
	int m_nCube = 0;
	GLuint m_CubeShader = 0;

	GLuint m_DrawTextureShader = 0;
	GLuint m_FBOTexture0 = 0;
	GLuint m_FBOTexture1 = 0;
	GLuint m_FBOTexture2 = 0;
	GLuint m_FBOTexture3 = 0;

	GLuint m_DepthRenderBuffer = 0;
	GLuint m_FBO0;
	GLuint m_FBO1;
	GLuint m_FBO2;
	GLuint m_FBO3;

	GLuint m_DrawHDRTextureShader = 0;
};

template <typename T>
static T GetRandom(T min, T max)
{
	std::random_device rd;
	std::mt19937_64 rnd(rd());
	std::uniform_real_distribution<T> range(min, max);
	
	return range(rnd);
}