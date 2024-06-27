#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <cmath>
#include <stack>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Utils.h"
#include "Sphere.h"
#include "ImportedModel.h"

using namespace std;

#define numVAOs 2
#define numVBOs 5

GLuint renderingProgram, renderingProgramCubeMap, renderingProgram2;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

const char* vertexShader = "vertShader.glsl";
const char* fragmentShader = "fragShader.glsl";
const char* vertex2Shader = "vertShader2.glsl";
const char* fragment2Shader = "fragShader2.glsl";
const int SCREEN_WIDTH = 2560;
const int SCREEN_HEIGHT = 1440;


float deltaTime = 0.0f;
float lastTime = 0.0f;
float timeFactor;

/*------------------Camera Variable-------------------*/
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 6.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float m_yaw = -90.0f;
float m_pitch = 0.0f;
float fov = 45.0f;


/*----Mouse variable------*/
bool firstMouse = true;
float lastX = 800.0f / 2.0;
float lastY = 600.0f / 2.0;


/*------------Objects and Textures--------------*/
Sphere mySphere(48);
glm::vec3 modelPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cubePos = glm::vec3(0.0f, 0.0f, 0.0f);
ImportedModel myModel("obj/Knife.obj");
Utils utils;
GLuint cubeTexture;
GLuint skyboxTexture;

/*-----------------Matrices and location variables----------------------------------------*/
GLuint mvLoc, projLoc, tfLoc, nLoc, objectTypeLoc;
GLuint globalAmbLoc, ambLoc, diffLoc, specLoc, posLoc, mAmbLoc, mDiffLoc, mSpecLoc, mShiLoc;
int width, height;
float aspect;
glm::mat4 pMat, vMat, mMat, mvMat, invTrMat, tMat;
glm::mat3 rMat;

/*-----------------Light variable----------------------*/
glm::vec3 initialLightLoc = glm::vec3(5.0f, 2.0f, 2.0f);
glm::vec3 currentLightPos, lightPosV;
float lightPos[3];
// white light properties
float globalAmbient[4] = { 0.7f, 0.7f, 0.7f, 1.0f };
float lightAmbient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
float lightDiffue[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float lightSpecular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float* matAmb = utils.jadeAmbient();
float* matDif = utils.jadeDiffuse();
float* matSpe = utils.jadeSpecular();
float matShi = utils.jadeShininess();

float* matAmb2 = utils.bronzeAmbient();
float* matDif2 = utils.bronzeDiffuse();
float* matSpe2 = utils.bronzeSpecular();
float matShi2 = utils.bronzeShininess();

void setupVertices(void) {

	float cubePositions[108] = {
		-1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f
	};
	float cubeTexCoords[72] = {
		1.00f, 0.66f, 1.00f, 0.33f, 0.75f, 0.33f, 
		0.75f, 0.33f, 0.75f, 0.66f, 1.00f, 0.66f, 
		0.75f, 0.33f, 0.50f, 0.33f, 0.75f, 0.66f, 
		0.50f, 0.33f, 0.50f, 0.66f, 0.75f, 0.66f, 
		0.50f, 0.33f, 0.25f, 0.33f, 0.50f, 0.66f, 
		0.25f, 0.33f, 0.25f, 0.66f, 0.50f, 0.66f, 
		0.25f, 0.33f, 0.00f, 0.33f, 0.25f, 0.66f, 
		0.00f, 0.33f, 0.00f, 0.66f, 0.25f, 0.66f, 
		0.25f, 0.33f, 0.50f, 0.33f, 0.50f, 0.00f, 
		0.50f, 0.00f, 0.25f, 0.00f, 0.25f, 0.33f, 
		0.25f, 1.00f, 0.50f, 1.00f, 0.50f, 0.66f, 
		0.50f, 0.66f, 0.25f, 0.66f, 0.25f, 1.00f
	};

	std::vector<int> ind = mySphere.getIndices();
	std::vector<glm::vec3> vert = mySphere.getVertices();
	std::vector<glm::vec2> tex = mySphere.getTexCoords();
	std::vector<glm::vec3> norm = mySphere.getNormals();

	// for sphere
	int numIndices = mySphere.getNumIndices();

	//// for model
	//int numObjVertices = myModel.getNumVertices();

	std::vector<float> pvalues;
	std::vector<float> tvalues;
	std::vector<float> nvalues;

	// Use for sphere object
	for (int i = 0; i < numIndices; i++)
	{
		pvalues.push_back((vert[ind[i]]).x);
		pvalues.push_back((vert[ind[i]]).y);
		pvalues.push_back((vert[ind[i]]).z);

		tvalues.push_back((tex[ind[i]]).s);
		tvalues.push_back((tex[ind[i]]).t);

		nvalues.push_back((norm[ind[i]]).x);
		nvalues.push_back((norm[ind[i]]).y);
		nvalues.push_back((norm[ind[i]]).z);
	}

	//// Use for model object
	//for (int i = 0; i < numObjVertices; i++)
	//{
	//	pvalues.push_back((vert[i]).x);
	//	pvalues.push_back((vert[i]).y);
	//	pvalues.push_back((vert[i]).z);

	//	tvalues.push_back((tex[i]).s);
	//	tvalues.push_back((tex[i]).t);

	//	nvalues.push_back((norm[i]).x);
	//	nvalues.push_back((norm[i]).y);
	//	nvalues.push_back((norm[i]).z);
	//}

	glGenVertexArrays(numVAOs, vao);
	glGenBuffers(numVBOs, vbo);

	// Sphere
	glBindVertexArray(vao[0]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * sizeof(float), pvalues.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * sizeof(float), tvalues.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * sizeof(float), nvalues.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(2);


	// Cube
	glBindVertexArray(vao[1]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubePositions), cubePositions, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeTexCoords), cubeTexCoords, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	// Check for errors
	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		std::cerr << "OpenGL error: " << err << std::endl;
	}
}

