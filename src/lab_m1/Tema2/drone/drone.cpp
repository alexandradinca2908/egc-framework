
#include "drone.h"
#include "core/engine.h"

using namespace m1;

Drone::Drone(int resX, int resY, glm::vec3 position)
{
	setVertices(generateVertices(resX, resY));
	setIndices(generateIndices());
	setPosition(position);
	setAngleOx(0);
	setAngleOy(85.0f);
	setAngleOz(0);
}

Drone::~Drone()
{
}

//  Getters
vector<VertexFormat>& Drone::getVertices() {
	return this->vertices;
}

vector<unsigned int>& Drone::getIndices() {
	return this->indices;
}

glm::vec3 Drone::getPosition() {
	return this->position;
}

float Drone::getAngleOx() {
	return this->angleOx;
}

float Drone::getAngleOy() {
	return this->angleOy;
}

float Drone::getAngleOz() {
	return this->angleOz;
}

//  Setters
void Drone::setVertices(vector<VertexFormat> vertices) {
	this->vertices = vertices;
}

void Drone::setIndices(vector<unsigned int> indices) {
	this->indices = indices;
}

void Drone::setPosition(glm::vec3 position) {
	this->position = position;
}

void Drone::setAngleOx(float angle) {
	this->angleOx = angle;
}

void Drone::setAngleOy(float angle) {
	this->angleOy = angle;
}

void Drone::setAngleOz(float angle) {
	this->angleOz = angle;
}

