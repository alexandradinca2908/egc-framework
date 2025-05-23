
#include "healthbar.h"
#include "core/engine.h"

using namespace m1;

Healthbar::Healthbar(int resX, int resY, int index, glm::vec3 position)
{
	setVertices(generateVerticesFull(resX, resY, index), generateVerticesEmpty(resX, resY, index));
	setIndices(generateIndicesFull(), generateIndicesEmpty());
	setPosition(position);
}

Healthbar::~Healthbar()
{
}

//  Getters
vector<VertexFormat>& Healthbar::getVerticesFull() {
	return this->verticesFull;
}

vector<VertexFormat>& Healthbar::getVerticesEmpty() {
	return this->verticesEmpty;
}

vector<unsigned int>& Healthbar::getIndicesFull() {
	return this->indicesFull;
}

vector<unsigned int>& Healthbar::getIndicesEmpty() {
	return this->indicesEmpty;
}

unsigned int& Healthbar::getVBOFull() {
	return this->VBO_full;
}

glm::vec3 Healthbar::getPosition() {
	return this->position;
}

//  Setters
void Healthbar::setVertices(vector<VertexFormat> verticesFull, vector<VertexFormat> verticesEmpty) {
	this->verticesFull = verticesFull;
	this->verticesEmpty = verticesEmpty;
}

void Healthbar::setIndices(vector<unsigned int> indicesFull, vector<unsigned int> indicesEmpty) {
	this->indicesFull = indicesFull;
	this->indicesEmpty = indicesEmpty;
}

void Healthbar::setPosition(glm::vec3 position) {
	this->position = position;
}

//  Generating healthbar marker vertices
vector<VertexFormat> Healthbar::generateVerticesFull(int resX, int resY, int index) {
	glm::vec3 healthbarColourMain;

	if (index == 1) {
		healthbarColourMain = HEALTHBARCOLOURMAIN1;
	}
	else {
		healthbarColourMain = HEALTHBARCOLOURMAIN2;
	}

	vector<VertexFormat> vertices
	{
		VertexFormat(glm::vec3(-2.0f * resX / ENDX, 3.0f * resY / ENDY, 0), NORM, healthbarColourMain),
		VertexFormat(glm::vec3(-2.0f * resX / ENDX, 3.5f * resY / ENDY, 0), NORM, healthbarColourMain),
		VertexFormat(glm::vec3(2.0f * resX / ENDX, 3.0f * resY / ENDY, 0), NORM, healthbarColourMain),
		VertexFormat(glm::vec3(2.0f * resX / ENDX, 3.5f * resY / ENDY, 0), NORM, healthbarColourMain)
	};

	return vertices;
}

//  Generating healthbar marker indices
vector<unsigned int> Healthbar::generateIndicesFull() {
	vector<unsigned int> indices =
	{
		0, 1, 2,
		1, 2, 3
	};

	return indices;
}

//  Generating empty healthbar vertices
vector<VertexFormat> Healthbar::generateVerticesEmpty(int resX, int resY, int index) {
	glm::vec3 healthbarColourMain;

	if (index == 1) {
		healthbarColourMain = HEALTHBARCOLOURMAIN1;
	}
	else {
		healthbarColourMain = HEALTHBARCOLOURMAIN2;
	}

	vector<VertexFormat> vertices
	{
		//  Left
		VertexFormat(glm::vec3(-2.0f * resX / ENDX, 3.0f * resY / ENDY, 0), NORM, healthbarColourMain),
		VertexFormat(glm::vec3(-1.8f * resX / ENDX, 3.0f * resY / ENDY, 0), NORM, healthbarColourMain),
		VertexFormat(glm::vec3(-2.0f * resX / ENDX, 3.5f * resY / ENDY, 0), NORM, healthbarColourMain),
		VertexFormat(glm::vec3(-1.8f * resX / ENDX, 3.5f * resY / ENDY, 0), NORM, healthbarColourMain),

		//  Left-up
		VertexFormat(glm::vec3(-2.0f * resX / ENDX, 3.35f * resY / ENDY, 0), NORM, healthbarColourMain),

		//  Right-up
		VertexFormat(glm::vec3(2.0f * resX / ENDX, 3.35f * resY / ENDY, 0), NORM, healthbarColourMain),

		//  Right
		VertexFormat(glm::vec3(1.8f * resX / ENDX, 3.5f * resY / ENDY, 0), NORM, healthbarColourMain),
		VertexFormat(glm::vec3(2.0f * resX / ENDX, 3.5f * resY / ENDY, 0), NORM, healthbarColourMain),
		VertexFormat(glm::vec3(1.8f * resX / ENDX, 3.0f * resY / ENDY, 0), NORM, healthbarColourMain),
		VertexFormat(glm::vec3(2.0f * resX / ENDX, 3.0f * resY / ENDY, 0), NORM, healthbarColourMain),

		//  Right-down
		VertexFormat(glm::vec3(2.0f * resX / ENDX, 3.15f * resY / ENDY, 0), NORM, healthbarColourMain),

		//  Left-down
		VertexFormat(glm::vec3(-2.0f * resX / ENDX, 3.15f * resY / ENDY, 0), NORM, healthbarColourMain),
	};

	return vertices;
}

//  Generating empty healthbar indices
vector<unsigned int> Healthbar::generateIndicesEmpty() {
	vector<unsigned int> indices =
	{
		//  Left
		0, 1, 2,
		1, 2, 3,
		//  Up
		2, 4, 7,
		4, 5, 7,
		//  Right
		6, 7, 8,
		7, 8, 9,
		//  Down
		9, 10, 0,
		10, 11, 0
	};

	return indices;
}

float Healthbar::barLength(int resX) {
	return 4.0f * resX / ENDX;
}