void init(GLFWwindow* window) {
	renderingProgram = utils.createShaderProgram(vertexShader, fragmentShader);
	renderingProgram2 = utils.createShaderProgram(vertex2Shader, fragment2Shader);
	renderingProgramCubeMap = utils.createShaderProgram("vertCShader.glsl", "fragCShader.glsl");
	setupVertices();

	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(glm::radians(fov), aspect, 0.1f, 1000.0f);

	skyboxTexture = utils.loadTexture("img/space.jpg");
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

void lightLocations(GLuint renderProgram, glm::vec3 lightPosV, float* Amb, float* Dif, float* Spe, float Shi)
{
	lightPos[0] = lightPosV.x;
	lightPos[1] = lightPosV.y;
	lightPos[2] = lightPosV.z;

	// get locations of light and material fields
	globalAmbLoc = glGetUniformLocation(renderProgram, "globalAmbient");
	ambLoc = glGetUniformLocation(renderProgram, "light.ambient");
	diffLoc = glGetUniformLocation(renderProgram, "light.diffuse");
	specLoc = glGetUniformLocation(renderProgram, "light.specular");
	posLoc = glGetUniformLocation(renderProgram, "light.position");
	mAmbLoc = glGetUniformLocation(renderProgram, "material.ambient");
	mDiffLoc = glGetUniformLocation(renderProgram, "material.diffuse");
	mSpecLoc = glGetUniformLocation(renderProgram, "material.specular");
	mShiLoc = glGetUniformLocation(renderProgram, "material.shininess");

	// set uniform light and material values in shader
	glProgramUniform4fv(renderProgram, globalAmbLoc, 1, globalAmbient);
	glProgramUniform4fv(renderProgram, ambLoc, 1, lightAmbient);
	glProgramUniform4fv(renderProgram, diffLoc, 1, lightDiffue);
	glProgramUniform4fv(renderProgram, specLoc, 1, lightSpecular);
	glProgramUniform4fv(renderProgram, posLoc, 1, lightPos);
	glProgramUniform4fv(renderProgram, mAmbLoc, 1, Amb);
	glProgramUniform4fv(renderProgram, mDiffLoc, 1, Dif);
	glProgramUniform4fv(renderProgram, mSpecLoc, 1, Spe);
	glProgramUniform1f(renderProgram, mShiLoc, Shi);
}

void installLights(glm::mat4 vMatrix) {

	// convert lights position in space
	lightPosV = glm::vec3(vMatrix * glm::vec4(currentLightPos, 1.0));
	lightLocations(renderingProgram, lightPosV, matAmb, matDif, matSpe, matShi);
	lightLocations(renderingProgram2, lightPosV, matAmb2, matDif2, matSpe2, matShi2);
}

float toRadians(float degrees)
{
	return (degrees * 2.0f * 3.14159f) / 360.0f;
}

void processInput(GLFWwindow* window) {

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);

	float cameraSpeed = 2.5 * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) cameraSpeed *= 3;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void shaderLocations(GLuint renderProgram)
{
	mvLoc = glGetUniformLocation(renderProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderProgram, "proj_matrix");
	tfLoc = glGetUniformLocation(renderProgram, "tf");
	nLoc = glGetUniformLocation(renderProgram, "norm_matrix");
	objectTypeLoc = glGetUniformLocation(renderProgram, "objectType");
}