//  Generating Drone vertices
vector<VertexFormat> Drone::generateVertices(int resX, int resY) {
	vector<VertexFormat> vertices
	{
		//  Drone body
		//  Leg 1
		VertexFormat(glm::vec3(-0.07f * resX / ENDX, -0.07f * resY / ENDY, -0.6f * resX / ENDX), DRONEBODY, NORM),
		VertexFormat(glm::vec3(-0.07f * resX / ENDX, 0.07f * resY / ENDY, -0.6f * resX / ENDX), DRONEBODY, NORM),
		VertexFormat(glm::vec3(0.07f * resX / ENDX, -0.07f * resY / ENDY, -0.6f * resX / ENDX), DRONEBODY, NORM),
		VertexFormat(glm::vec3(0.07f * resX / ENDX, 0.07f * resY / ENDY, -0.6f * resX / ENDX), DRONEBODY, NORM),
		VertexFormat(glm::vec3(-0.07f * resX / ENDX, -0.07f * resY / ENDY, 0.6f * resX / ENDX), DRONEBODY, NORM),
		VertexFormat(glm::vec3(-0.07f * resX / ENDX, 0.07f * resY / ENDY, 0.6f * resX / ENDX), DRONEBODY, NORM),
		VertexFormat(glm::vec3(0.07f * resX / ENDX, -0.07f * resY / ENDY, 0.6f * resX / ENDX), DRONEBODY, NORM),
		VertexFormat(glm::vec3(0.07f * resX / ENDX, 0.07f * resY / ENDY, 0.6f * resX / ENDX), DRONEBODY, NORM),

		//  Leg 2
		VertexFormat(glm::vec3(-0.6f * resX / ENDX, -0.07f * resY / ENDY, -0.07f * resX / ENDX), DRONEBODY, NORM),
		VertexFormat(glm::vec3(-0.6f * resX / ENDX, 0.07f * resY / ENDY, -0.07f * resX / ENDX), DRONEBODY, NORM),
		VertexFormat(glm::vec3(-0.6f * resX / ENDX, -0.07f * resY / ENDY, 0.07f * resX / ENDX), DRONEBODY, NORM),
		VertexFormat(glm::vec3(-0.6f * resX / ENDX, 0.07f * resY / ENDY, 0.07f * resX / ENDX), DRONEBODY, NORM),
		VertexFormat(glm::vec3(0.6f * resX / ENDX, -0.07f * resY / ENDY, -0.07f * resX / ENDX), DRONEBODY, NORM),
		VertexFormat(glm::vec3(0.6f * resX / ENDX, 0.07f * resY / ENDY, -0.07f * resX / ENDX), DRONEBODY, NORM),
		VertexFormat(glm::vec3(0.6f * resX / ENDX, -0.07f * resY / ENDY, 0.07f * resX / ENDX), DRONEBODY, NORM),
		VertexFormat(glm::vec3(0.6f * resX / ENDX, 0.07f * resY / ENDY, 0.07f * resX / ENDX), DRONEBODY, NORM),

		//  Leg 1 rotors supports
		//  Rotor 1
		VertexFormat(glm::vec3(-0.07f * resX / ENDX, 0.07f * resY / ENDY, -0.6f * resX / ENDX), DRONESUPPORT, NORM),
		VertexFormat(glm::vec3(0.07f * resX / ENDX, 0.07f * resY / ENDY, -0.6f * resX / ENDX), DRONESUPPORT, NORM),
		VertexFormat(glm::vec3(-0.07f * resX / ENDX, 0.07f * resY / ENDY, -0.45f * resX / ENDX), DRONESUPPORT, NORM),
		VertexFormat(glm::vec3(0.07f * resX / ENDX, 0.07f * resY / ENDY, -0.45f * resX / ENDX), DRONESUPPORT, NORM),
		VertexFormat(glm::vec3(-0.07f * resX / ENDX, 0.15f * resY / ENDY, -0.6f * resX / ENDX), DRONESUPPORT, NORM),
		VertexFormat(glm::vec3(0.07f * resX / ENDX, 0.15f * resY / ENDY, -0.6f * resX / ENDX), DRONESUPPORT, NORM),
		VertexFormat(glm::vec3(-0.07f * resX / ENDX, 0.15f * resY / ENDY, -0.45f * resX / ENDX), DRONESUPPORT, NORM),
		VertexFormat(glm::vec3(0.07f * resX / ENDX, 0.15f * resY / ENDY, -0.45f * resX / ENDX), DRONESUPPORT, NORM),

		//  Rotor 2
		VertexFormat(glm::vec3(-0.07f * resX / ENDX, 0.07f * resY / ENDY, 0.6f * resX / ENDX), DRONESUPPORT, NORM),
		VertexFormat(glm::vec3(0.07f * resX / ENDX, 0.07f * resY / ENDY, 0.6f * resX / ENDX), DRONESUPPORT, NORM),
		VertexFormat(glm::vec3(-0.07f * resX / ENDX, 0.07f * resY / ENDY, 0.45f * resX / ENDX), DRONESUPPORT, NORM),
		VertexFormat(glm::vec3(0.07f * resX / ENDX, 0.07f * resY / ENDY, 0.45f * resX / ENDX), DRONESUPPORT, NORM),
		VertexFormat(glm::vec3(-0.07f * resX / ENDX, 0.15f * resY / ENDY, 0.6f * resX / ENDX), DRONESUPPORT, NORM),
		VertexFormat(glm::vec3(0.07f * resX / ENDX, 0.15f * resY / ENDY, 0.6f * resX / ENDX), DRONESUPPORT, NORM),
		VertexFormat(glm::vec3(-0.07f * resX / ENDX, 0.15f * resY / ENDY, 0.45f * resX / ENDX), DRONESUPPORT, NORM),
		VertexFormat(glm::vec3(0.07f * resX / ENDX, 0.15f * resY / ENDY, 0.45f * resX / ENDX), DRONESUPPORT, NORM),

		//  Leg 2 - rotor supports
		//  Rotor 1
		VertexFormat(glm::vec3(-0.6f * resX / ENDX, 0.07f * resY / ENDY, -0.07f * resX / ENDX), DRONESUPPORT, NORM),
		VertexFormat(glm::vec3(-0.6f * resX / ENDX, 0.07f * resY / ENDY, 0.07f * resX / ENDX), DRONESUPPORT, NORM),
		VertexFormat(glm::vec3(-0.45f * resX / ENDX, 0.07f * resY / ENDY, -0.07f * resX / ENDX), DRONESUPPORT, NORM),
		VertexFormat(glm::vec3(-0.45f * resX / ENDX, 0.07f * resY / ENDY, 0.07f * resX / ENDX), DRONESUPPORT, NORM),
		VertexFormat(glm::vec3(-0.6f * resX / ENDX, 0.15f * resY / ENDY, -0.07f * resX / ENDX), DRONESUPPORT, NORM),
		VertexFormat(glm::vec3(-0.6f * resX / ENDX, 0.15f * resY / ENDY, 0.07f * resX / ENDX), DRONESUPPORT, NORM),
		VertexFormat(glm::vec3(-0.45f * resX / ENDX, 0.15f * resY / ENDY, -0.07f * resX / ENDX), DRONESUPPORT, NORM),
		VertexFormat(glm::vec3(-0.45f * resX / ENDX, 0.15f * resY / ENDY, 0.07f * resX / ENDX), DRONESUPPORT, NORM),

		//  Rotor 2
		VertexFormat(glm::vec3(0.6f * resX / ENDX, 0.07f * resY / ENDY, -0.07f * resX / ENDX), DRONESUPPORT, NORM),
		VertexFormat(glm::vec3(0.6f * resX / ENDX, 0.07f * resY / ENDY, 0.07f * resX / ENDX), DRONESUPPORT, NORM),
		VertexFormat(glm::vec3(0.45f * resX / ENDX, 0.07f * resY / ENDY, -0.07f * resX / ENDX), DRONESUPPORT, NORM),
		VertexFormat(glm::vec3(0.45f * resX / ENDX, 0.07f * resY / ENDY, 0.07f * resX / ENDX), DRONESUPPORT, NORM),
		VertexFormat(glm::vec3(0.6f * resX / ENDX, 0.15f * resY / ENDY, -0.07f * resX / ENDX), DRONESUPPORT, NORM),
		VertexFormat(glm::vec3(0.6f * resX / ENDX, 0.15f * resY / ENDY, 0.07f * resX / ENDX), DRONESUPPORT, NORM),
		VertexFormat(glm::vec3(0.45f * resX / ENDX, 0.15f * resY / ENDY, -0.07f * resX / ENDX), DRONESUPPORT, NORM),
		VertexFormat(glm::vec3(0.45f * resX / ENDX, 0.15f * resY / ENDY, 0.07f * resX / ENDX), DRONESUPPORT, NORM),
	};

	return vertices;
}

