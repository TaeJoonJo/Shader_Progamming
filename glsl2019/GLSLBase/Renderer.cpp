#include "stdafx.h"
#include "Renderer.h"
#include "LoadPng.h"
#include <Windows.h>
#include <cstdlib>
#include <cassert>

Renderer::Renderer(int windowSizeX, int windowSizeY)
{
	Initialize(windowSizeX, windowSizeY);
}


Renderer::~Renderer()
{
}

void Renderer::Initialize(int windowSizeX, int windowSizeY)
{
	srand(time(NULL));
	
	//Set window size
	m_WindowSizeX = windowSizeX;
	m_WindowSizeY = windowSizeY;

	//Load shaders
	m_SolidRectShader = CompileShaders("./Shaders/SolidRect.vs", "./Shaders/SolidRect.fs");
	m_SolidTriShader = CompileShaders("./Shaders/SolidRect.vs", "./Shaders/SolidRect.fs");
	m_ParticleShader = CompileShaders("./Shaders/Lecture3_Particle.vs", "./Shaders/Lecture3_Particle.fs");
	m_PosShader = CompileShaders("./Shaders/Lecture3_3.vs", "./Shaders/Lecture3_3.fs");

	m_SandBoxShader = CompileShaders("./Shaders/SandBox.vs", "./Shaders/SandBox.fs");
	//Create VBOs
	CreateVertexBufferObjects();
}

void Renderer::CreateVertexBufferObjects()
{
	//float rect[]
	//	=
	//{
	//	-0.5, -0.5, 0.f, -0.5, 0.5, 0.f, 0.5, 0.5, 0.f, //Triangle1
	//	-0.5, -0.5, 0.f,  0.5, 0.5, 0.f, 0.5, -0.5, 0.f, //Triangle2
	//};
	float fsize = 0.5f;
	float rect[]
		=
	{
		-fsize, -fsize, 0.f, 0.5f, 0.f, 0.f,				// x, y, z, value, u, v
		-fsize,  fsize, 0.f, 0.5f, 0.f, 1.f,  
		 fsize,  fsize, 0.f, 0.5f, 1.f, 1.f,				//Triangle1
		-fsize, -fsize, 0.f, 0.5f, 0.f, 0.f,  
		 fsize,  fsize, 0.f, 0.5f, 1.f, 1.f,  
		 fsize, -fsize, 0.f, 0.5f, 1.f, 0.f					//Triangle2
	};
	//glEnableVertexArrayAttrib()
	glGenBuffers(1, &m_VBORect);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);

	float rectColor[]
		=
	{
		1.f, 0.f, 0.f, 1.f,
		1.f, 0.f, 0.f, 1.f,
		1.f, 0.f, 0.f, 1.f, //Triangle1
		1.f, 0.f, 0.f, 1.f,
		1.f, 0.f, 0.f, 1.f,
		1.f, 0.f, 0.f, 1.f	//Triangle2
	};
	//glEnableVertexArrayAttrib()
	glGenBuffers(1, &m_VBORectColor);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORectColor);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectColor), rectColor, GL_STATIC_DRAW);

	//// Lecture02
	//float tri[]
	//	=
	//{
	//	-0.5f,-0.5f,0.f, 0.f,0.5f,0.f, 0.5f,-0.5f,0.f
	//};
	//glGenBuffers(1, &m_VBOTri);
	//glBindBuffer(GL_ARRAY_BUFFER, m_VBOTri);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(tri), tri, GL_STATIC_DRAW);

	Lecture2_3(100);

	Lecture6_Shader(100, false, &m_VBOpos, &m_nPos, 0, 0, 0);

	CreateGridMesh();
}

void Renderer::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	//쉐이더 오브젝트 생성
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = (GLint)strlen(pShaderText);
	//쉐이더 코드를 쉐이더 오브젝트에 할당
	glShaderSource(ShaderObj, 1, p, Lengths);

	//할당된 쉐이더 코드를 컴파일
	glCompileShader(ShaderObj);

	GLint success;
	// ShaderObj 가 성공적으로 컴파일 되었는지 확인
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];

		//OpenGL 의 shader log 데이터를 가져옴
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	// ShaderProgram 에 attach!!
	glAttachShader(ShaderProgram, ShaderObj);
}

