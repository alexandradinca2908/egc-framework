

#include "projectile.h"
#include "core/engine.h"

using namespace m1;

Projectile::Projectile() {}

Projectile::Projectile(int resX, int resY)
{
	setVertices(generateVertices(resX, resY));
	setIndices(generateIndices());
}

Projectile::~Projectile()
{
}

//  Getters
vector<VertexFormat>& Projectile::getVertices() {
	return this->vertices;
}

vector<unsigned int>& Projectile::getIndices() {
	return this->indices;
}

glm::vec3 Projectile::getPosition() {
	return this->position;
}

glm::vec3 Projectile::getDirection() {
	return this->direction;
}

float Projectile::getAngle() {
	return this->angle;
}

int Projectile::getCollisions() {
	return this->collisions;
}

//  Setters
void Projectile::setVertices(vector<VertexFormat> vertices) {
	this->vertices = vertices;
}

void Projectile::setIndices(vector<unsigned int> indices) {
	this->indices = indices;
}

void Projectile::setPosition(glm::vec3 position) {
	this->position = position;
}

void Projectile::setDirection(glm::vec3 direction) {
	this->direction = direction;
}

void Projectile::setAngle(float angle) {
	this->angle = angle;
}

void Projectile::setCollisions(int collisions) {
	this->collisions = collisions;
}

//  Generating projectile vertices
vector<VertexFormat> Projectile::generateVertices(int resX, int resY) {
	//  Projectile is placed in the bottom of the turret
	//  Radius equals 1.5 turret width for better visibility
	float r = 0.225f * resY / ENDY;
	float x = 0;
	float y = (float) resY / ENDY;

	vector<VertexFormat> vertices
	{
		//  Circle center
		VertexFormat(glm::vec3(x, y, 0), NORM, PROJECTILECOLOUR),
	};

	float alpha = 0;
	for (int i = 0; i <= CIRCLE; i++) {
		float xi = x + r * cos(alpha);
		float yi = y + r * sin(alpha);
		vertices.push_back(VertexFormat(glm::vec3(xi, yi, 0), NORM, PROJECTILECOLOUR));

		alpha += (float) 2 * M_PI / CIRCLE;
	}

	return vertices;
}

//  Generating projectile indices
vector<unsigned int> Projectile::generateIndices() {
	vector<unsigned int> indices;

	unsigned int center = 0;
	for (unsigned int i = 1; i < vertices.size() - 1; i++) {
		indices.push_back(i);
		indices.push_back(center);
		indices.push_back(i + 1);
	}

	return indices;
}