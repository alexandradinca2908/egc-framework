
#include "alien.h"
#include "core/engine.h"

using namespace m1;

Alien::Alien(int resX, int resY, glm::vec3 position)
{
	setVertices(generateVertices(resX, resY));
	setIndices(generateIndices());
	setPosition(position);
	setAngle(0);
	setLeft(true);
	setGrabbed(false);
}

Alien::~Alien()
{
}

//  Getters
vector<VertexFormat>& Alien::getVertices() {
	return this->vertices;
}

vector<unsigned int>& Alien::getIndices() {
	return this->indices;
}

glm::vec3 Alien::getPosition() {
	return this->position;
}

float Alien::getAngle() {
	return this->angle;
}

bool Alien::getLeft() {
	return this->left;
}

bool Alien::getGrabbed() {
	return this->grabbed;
}

//  Setters
void Alien::setVertices(vector<VertexFormat> vertices) {
	this->vertices = vertices;
}

void Alien::setIndices(vector<unsigned int> indices) {
	this->indices = indices;
}

void Alien::setPosition(glm::vec3 position) {
	this->position = position;
}

void Alien::setAngle(float angle) {
	this->angle = angle;
}

void Alien::setLeft(bool left) {
	this->left = left;
}

void Alien::setGrabbed(bool grabbed) {
	this->grabbed = grabbed;
}

//  Generating Alien vertices
vector<VertexFormat> Alien::generateVertices(int resX, int resY) {
	//  Ellipse data - UFO
	float x = 0;
	float y = (float) -resY / ENDY;

	float a = 5.0f * resX / ENDX;
	float b = (float) resY / ENDY;

	vector<VertexFormat> vertices
	{
		//  Ellipse center
		VertexFormat(glm::vec3(x, y, 0), NORM, UFOCOLOUR1)
	};

	//  Add ellipse points
	float alpha = 0;
	for (int i = 0; i <= 2 * CIRCLE; i++) {
		float xi = x + a * cos(alpha);
		float yi = y + b * sin(alpha);
		vertices.push_back(VertexFormat(glm::vec3(xi, yi, 0), NORM, UFOCOLOUR2));

		alpha += (float) 2 * M_PI / CIRCLE;
	}

	//  Triangle data - Alien eyes
	x = resX / ENDX / 4.0f;
	y = (-resY / ENDY + 1.5f * b) / 2.0f;

	//  Left eye
	vertices.push_back(VertexFormat(glm::vec3(-x, y, 0), NORM, ALIENEYES));
	vertices.push_back(VertexFormat(glm::vec3(-2.5f * x, y, 0), NORM, ALIENEYES));
	vertices.push_back(VertexFormat(glm::vec3(-1.5f * x, 1.5f * y, 0), NORM, ALIENEYES));

	//  Right eye
	vertices.push_back(VertexFormat(glm::vec3(x, y, 0), NORM, ALIENEYES));
	vertices.push_back(VertexFormat(glm::vec3(2.5f * x, y, 0), NORM, ALIENEYES));
	vertices.push_back(VertexFormat(glm::vec3(1.5f * x, 1.5f * y, 0), NORM, ALIENEYES));

	//  Circle data - Alien head
	x = 0;
	y = -resY / ENDY + 1.5f * b;
	float r = resY / ENDY / 2.0f;

	//  Add center
	vertices.push_back(VertexFormat(glm::vec3(x, y, 0), NORM, ALIENCOLOUR));

	//  Add circle
	alpha = 0;
	for (int i = 0; i <= CIRCLE; i++) {
		float xi = x + r * cos(alpha);
		float yi = y + r * sin(alpha);
		vertices.push_back(VertexFormat(glm::vec3(xi, yi, 0), NORM, ALIENCOLOUR));

		alpha += (float) 2 * M_PI / CIRCLE;
	}

	//  Triangle - Alien chin
	vertices.push_back(VertexFormat(glm::vec3(x - r, y, 0), NORM, ALIENCOLOUR));
	vertices.push_back(VertexFormat(glm::vec3(x + r, y, 0), NORM, ALIENCOLOUR));
	vertices.push_back(VertexFormat(glm::vec3(x, y - 3.0f * r, 0), NORM, ALIENCOLOUR));

	//  Circle data - Glass chamber
	x = 0;
	y = -resY / ENDY + b;
	r = resY / ENDY * 1.3f;

	//  Add center
	vertices.push_back(VertexFormat(glm::vec3(x, y, 0), NORM, GLASSCOLOUR1));

	//  Add circle
	alpha = 0;
	for (int i = 0; i <= CIRCLE; i++) {
		float xi = x + r * cos(alpha);
		float yi = y + r * sin(alpha);
		vertices.push_back(VertexFormat(glm::vec3(xi, yi, 0), NORM, GLASSCOLOUR2));

		alpha += (float) 2 * M_PI / CIRCLE;
	}

	//  Triangle data - UFO light
	x = 0;
	y = -resY / ENDY - b;
	vertices.push_back(VertexFormat(glm::vec3(x, y + r, 0), NORM, ALIENLIGHT1));
	vertices.push_back(VertexFormat(glm::vec3(x + a, y - 6.0f * r, 0), NORM, ALIENLIGHT2));
	vertices.push_back(VertexFormat(glm::vec3(x - a, y - 6.0f * r, 0), NORM, ALIENLIGHT2));

	return vertices;
}

//  Generating Alien indices
vector<unsigned int> Alien::generateIndices() {
	vector<unsigned int> indices;
	unsigned int i = 0;
	unsigned int center = 0;
	unsigned int start = 0;
	unsigned int end = 0;

	//  UFO
	center = 0;
	start = 1;
	end = (unsigned int)(2 * CIRCLE + 1);
	for (i = start; i < end; i++) {
		indices.push_back(i);
		indices.push_back(center);
		indices.push_back(i + 1);
	}

	//  Alien
	//  Eyes
	start = end + 1;
	end = start + 6;
	for (i = start; i <= end; i += 3) {
		indices.push_back(i);
		indices.push_back(i + 1);
		indices.push_back(i + 2);
	}

	//  Head
	center = end + 1;
	start = center + 1;
	end = start + CIRCLE + 1;
	for (i = start; i < end; i++) {
		indices.push_back(i);
		indices.push_back(center);
		indices.push_back(i + 1);
	}

	//  Chin
	indices.push_back(i - 1);
	indices.push_back(i);
	indices.push_back(i + 1);

	//  Glass
	center = i + 2;
	start = center + 1;
	end = start + CIRCLE + 1;

	for (i = start; i < end; i++) {
		indices.push_back(i);
		indices.push_back(center);
		indices.push_back(i + 1);
	}

	//  Light
	indices.push_back(i);
	indices.push_back(i + 1);
	indices.push_back(i + 2);

	return indices;
}