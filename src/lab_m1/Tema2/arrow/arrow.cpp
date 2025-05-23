 

#include "arrow.h"
#include "core/engine.h"

using namespace m1;

Arrow::Arrow(int resX, int resY, glm::vec3 position)
{
	setVertices(generateVertices(resX, resY));
	setIndices(generateIndices());
	setPosition(position);
	setAngleOx(0);
	setAngleOy(0);
	setAngleOz(0);
}

Arrow::~Arrow()
{
}

//  Getters
vector<VertexFormat>& Arrow::getVertices() {
	return this->vertices;
}

vector<unsigned int>& Arrow::getIndices() {
	return this->indices;
}

glm::vec3 Arrow::getPosition() {
	return this->position;
}

float Arrow::getAngleOx() {
	return this->angleOx;
}

float Arrow::getAngleOy() {
	return this->angleOy;
}

float Arrow::getAngleOz() {
	return this->angleOz;
}

//  Setters
void Arrow::setVertices(vector<VertexFormat> vertices) {
	this->vertices = vertices;
}

void Arrow::setIndices(vector<unsigned int> indices) {
	this->indices = indices;
}

void Arrow::setPosition(glm::vec3 position) {
	this->position = position;
}

void Arrow::setAngleOx(float angle) {
	this->angleOx = angle;
}

void Arrow::setAngleOy(float angle) {
	this->angleOy = angle;
}

void Arrow::setAngleOz(float angle) {
	this->angleOz = angle;
}

//  Generating Arrow vertices
vector<VertexFormat> Arrow::generateVertices(int resX, int resY) {
	vector<VertexFormat> vertices
	{
		//  Arrow body
		VertexFormat(glm::vec3(-0.07f, 0, 0), ARROWCOLOUR, NORM),
		VertexFormat(glm::vec3(-0.07f, 0, -0.4f), ARROWCOLOUR, NORM),
		VertexFormat(glm::vec3(0.07f, 0, -0.4f), ARROWCOLOUR, NORM),
		VertexFormat(glm::vec3(0.07f, 0, 0), ARROWCOLOUR, NORM),

		//  Arrow tip
		VertexFormat(glm::vec3(-0.13f, 0, -0.4f), ARROWCOLOUR, NORM),
		VertexFormat(glm::vec3(0, 0, -0.55f), ARROWCOLOUR, NORM),
		VertexFormat(glm::vec3(0.13f, 0, -0.4f), ARROWCOLOUR, NORM)
	};

	return vertices;
}

//  Generating Arrow indices
vector<unsigned int> Arrow::generateIndices() {
	vector<unsigned int> indices
	{
		//  Body
		0, 2, 1,
		0, 3, 2,
		4, 6, 5
	};

	return indices;
}