#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <glm/glm.hpp>
#include "ImportedModel.h"
using namespace std;


//-----------Imported Model Class---------------------
ImportedModel::ImportedModel(const char* filePath) {
	ModelImporter modelImporter = ModelImporter();
	modelImporter.parseOBJ(filePath);

	numVertices = modelImporter.getNumVertices();

	std::vector<float> verts = modelImporter.getVertices();
	std::vector<float> tcs = modelImporter.getTextureCoords();
	std::vector<float> normals = modelImporter.getNormals();

	for (int i = 0; i < numVertices; i++) {
		vertices.push_back(glm::vec3(verts[i * 3], verts[i * 3 + 1], verts[i * 3 + 2]));
		texCoords.push_back(glm::vec2(tcs[i * 2], tcs[i * 2 + 1]));
		normalVecs.push_back(glm::vec3(normals[i * 3], normals[i * 3 + 1], normals[i * 3 + 2]));
	}
}

int ImportedModel::getNumVertices() { return numVertices; }
std::vector<glm::vec3> ImportedModel::getVertices() { return vertices; }
std::vector<glm::vec2> ImportedModel::getTexCoords() { return texCoords; }
std::vector<glm::vec3> ImportedModel::getNormals() { return normalVecs; }

//----------Model Importer Class---------------------------
ModelImporter::ModelImporter() {}

void ModelImporter::parseOBJ(const char* filePath) {
    float x, y, z;
    string content;
    ifstream fileStream(filePath, ios::in);
    string line = "";

    while (getline(fileStream, line)) {
        if (line.substr(0, 2) == "v ") {
            stringstream ss(line.substr(2));
            ss >> x >> y >> z;
            vertVals.push_back(x);
            vertVals.push_back(y);
            vertVals.push_back(z);
        }
        else if (line.substr(0, 2) == "vt") {
            stringstream ss(line.substr(3));
            ss >> x >> y;
            stVals.push_back(x);
            stVals.push_back(y);
        }
        else if (line.substr(0, 2) == "vn") {
            stringstream ss(line.substr(3));
            ss >> x >> y >> z;
            normVals.push_back(x);
            normVals.push_back(y);
            normVals.push_back(z);
        }
        else if (line.substr(0, 2) == "f ") {
            stringstream ss(line.substr(2));
            string v, t, n;
            for (int i = 0; i < 3; i++) {
                getline(ss, v, '/');
                getline(ss, t, '/');
                getline(ss, n, ' ');

                int vertIndex = (stoi(v) - 1) * 3;
                int texCoordIndex = (stoi(t) - 1) * 2;
                int normIndex = (stoi(n) - 1) * 3;

                triangleVerts.push_back(vertVals[vertIndex]);
                triangleVerts.push_back(vertVals[vertIndex + 1]);
                triangleVerts.push_back(vertVals[vertIndex + 2]);

                textureCoords.push_back(stVals[texCoordIndex]);
                textureCoords.push_back(stVals[texCoordIndex + 1]);

                normals.push_back(normVals[normIndex]);
                normals.push_back(normVals[normIndex + 1]);
                normals.push_back(normVals[normIndex + 2]);
            }
        }
    }
}

int ModelImporter::getNumVertices() { return (triangleVerts.size() / 3); }
std::vector<float> ModelImporter::getVertices() { return triangleVerts; }
std::vector<float> ModelImporter::getTextureCoords() { return textureCoords; }
std::vector<float> ModelImporter::getNormals() { return normals; }