//  Generating Drone indices
vector<unsigned int> Drone::generateIndices() {
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
		4, 6, 7, 

		//  Leg 2
		10, 9, 11,
		10, 8, 9,
		13, 11, 9,
		13, 15, 11,
		14, 11, 15,
		14, 10, 11,
		14, 8, 10,
		14, 12, 8,
		8, 13, 9,
		8, 12, 13,
		12, 15, 13,
		12, 14, 15,

		//  Leg 1 - rotor 1 support
		22, 21, 20,
		22, 23, 21,
		18, 23, 22,
		18, 19, 23,
		19, 21, 23,
		19, 17, 21,
		17, 20, 21,
		17, 16, 20,
		16, 22, 20,
		16, 18, 22,

		//  Leg 1 - rotor 2 support
		30, 29, 28,
		30, 31, 29,
		26, 31, 30,
		26, 27, 31,
		27, 29, 31,
		27, 25, 29,
		25, 28, 29,
		25, 24, 28,
		24, 30, 28,
		24, 26, 30,

		//  Leg 2 - rotor 1 support
		38, 37, 36,
		38, 39, 37,
		34, 39, 38,
		34, 35, 39,
		35, 37, 39,
		35, 33, 37,
		33, 36, 37,
		33, 32, 36,
		32, 38, 36,
		32, 34, 38,

		//  Leg 2 - rotor 2 support
		46, 45, 44,
		46, 47, 45,
		42, 47, 46,
		42, 43, 47,
		43, 45, 47,
		43, 41, 45,
		41, 44, 45,
		41, 40, 44,
		40, 46, 44,
		40, 42, 46
	};

	return indices;
}

glm::vec3 Drone::calculateRotorOffset(int resX, int resY, int index) {
	float rotorOffset = 0.02f * resY / ENDY;

	if (index == 0) {
		return glm::vec3(0, 0.15f * resY / ENDY + rotorOffset, -0.525f * resX / ENDX);
	}
	else if (index == 1) {
		return glm::vec3(0, 0.15f * resY / ENDY + rotorOffset, 0.525f * resX / ENDX);
	}
	else if (index == 2) {
		return glm::vec3(-0.525f * resX / ENDX, 0.15f * resY / ENDY + rotorOffset, 0);
	}
	else {
		return glm::vec3(0.525f * resX / ENDX, 0.15f * resY / ENDY + rotorOffset, 0);
	}
}