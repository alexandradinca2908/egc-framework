#include "delivery.h"
#include "core/engine.h"

using namespace m1;

Delivery::Delivery(int resX, int resY)
{
	setVertices(generateVertices(resX, resY));
	setIndices(generateIndices());
	setPosition(position);
}

Delivery::~Delivery()
{
}

//  Getters
vector<VertexFormat>& Delivery::getVertices() {
	return this->vertices;
}

vector<unsigned int>& Delivery::getIndices() {
	return this->indices;
}

glm::vec3 Delivery::getPosition() {
	return this->position;
}

float Delivery::getAngleOx() {
	return this->angleOx;
}

float Delivery::getAngleOy() {
	return this->angleOy;
}

float Delivery::getAngleOz() {
	return this->angleOz;
}

float Delivery::getScale() {
	return this->scale;
}

//  Setters
void Delivery::setVertices(vector<VertexFormat> vertices) {
	this->vertices = vertices;
}

void Delivery::setIndices(vector<unsigned int> indices) {
	this->indices = indices;
}

void Delivery::setPosition(glm::vec3 position) {
	this->position = position;
}

void Delivery::setAngleOx(float angle) {
	this->angleOx = angle;
}

void Delivery::setAngleOy(float angle) {
	this->angleOy = angle;
}

void Delivery::setAngleOz(float angle) {
	this->angleOz = angle;
}

void Delivery::setScale(float scale) {
	this->scale = scale;
}

//  Generating Delivery vertices
vector<VertexFormat> Delivery::generateVertices(int resX, int resY) {
	vector<VertexFormat> vertices;

	//  Delivery inner circle
	float alpha = 0.0f;
	float y = 0.0f;

	for (int i = 0; i <= CIRCLE; i++) {
		float xi = RADIUS1 * cos(alpha);
		float zi = RADIUS1 * sin(alpha);
		vertices.push_back(VertexFormat(glm::vec3(xi, y, zi), DELIVERYCOLOUR, NORM));

		alpha += (float)2 * M_PI / CIRCLE;
	}

	//  Delivery outer circle
	alpha = 0.0f;
	for (int i = 0; i <= CIRCLE; i++) {
		float xi = RADIUS2 * cos(alpha);
		float zi = RADIUS2 * sin(alpha);
		vertices.push_back(VertexFormat(glm::vec3(xi, y, zi), DELIVERYCOLOUR, NORM));

		alpha += (float)2 * M_PI / CIRCLE;
	}

	return vertices;
}

//  Generating Delivery indices
vector<unsigned int> Delivery::generateIndices() {
	vector<unsigned int> indices;

	for (unsigned int i = 0; i < CIRCLE; i++) {
		indices.push_back(i);
		indices.push_back(i + CIRCLE + 2);
		indices.push_back(i + CIRCLE + 1);

		indices.push_back(i);
		indices.push_back(i + 1);
		indices.push_back(i + CIRCLE + 2);
	}

	indices.push_back(CIRCLE);
	indices.push_back(CIRCLE + 1);
	indices.push_back(2 * CIRCLE + 1);

	indices.push_back(CIRCLE);
	indices.push_back(0);
	indices.push_back(CIRCLE + 1);

	return indices;
}

bool Delivery::checkCollision(glm::vec3 position) {

	//  Collision error
	float err = 5.0f;

	//  Circle collision
	if (position.y > err) {
		return false;
	}

	//  Check circle lateral bounds
	float radius = RADIUS1;

	float dX = this->position.x - position.x;
	float dZ = this->position.z - position.z;
	float distance = sqrt(dX * dX + dZ * dZ);

	if (radius >= distance) {
		return true;
	}

	return false;
}