bool Renderer::ReadFile(char* filename, std::string *target)
{
	std::ifstream file(filename);
	if (file.fail())
	{
		std::cout << filename << " file loading failed.. \n";
		file.close();
		return false;
	}
	std::string line;
	while (getline(file, line)) {
		target->append(line.c_str());
		target->append("\n");
	}
	return true;
}

GLuint Renderer::CompileShaders(char* filenameVS, char* filenameFS)
{
	GLuint ShaderProgram = glCreateProgram(); //빈 쉐이더 프로그램 생성

	if (ShaderProgram == 0) { //쉐이더 프로그램이 만들어졌는지 확인
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vs, fs;

	//shader.vs 가 vs 안으로 로딩됨
	if (!ReadFile(filenameVS, &vs)) {
		printf("Error compiling vertex shader\n");
		return -1;
	};

	//shader.fs 가 fs 안으로 로딩됨
	if (!ReadFile(filenameFS, &fs)) {
		printf("Error compiling fragment shader\n");
		return -1;
	};

	// ShaderProgram 에 vs.c_str() 버텍스 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

	// ShaderProgram 에 fs.c_str() 프레그먼트 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	//Attach 완료된 shaderProgram 을 링킹함
	glLinkProgram(ShaderProgram);

	//링크가 성공했는지 확인
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		// shader program 로그를 받아옴
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error linking shader program\n" << ErrorLog;
		return -1;
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error validating shader program\n" << ErrorLog;
		return -1;
	}

	glUseProgram(ShaderProgram);
	std::cout << filenameVS << ", " << filenameFS << " Shader compiling is done.\n";

	return ShaderProgram;
}
unsigned char * Renderer::loadBMPRaw(const char * imagepath, unsigned int& outWidth, unsigned int& outHeight)
{
	std::cout << "Loading bmp file " << imagepath << " ... " << std::endl;
	outWidth = -1;
	outHeight = -1;
	// Data read from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	// Actual RGB data
	unsigned char * data;

	// Open the file
	FILE * file = NULL;
	fopen_s(&file, imagepath, "rb");
	if (!file)
	{
		std::cout << "Image could not be opened, " << imagepath << " is missing. " << std::endl;
		return NULL;
	}

	if (fread(header, 1, 54, file) != 54)
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	if (header[0] != 'B' || header[1] != 'M')
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	if (*(int*)&(header[0x1E]) != 0)
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	if (*(int*)&(header[0x1C]) != 24)
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	outWidth = *(int*)&(header[0x12]);
	outHeight = *(int*)&(header[0x16]);

	if (imageSize == 0)
		imageSize = outWidth * outHeight * 3;

	if (dataPos == 0)
		dataPos = 54;

	data = new unsigned char[imageSize];

	fread(data, 1, imageSize, file);

	fclose(file);

	std::cout << imagepath << " is succesfully loaded. " << std::endl;

	return data;
}

GLuint Renderer::CreatePngTexture(char * filePath)
{
	//Load Pngs: Load file and decode image.
	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, filePath);
	if (error != 0)
	{
		lodepng_error_text(error);
		assert(error == 0);
		return -1;
	}

	GLuint temp;
	glGenTextures(1, &temp);

	glBindTexture(GL_TEXTURE_2D, temp);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

	return temp;
}

GLuint Renderer::CreateBmpTexture(char * filePath)
{
	//Load Bmp: Load file and decode image.
	unsigned int width, height;
	unsigned char * bmp
		= loadBMPRaw(filePath, width, height);

	if (bmp == NULL)
	{
		std::cout << "Error while loading bmp file : " << filePath << std::endl;
		assert(bmp != NULL);
		return -1;
	}

	GLuint temp;
	glGenTextures(1, &temp);

	glBindTexture(GL_TEXTURE_2D, temp);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bmp);

	return temp;
}

void Renderer::Test()
{
	glUseProgram(m_SolidRectShader);

	GLuint uTime = glGetUniformLocation(m_SolidRectShader, "u_Time");

	m_uTime += 0.01f * m_uTimeDir;
	/*if (m_uTime >= 3.f)
		m_uTimeDir *= -1.f;
	else if(m_uTime < 0.f)
		m_uTimeDir *= -1.f;
	*/
	glUniform1f(uTime, m_uTime);

	GLuint attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glVertexAttribPointer(attribPosition, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);		// Location - x y z

	GLuint attribColor = glGetAttribLocation(m_SolidRectShader, "a_Color");
	glEnableVertexAttribArray(attribColor);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORectColor);
	glVertexAttribPointer(attribColor, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);		// Color - r g b

	glDrawArrays(GL_TRIANGLES, 0, 6);	// 6개의 버텍스를 그려라

	glDisableVertexAttribArray(attribPosition);
	glDisableVertexAttribArray(attribColor);
}

