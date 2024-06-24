#pragma once
#include <vector>
#include <glm/glm.hpp>


class ImportedModel
{
	int numVertices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normalVecs;

public:
	ImportedModel(const char* filePath);
	int getNumVertices();
	std::vector<glm::vec3> getVertices();
	std::vector<glm::vec2> getTexCoords();
	std::vector<glm::vec3> getNormals();
};

class ModelImporter
{
	// values as read from OBJ files
	std::vector<float> vertVals;
	std::vector<float> stVals;
	std::vector<float> normVals;

	// values stored later use as vertex attributes
	std::vector<float> triangleVerts;
	std::vector<float> textureCoords;
	std::vector<float> normals;

public:
	ModelImporter();
	void parseOBJ(const char* filePath);
	int getNumVertices();
	std::vector<float> getVertices();
	std::vector<float> getTextureCoords();
	std::vector<float> getNormals();
};