void display(GLFWwindow* window, double currentTime) {

	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);

	processInput(window);

	shaderLocations(renderingProgram);

	deltaTime = currentTime - lastTime;
	lastTime = currentTime;

	vMat = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	//--------------------SkyBox----------------------------------------
	glUseProgram(renderingProgramCubeMap);

	mMat = glm::translate(glm::mat4(1.0f), glm::vec3(cubePos.x, cubePos.y, cubePos.z));
	mMat = glm::scale(mMat, glm::vec3(100.f, 100.f, 100.f));
	mvMat = vMat * mMat;
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniform1i(objectTypeLoc, 1);

	glBindVertexArray(vao[1]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

	glDisable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glEnable(GL_DEPTH_TEST);

	//--------------------------------Sphere----------------------------------
	glUseProgram(renderingProgram);
	mMat = glm::translate(glm::mat4(1.0f), glm::vec3(modelPos.x, modelPos.y, modelPos.z));
	mMat *= glm::rotate(mMat, toRadians(35.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	mvMat = vMat * mMat;
	timeFactor = ((float)currentTime);

	currentLightPos = glm::vec3(initialLightLoc.x, initialLightLoc.y, initialLightLoc.z);
	installLights(vMat);

	invTrMat = glm::transpose(glm::inverse(mvMat));

	glUniform1f(tfLoc, (float)timeFactor);
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
	glUniform1i(objectTypeLoc, 0);

	glBindVertexArray(vao[0]);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glDrawArrays(GL_TRIANGLES, 0, mySphere.getNumIndices());

	// Sphere 2
	glUseProgram(renderingProgram2);

	shaderLocations(renderingProgram2);

	mMat = glm::translate(glm::mat4(1.0f), glm::vec3(modelPos.x, modelPos.y + 4, modelPos.z + 20));
	mMat *= glm::rotate(mMat, toRadians(35.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	mvMat = vMat * mMat;
	timeFactor = ((float)currentTime);

	currentLightPos = glm::vec3(initialLightLoc.x, initialLightLoc.y, initialLightLoc.z);
	installLights(vMat);

	invTrMat = glm::transpose(glm::inverse(mvMat));

	glUniform1f(tfLoc, (float)timeFactor);
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
	glUniform1i(objectTypeLoc, 2);

	glBindVertexArray(vao[0]);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glDrawArrays(GL_TRIANGLES, 0, mySphere.getNumIndices());

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	if (fov >= 1.0f && fov <= 45.0f) fov -= yoffset;
	if (fov <= 1.0f) fov = 1.0f;
	if (fov >= 45.0f) fov = 45.0f;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	m_yaw += xoffset;
	m_pitch += yoffset;

	if (m_pitch > 89.0f) m_pitch = 89.0f;
	if (m_pitch < -89.0f) m_pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	front.y = sin(glm::radians(m_pitch));
	front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	cameraFront = glm::normalize(front);
}

int main(void) {

	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OPENGL PRACTICE", NULL, NULL);
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	init(window);

	while (!glfwWindowShouldClose(window)) {

		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}