void Renderer::Lecture2()
{
	glUseProgram(m_SolidTriShader);

	int attribPosition = glGetAttribLocation(m_SolidTriShader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTri);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(attribPosition);
}

void Renderer::Lecture2_2()
{
	glUseProgram(m_SolidRectShader);

	GLuint uTime = glGetUniformLocation(m_SolidRectShader, "u_Time");

	m_uTime += 0.01f * m_uTimeDir;

	glUniform1f(uTime, m_uTime);

	int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOGen);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, m_nGen * 6 * 3);

	glDisableVertexAttribArray(attribPosition);
}

void Renderer::Lecture2_3(int num)
{
	glUseProgram(m_SolidRectShader);

	m_nGen = num;
	
	float rectSize = 0.03f;
	int verticePerQuad = 6;
	int floatsPerVertex = 3 + 3 + 2;

	int vertexArraySize = num * floatsPerVertex * verticePerQuad;
	float *genVertex = new float[vertexArraySize] {};
	int idx = 0;
	std::random_device rd;
	std::mt19937_64 rnd(rd());
	std::uniform_int_distribution<int> range(0.f, 6.f);
	range(rnd);

	for (int i = 1; i <= num; ++i)
	{
		float emitTimeMax = 6.f;
		float lifeTimeMax = 3.f;

		float randX = 2.f * (((float)rand() / (float)(RAND_MAX)) - 0.5f);
		float randY = 2.f * (((float)rand() / (float)(RAND_MAX)) - 0.5f);
		float randvelx = 2.f * (((float)rand() / (float)(RAND_MAX)) - 0.5f);
		float randvely = 2.f * (((float)rand() / (float)(RAND_MAX)) - 0.5f);
		float randvelz = 0.f;
		float randgx = 2.f * (((float)rand() / (float)(RAND_MAX)) - 0.5f);
		float randgy = 2.f * (((float)rand() / (float)(RAND_MAX)) - 0.5f);
		float randgz = 0.f;
		float randemit = (float)(((float)rand() / (float)RAND_MAX) * emitTimeMax);
		float randlife = (float)(((float)rand() / (float)RAND_MAX) * lifeTimeMax) + 1.f;

		genVertex[idx++] = randX + rectSize;
		genVertex[idx++] = randY + rectSize;
		genVertex[idx++] = 0.f;
		genVertex[idx++] = randvelx;
		genVertex[idx++] = randvely;
		genVertex[idx++] = randvelz;
		genVertex[idx++] = randemit;
		genVertex[idx++] = randlife;
		genVertex[idx++] = randX - rectSize;
		genVertex[idx++] = randY + rectSize;
		genVertex[idx++] = 0.f;
		genVertex[idx++] = randvelx;
		genVertex[idx++] = randvely;
		genVertex[idx++] = randvelz;
		genVertex[idx++] = randemit;
		genVertex[idx++] = randlife;
		genVertex[idx++] = randX - rectSize;
		genVertex[idx++] = randY - rectSize;
		genVertex[idx++] = 0.f;
		genVertex[idx++] = randvelx;
		genVertex[idx++] = randvely;
		genVertex[idx++] = randvelz;
		genVertex[idx++] = randemit;
		genVertex[idx++] = randlife;
		
		genVertex[idx++] = randX + rectSize;
		genVertex[idx++] = randY - rectSize;
		genVertex[idx++] = 0.f;
		genVertex[idx++] = randvelx;
		genVertex[idx++] = randvely;
		genVertex[idx++] = randvelz;
		genVertex[idx++] = randemit;
		genVertex[idx++] = randlife;
		genVertex[idx++] = randX + rectSize;
		genVertex[idx++] = randY + rectSize;
		genVertex[idx++] = 0.f;
		genVertex[idx++] = randvelx;
		genVertex[idx++] = randvely;
		genVertex[idx++] = randvelz;
		genVertex[idx++] = randemit;
		genVertex[idx++] = randlife;
		genVertex[idx++] = randX - rectSize;
		genVertex[idx++] = randY - rectSize;
		genVertex[idx++] = 0.f;
		genVertex[idx++] = randvelx;
		genVertex[idx++] = randvely;
		genVertex[idx++] = randvelz;
		genVertex[idx++] = randemit;
		genVertex[idx++] = randlife;
	}
	glGenBuffers(1, &m_VBOGen);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOGen);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexArraySize, genVertex, GL_STATIC_DRAW);

	delete[] genVertex;
}

