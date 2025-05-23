

#include "guideline.h"
#include "core/engine.h"

using namespace m1;

Guideline::Guideline(int resX, int resY)
{
	setVertices(generateVertices(resX, resY));
	setIndices(generateIndices());
	setAngle(90.0f);
}

Guideline::~Guideline()
{
}

//  Getters
vector<VertexFormat>& Guideline::getVertices() {
	return this->vertices;
}

vector<unsigned int>& Guideline::getIndices() {
	return this->indices;
}

glm::vec3 Guideline::getPosition() {
	return this->position;
}

glm::vec3 Guideline::getDirection() {
	return this->direction;
}

float Guideline::getAngle() {
	return this->angle;
}

int Guideline::getCollisions() {
	return this->collisions;
}

//  Setters
void Guideline::setVertices(vector<VertexFormat> vertices) {
	this->vertices = vertices;
}

void Guideline::setIndices(vector<unsigned int> indices) {
	this->indices = indices;
}

void Guideline::setPosition(glm::vec3 position) {
	this->position = position;
}

void Guideline::setDirection(glm::vec3 direction) {
	this->direction = direction;
}

void Guideline::setAngle(float angle) {
	this->angle = angle;
}

//  Generating Guideline vertices
vector<VertexFormat> Guideline::generateVertices(int resX, int resY) {
	//  Guideline is placed in the center of the Guideline circle
	vector<VertexFormat> vertices
	{
		VertexFormat(glm::vec3(0, resY / ENDY, 0), NORM, GUIDELINECOLOUR),
		VertexFormat(glm::vec3(0, 1.15 * resY / ENDY, 0), NORM, GUIDELINECOLOUR),
		VertexFormat(glm::vec3(0.2f * resX / ENDX, 1.15f * resY / ENDY, 0), NORM, GUIDELINECOLOUR),
		VertexFormat(glm::vec3(0.2f * resX / ENDX, resY / ENDY, 0), NORM, GUIDELINECOLOUR)
	};

	return vertices;
}

//  Generating Guideline indices
vector<unsigned int> Guideline::generateIndices() {
	vector<unsigned int> indices =
	{
		0, 1, 2,
		0, 2, 3
	};

	return indices;
}

bool Guideline::inScreen(int resX, int resY) {
	if (this->getPosition().x >= 0 && this->getPosition().x <= resX &&
		this->getPosition().y >= 0 && this->getPosition().y <= resY) {
		return true;
	}

	return false;
}

glm::vec3 Guideline::initialStart(int resY) {
	//  1.075f is resY / ENDY + (resY / ENDY + 1.15f * resY / ENDY) / 2
	//	This way we can put xOy origin in the middle of the barrel
	return glm::vec3(0, 1.075f * resY / ENDY, 0);
}