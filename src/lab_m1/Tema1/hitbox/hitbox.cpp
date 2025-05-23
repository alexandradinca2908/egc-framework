
#include "hitbox.h"
#include "core/engine.h"

using namespace m1;

Hitbox::Hitbox(int resX, int resY, glm::vec3 position, float angle)
{
	setVertices(generateVertices(resX, resY));
	setIndices(generateIndices());
	setPosition(position);
	setAngle(angle);
}

Hitbox::~Hitbox()
{
}

//  Getters
vector<VertexFormat>& Hitbox::getVertices() {
	return this->vertices;
}

vector<unsigned int>& Hitbox::getIndices() {
	return this->indices;
}

glm::vec3 Hitbox::getPosition() {
	return this->position;
}

float Hitbox::getAngle() {
	return this->angle;
}

//  Setters
void Hitbox::setVertices(vector<VertexFormat> vertices) {
	this->vertices = vertices;
}

void Hitbox::setIndices(vector<unsigned int> indices) {
	this->indices = indices;
}

void Hitbox::setPosition(glm::vec3 position) {
	this->position = position;
}

void Hitbox::setAngle(float angle) {
	this->angle = angle;
}

//  Generating Hitbox vertices
vector<VertexFormat> Hitbox::generateVertices(int resX, int resY) {
	//  Ellipse data
	float x = 0;
	float y = (resY / ENDY + resY / ENDY / 4) / 2.0f ;

	float tankLength = 5.0f * resX / ENDX;
	float a = tankLength / 2;

	float baseHeight = resY / ENDY / 2.0f;
	float topHeight = resY / ENDY / 3.0f;
	float b = (baseHeight + topHeight) / 2.0f;

	vector<VertexFormat> vertices
	{
		//  Ellipse center - set in tank center
		VertexFormat(glm::vec3(x, y, 0), NORM, glm::vec3(1, 1, 1))
	};

	//  Add ellipse points
	float alpha = 0;
	for (int i = 0; i <= CIRCLE; i++) {
		float xi = x + a * cos(alpha);
		float yi = y + b * sin(alpha);
		vertices.push_back(VertexFormat(glm::vec3(xi, yi, 0), NORM, glm::vec3(1, 1, 1)));

		alpha += (float) 2 * M_PI / CIRCLE;
	}

	return vertices;
}

//  Generating Hitbox indices
vector<unsigned int> Hitbox::generateIndices() {
	vector<unsigned int> indices;

	unsigned int center = 0;

	for (unsigned int i = 1; i < vertices.size() - 1; i++) {
		indices.push_back(i);
		indices.push_back(center);
		indices.push_back(i + 1);
	}

	return indices;
}

bool Hitbox::projectileCollision(float pX, float pY, int resX, int resY) {
	//  Calculate hitbox center coordinates
	float cX = this->getPosition().x;
	float cY = this->getPosition().y;

	//  Translate point to elipse xOy
	float tX = pX - cX;
	float tY = pY - cY;

	//  Rotate the point
	float rX = tX * cos(-this->getAngle()) - tY * sin(-this->getAngle());
	float rY = tX * sin(-this->getAngle()) + tY * cos(-this->getAngle());

	//  Take a and b of the equation
	float tankLength = 5.0f * resX / ENDX;
	float a = tankLength / 2;

	int baseHeight = resY / ENDY / 2;
	int topHeight = resY / ENDY / 3;
	float b = (baseHeight + topHeight) / 2.0f;

	//  Check ellipse equation to see if point is within hitbox
	if (rX * rX / (a * a) + rY * rY / (b * b) <= 1) {
		return true;
	}
	return false;
}