void Renderer::Lecture6_Shader(int num, bool israndom, GLuint *pvbo, int *pnvbo, float x, float y, float z)
{
	glUseProgram(m_SolidRectShader);

	*pnvbo = num;

	float rectSize = 0.05f;
	int verticePerQuad = 6;
	int floatsPerVertex = 3 + 3 + 2 + 2 + 1 + 4;

	int vertexArraySize = num * floatsPerVertex * verticePerQuad;
	float *genVertex = new float[vertexArraySize] {};
	int idx = 0;
	

	for (int i = 1; i <= num; ++i)
	{
		float heightMin = -0.2f;
		float heightMax = 0.2f;
		float widthMin = 0.1f;
		float widthMax = 0.5f;
		float randX = 0.f;
		float randY = 0.f;
		if (israndom) {
			randX = 2.f * (((float)rand() / (float)(RAND_MAX)) - 0.5f);
			randY = 2.f * (((float)rand() / (float)(RAND_MAX)) - 0.5f);
		}
		else {
			randX = x;
			randY = y;
		}

		float randvelx = GetRandom(-1.f, 1.f);
		float randvely = GetRandom(-1.f, 1.f);
		float randvelz = 0.f;

		float randgx = 2.f * (((float)rand() / (float)(RAND_MAX)) - 0.5f);
		float randgy = 2.f * (((float)rand() / (float)(RAND_MAX)) - 0.5f);
		float randgz = 0.f;

		float randemit = GetRandom(0.f, 5.f);
		float randlife = GetRandom(1.f, 2.f);
		float randHeight = GetRandom(heightMin, heightMax);
		float randWidth = GetRandom(widthMin, widthMax);
		float randValue = GetRandom(0.f, 1.f);

		float randR = GetRandom(0.f, 1.f);
		float randG = GetRandom(0.f, 1.f);
		float randB = GetRandom(0.f, 1.f);
		float randA = 1.f;

		genVertex[idx++] = randX + rectSize;
		genVertex[idx++] = randY + rectSize;
		genVertex[idx++] = 0.f;
		genVertex[idx++] = randvelx;
		genVertex[idx++] = randvely;
		genVertex[idx++] = randvelz;
		genVertex[idx++] = randemit;
		genVertex[idx++] = randlife;
		genVertex[idx++] = randHeight;
		genVertex[idx++] = randWidth;
		genVertex[idx++] = randValue;
		genVertex[idx++] = randR;
		genVertex[idx++] = randG;
		genVertex[idx++] = randB;
		genVertex[idx++] = randA;
		genVertex[idx++] = randX - rectSize;
		genVertex[idx++] = randY + rectSize;
		genVertex[idx++] = 0.f;
		genVertex[idx++] = randvelx;
		genVertex[idx++] = randvely;
		genVertex[idx++] = randvelz;
		genVertex[idx++] = randemit;
		genVertex[idx++] = randlife;
		genVertex[idx++] = randHeight;
		genVertex[idx++] = randWidth;
		genVertex[idx++] = randValue;
		genVertex[idx++] = randR;
		genVertex[idx++] = randG;
		genVertex[idx++] = randB;
		genVertex[idx++] = randA;
		genVertex[idx++] = randX - rectSize;
		genVertex[idx++] = randY - rectSize;
		genVertex[idx++] = 0.f;
		genVertex[idx++] = randvelx;
		genVertex[idx++] = randvely;
		genVertex[idx++] = randvelz;
		genVertex[idx++] = randemit;
		genVertex[idx++] = randlife;
		genVertex[idx++] = randHeight;
		genVertex[idx++] = randWidth;
		genVertex[idx++] = randValue;
		genVertex[idx++] = randR;
		genVertex[idx++] = randG;
		genVertex[idx++] = randB;
		genVertex[idx++] = randA;

		genVertex[idx++] = randX + rectSize;
		genVertex[idx++] = randY - rectSize;
		genVertex[idx++] = 0.f;
		genVertex[idx++] = randvelx;
		genVertex[idx++] = randvely;
		genVertex[idx++] = randvelz;
		genVertex[idx++] = randemit;
		genVertex[idx++] = randlife;
		genVertex[idx++] = randHeight;
		genVertex[idx++] = randWidth;
		genVertex[idx++] = randValue;
		genVertex[idx++] = randR;
		genVertex[idx++] = randG;
		genVertex[idx++] = randB;
		genVertex[idx++] = randA;
		genVertex[idx++] = randX + rectSize;
		genVertex[idx++] = randY + rectSize;
		genVertex[idx++] = 0.f;
		genVertex[idx++] = randvelx;
		genVertex[idx++] = randvely;
		genVertex[idx++] = randvelz;
		genVertex[idx++] = randemit;
		genVertex[idx++] = randlife;
		genVertex[idx++] = randHeight;
		genVertex[idx++] = randWidth;
		genVertex[idx++] = randValue;
		genVertex[idx++] = randR;
		genVertex[idx++] = randG;
		genVertex[idx++] = randB;
		genVertex[idx++] = randA;
		genVertex[idx++] = randX - rectSize;
		genVertex[idx++] = randY - rectSize;
		genVertex[idx++] = 0.f;
		genVertex[idx++] = randvelx;
		genVertex[idx++] = randvely;
		genVertex[idx++] = randvelz;
		genVertex[idx++] = randemit;
		genVertex[idx++] = randlife;
		genVertex[idx++] = randHeight;
		genVertex[idx++] = randWidth;
		genVertex[idx++] = randValue;
		genVertex[idx++] = randR;
		genVertex[idx++] = randG;
		genVertex[idx++] = randB;
		genVertex[idx++] = randA;
	}
	glGenBuffers(1, pvbo);
	glBindBuffer(GL_ARRAY_BUFFER, *pvbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexArraySize, genVertex, GL_STATIC_DRAW);

	delete[] genVertex;
}

