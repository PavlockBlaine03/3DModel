#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL2/SOIL2.h>
using namespace std;
using namespace glm;

class Utils
{
public:
	Utils() {};

	void printShaderLog(GLuint shader);
	void printProgramLog(int prog);
	bool checkOpenGLError();
	string readShaderSource(const char* filePath);
	GLuint createShaderProgram(const char *vp, const char *fp);
	GLuint createShaderProgram(const char *vp, const char *gp, const char *fp);
	GLuint createShaderProgram(const char *vp, const char *tCS, const char *tES, const char *fp);
	GLuint createShaderProgram(const char *vp, const char *tCS, const char *tES, const char *gp, const char *fp);
	GLuint loadTexture(const char* texImagePath);

	// Gold material- ADS: ambient, diffuse, specular, and shininess
	float* goldAmbient();
	float* goldDiffuse();
	float* goldSpecular();
	float goldShininess();

	// Silver material
	float* silverAmbient();
	float* silverDiffuse();
	float* silverSpecular();
	float silverShininess();

	// Bronze material
	float* bronzeAmbient();
	float* bronzeDiffuse();
	float* bronzeSpecular();
	float bronzeShininess();
};