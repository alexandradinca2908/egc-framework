
#include "packet.h"
#include "core/engine.h"

using namespace m1;

Packet::Packet(int resX, int resY)
{
	setVertices(generateVertices(resX, resY));
	setIndices(generateIndices());
	setPosition(glm::vec3(0, 0, 0));
	setStartPosition(glm::vec3(0, 0, 0));
	setAngleOx(0);
	setAngleOy(0);
	setAngleOz(0);
	setScale(0);
}

Packet::~Packet()
{
}

//  Getters
vector<VertexFormat>& Packet::getVertices() {
	return this->vertices;
}

vector<unsigned int>& Packet::getIndices() {
	return this->indices;
}

glm::vec3 Packet::getPosition() {
	return this->position;
}

glm::vec3 Packet::getStartPosition() {
	return this->startPosition;
}

float Packet::getAngleOx() {
	return this->angleOx;
}

float Packet::getAngleOy() {
	return this->angleOy;
}

float Packet::getAngleOz() {
	return this->angleOz;
}

float Packet::getScale() {
	return this->scale;
}

//  Setters
void Packet::setVertices(vector<VertexFormat> vertices) {
	this->vertices = vertices;
}

void Packet::setIndices(vector<unsigned int> indices) {
	this->indices = indices;
}

void Packet::setPosition(glm::vec3 position) {
	this->position = position;
}

void Packet::setStartPosition(glm::vec3 position) {
	this->startPosition = position;
}

void Packet::setAngleOx(float angle) {
	this->angleOx = angle;
}

void Packet::setAngleOy(float angle) {
	this->angleOy = angle;
}

void Packet::setAngleOz(float angle) {
	this->angleOz = angle;
}

void Packet::setScale(float scale) {
	this->scale = scale;
}

//  Generating Drone vertices
vector<VertexFormat> Packet::generateVertices(int resX, int resY) {
	vector<VertexFormat> vertices
	{
		VertexFormat(glm::vec3(-0.5f, 0.0f, -0.5f), PACKETCOLOUR1, NORM),
		VertexFormat(glm::vec3(0.5f, 0.0f, -0.5f), PACKETCOLOUR1, NORM),
		VertexFormat(glm::vec3(0.5f, 0.0f, 0.5f), PACKETCOLOUR1, NORM),
		VertexFormat(glm::vec3(-0.5f, 0.0f, 0.5f), PACKETCOLOUR1, NORM),
		VertexFormat(glm::vec3(-0.5f, 0.5f, -0.5f), PACKETCOLOUR2, NORM),
		VertexFormat(glm::vec3(0.5f, 0.5f, -0.5f), PACKETCOLOUR2, NORM),
		VertexFormat(glm::vec3(0.5f, 0.5f, 0.5f), PACKETCOLOUR2, NORM),
		VertexFormat(glm::vec3(-0.5f, 0.5f, 0.5f), PACKETCOLOUR2, NORM),
	};

	return vertices;
}

//  Generating Drone indices
vector<unsigned int> Packet::generateIndices() {
	vector<unsigned int> indices
	{
		//  Leg 1
		0, 2, 3,
		0, 1, 2,
		3, 6, 7,
		3, 2, 6,
		0, 7, 4,
		0, 3, 7,
		1, 4, 5,
		1, 0, 4,
		2, 5, 6,
		2, 1, 5,
		7, 5, 4,
		7, 6, 5
	};

	return indices;
}

vector<VertexFormat> Packet::generateVertices2D(int resX, int resY) {
	vector<VertexFormat> vertices
	{
		VertexFormat(glm::vec3(-0.5f, 0.5f, -0.5f), PACKETCOLOUR3, NORM),
		VertexFormat(glm::vec3(0.5f, 0.5f, -0.5f), PACKETCOLOUR3, NORM),
		VertexFormat(glm::vec3(0.5f, 0.5f, 0.5f), PACKETCOLOUR3, NORM),
		VertexFormat(glm::vec3(-0.5f, 0.5f, 0.5f), PACKETCOLOUR3, NORM)
	};

	return vertices;
}

vector<unsigned int> Packet::generateIndices2D() {
	vector<unsigned int> indices
	{
		//  Leg 1
		0, 1, 2,
		0, 2, 3
	};

	return indices;
}

bool Packet::checkCollision(glm::vec3 position) {
	//  Box upper limits
	float baseHeight = 0.0f;
	float topHeight = this->position.y + 0.5 * this->scale;

	//  Collision error
	float err = 1.5f;

	//  Box collision
	if (position.y > topHeight + err) {
		return false;
	}

	//  Check box lateral bounds
	float radius = 0.75f * this->scale;

	float dX = this->position.x - position.x;
	float dZ = this->position.z - position.z;
	float distance = sqrt(dX * dX + dZ * dZ);

	if (radius + err >= distance) {
		return true;
	}

	return false;
}