void Renderer::Lecture4()
{
	glUseProgram(m_ParticleShader);

	GLuint uTime = glGetUniformLocation(m_ParticleShader, "u_Time");

	m_uTime += 0.01f * m_uTimeDir;
	/*if (m_uTime >= 1.f)
		m_uTimeDir *= -1.f;
	else if(m_uTime < -1.f)
		m_uTimeDir *= -1.f;*/
	
	glUniform1f(uTime, m_uTime);

	GLuint aPot = glGetAttribLocation(m_ParticleShader, "a_Position");					// !!!쓰는 방법 외우기
	
	glEnableVertexAttribArray(aPot);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOGen);
	glVertexAttribPointer(aPot, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);

	GLuint aVel = glGetAttribLocation(m_ParticleShader, "a_Vel");

	glEnableVertexAttribArray(aVel);

	glVertexAttribPointer(aVel, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (GLvoid*)(sizeof(float) * 3));

	GLuint aEmit = glGetAttribLocation(m_ParticleShader, "a_EmitTime");

	glDisableVertexAttribArray(aPot);
	glDisableVertexAttribArray(aVel);
}

void Renderer::Lecture5()
{
	glUseProgram(m_ParticleShader);

	GLuint uTime = glGetUniformLocation(m_ParticleShader, "u_Time");
	GLuint uRepeat = glGetUniformLocation(m_ParticleShader, "u_Repeat");

	m_uTime += 0.01f;
	/*if (m_uTime >= 1.f)
		m_uTimeDir *= -1.f;
	else if(m_uTime < -1.f)
		m_uTimeDir *= -1.f;*/

	glUniform1f(uTime, m_uTime);
	//glUniformb(uRepeat, m_uTime);

	GLuint aPot = glGetAttribLocation(m_ParticleShader, "a_Position");					// !!!쓰는 방법 외우기
	GLuint aVel = glGetAttribLocation(m_ParticleShader, "a_Vel");
	GLuint aStartLife = glGetAttribLocation(m_ParticleShader, "a_StartLife");

	glEnableVertexAttribArray(aPot);
	glEnableVertexAttribArray(aVel);
	glEnableVertexAttribArray(aStartLife);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOGen);

	glVertexAttribPointer(aPot, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0);
	glVertexAttribPointer(aVel, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (GLvoid*)(sizeof(float) * 3));
	glVertexAttribPointer(aStartLife, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (GLvoid*)(sizeof(float) * 6));

	glDrawArrays(GL_TRIANGLES, 0, m_nGen * 3 * 6);

	glDisableVertexAttribArray(aPot);
	glDisableVertexAttribArray(aVel);
	glDisableVertexAttribArray(aStartLife);
}

