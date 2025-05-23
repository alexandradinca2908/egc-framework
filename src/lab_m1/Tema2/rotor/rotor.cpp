
#include "rotor.h"
#include "core/engine.h"

using namespace m1;

Rotor::Rotor(int resX, int resY, glm::vec3 position)
{
	setVertices(generateVertices(resX, resY));
	setIndices(generateIndices());
	setPosition(position);
	setAngleOy(45.0f);
}

Rotor::~Rotor()
{
}

//  Getters
vector<VertexFormat>& Rotor::getVertices() {
	return this->vertices;
}

vector<unsigned int>& Rotor::getIndices() {
	return this->indices;
}

glm::vec3 Rotor::getPosition() {
	return this->position;
}

float Rotor::getAngleOy() {
	return this->angleOy;
}

//  Setters
void Rotor::setVertices(vector<VertexFormat> vertices) {
	this->vertices = vertices;
}

void Rotor::setIndices(vector<unsigned int> indices) {
	this->indices = indices;
}

void Rotor::setPosition(glm::vec3 position) {
	this->position = position;
}

void Rotor::setAngleOy(float angle) {
	this->angleOy = angle;
}

//  Generating Rotor vertices
vector<VertexFormat> Rotor::generateVertices(int resX, int resY) {
	vector<VertexFormat> vertices
	{
		//  Rotor body
		//  Leg 1
		VertexFormat(glm::vec3(-0.01f, -0.02f, -0.2f), ROTORCOLOUR, NORM),
		VertexFormat(glm::vec3(-0.01f, 0.02f, -0.2f), ROTORCOLOUR, NORM),
		VertexFormat(glm::vec3(0.01f, -0.02f, -0.2f), ROTORCOLOUR, NORM),
		VertexFormat(glm::vec3(0.01f, 0.02f, -0.2f), ROTORCOLOUR, NORM),
		VertexFormat(glm::vec3(-0.01f, -0.02f, 0.2f), ROTORCOLOUR, NORM),
		VertexFormat(glm::vec3(-0.01f, 0.02f, 0.2f), ROTORCOLOUR, NORM),
		VertexFormat(glm::vec3(0.01f, -0.02f, 0.2f), ROTORCOLOUR, NORM),
		VertexFormat(glm::vec3(0.01f, 0.02f, 0.2f), ROTORCOLOUR, NORM)
	};

	return vertices;
}

//  Generating Rotor indices
vector<unsigned int> Rotor::generateIndices() {
	vector<unsigned int> indices
	{
		//  Leg 1
		2, 1, 3,
		2, 0, 1,
		5, 3, 1,
		5, 7, 3,
		6, 3, 7,
		6, 2, 3,
		6, 0, 2,
		6, 4, 0,
		0, 5, 1,
		0, 4, 5,
		4, 7, 5,
		4, 6, 7
	};

	return indices;
}