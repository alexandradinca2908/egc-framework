 

#include "turret.h"
#include "core/engine.h"

using namespace m1;

Turret::Turret(int resX, int resY)
{
	setVertices(generateVertices(resX, resY));
	setIndices(generateIndices());
	setAngle(90.0f);
}

Turret::~Turret()
{
}

//  Getters
vector<VertexFormat>& Turret::getVertices() {
	return this->vertices;
}

vector<unsigned int>& Turret::getIndices() {
	return this->indices;
}

glm::vec3 Turret::getPosition() {
	return this->position;
}

float Turret::getAngle() {
	return this->angle;
}

//  Setters
void Turret::setVertices(vector<VertexFormat> vertices) {
	this->vertices = vertices;
}

void Turret::setIndices(vector<unsigned int> indices) {
	this->indices = indices;
}

void Turret::setPosition(glm::vec3 position) {
	this->position = position;
}

void Turret::setAngle(float angle) {
	this->angle = angle;
}

//  Generating turret vertices
vector<VertexFormat> Turret::generateVertices(int resX, int resY) {
	//  Turret is placed in the center of the tank circle
	vector<VertexFormat> vertices
	{
		VertexFormat(glm::vec3(0, resY / ENDY, 0), NORM, TURRETCOLOUR),
		VertexFormat(glm::vec3(0, 1.15f * resY / ENDY, 0), NORM, TURRETCOLOUR),
		VertexFormat(glm::vec3(2.5f * resX / ENDX, 1.15f *  resY / ENDY, 0), NORM, TURRETCOLOUR),
		VertexFormat(glm::vec3(2.5f * resX / ENDX, resY / ENDY, 0), NORM, TURRETCOLOUR)
	};

	return vertices;
}

//  Generating turret indices
vector<unsigned int> Turret::generateIndices() {
	vector<unsigned int> indices =
	{
		0, 1, 2,
		0, 2, 3
	};

	return indices;
}

glm::vec3 Turret::initialStart(int resY) {
	//  1.075f is resY / ENDY + (resY / ENDY + 1.15f * resY / ENDY) / 2
	//	This way we can put xOy origin in the middle of the barrel
	return glm::vec3(0, 1.075f * resY / ENDY, 0);
}