void Renderer::Lecture6()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(m_PosShader);

	GLuint uTime = glGetUniformLocation(m_PosShader, "u_Time");
	GLuint uUTime = glGetUniformLocation(m_PosShader, "u_UTime");

	m_uTime += 0.001f * m_uTimeDir;
	//m_uUTime += 0.0001f;
	/*if (m_uTime > 5.f)
		m_uTimeDir *= -1.f;
	else if (m_uTime < 0.f)
		m_uTimeDir *= -1.f;*/

	glUniform1f(uTime, m_uTime);
	glUniform1f(uUTime, m_uUTime);

	GLuint aPot = glGetAttribLocation(m_PosShader, "a_Position");					// !!!쓰는 방법 외우기
	GLuint aTemp = glGetAttribLocation(m_PosShader, "a_Temp");
	GLuint aVel = glGetAttribLocation(m_PosShader, "a_Vel");
	GLuint aValue = glGetAttribLocation(m_PosShader, "a_Value");
	GLuint aColor = glGetAttribLocation(m_PosShader, "a_Color");

	glEnableVertexAttribArray(aPot);
	glEnableVertexAttribArray(aTemp);
	glEnableVertexAttribArray(aVel);
	glEnableVertexAttribArray(aValue);
	glEnableVertexAttribArray(aColor);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOpos);

	glVertexAttribPointer(aPot, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 15, 0);	
	glVertexAttribPointer(aVel, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid *)(sizeof(float) * 3));
	glVertexAttribPointer(aTemp, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid *)(sizeof(float) * 6));
	glVertexAttribPointer(aValue, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid *)(sizeof(float) * 10));
	glVertexAttribPointer(aColor, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid *)(sizeof(float) * 11));

	glDrawArrays(GL_TRIANGLES, 0, m_nPos * 3 * 6);

	glDisableVertexAttribArray(aPot);
	glDisableVertexAttribArray(aVel);
	glDisableVertexAttribArray(aTemp);
	glDisableVertexAttribArray(aValue);
	glDisableVertexAttribArray(aColor);
}

void Renderer::CreateGridMesh()
{
	float basePosX = -0.5f;
	float basePosY = -0.5f;
	float targetPosX = 0.5f;
	float targetPosY = 0.5f;

	int pointCountX = 32;
	int pointCountY = 32;

	float width = targetPosX - basePosX;
	float height = targetPosY - basePosY;

	float* point = new float[pointCountX*pointCountY * 2];
	float* vertices = new float[(pointCountX - 1)*(pointCountY - 1) * 2 * 3 * 3];
	m_nGridMesh = (pointCountX - 1)*(pointCountY - 1) * 2 * 3;

	//Prepare points
	for (int x = 0; x < pointCountX; x++)
	{
		for (int y = 0; y < pointCountY; y++)
		{
			point[(y*pointCountX + x) * 2 + 0] = basePosX + width * (x / (float)(pointCountX - 1));
			point[(y*pointCountX + x) * 2 + 1] = basePosY + height * (y / (float)(pointCountY - 1));
		}
	}

	//Make triangles
	int vertIndex = 0;
	for (int x = 0; x < pointCountX - 1; x++)
	{
		for (int y = 0; y < pointCountY - 1; y++)
		{
			//Triangle part 1
			vertices[vertIndex] = point[(y*pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y*pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;

			//Triangle part 2
			vertices[vertIndex] = point[(y*pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y*pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[(y*pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y*pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
		}
	}

	glGenBuffers(1, &m_VBOGridMesh);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOGridMesh);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*(pointCountX - 1)*(pointCountY - 1) * 2 * 3 * 3, vertices, GL_STATIC_DRAW);

	delete[] point;
	delete[] vertices;
}

void Renderer::DrawGridMesh()
{
	glUseProgram(m_SolidRectShader);

	//int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOGridMesh);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, m_nGridMesh);

	glDisableVertexAttribArray(0);
}

void Renderer::Lecture7()
{
	GLuint shader = m_SandBoxShader;

	glUseProgram(shader);

	GLuint aPos = glGetAttribLocation(shader, "a_Position");
	GLuint aUV = glGetAttribLocation(shader, "a_UV");

	glEnableVertexAttribArray(aPos);
	glEnableVertexAttribArray(aUV);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);

	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	glVertexAttribPointer(aUV, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (GLvoid *)(sizeof(float) * 4));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aUV);
}