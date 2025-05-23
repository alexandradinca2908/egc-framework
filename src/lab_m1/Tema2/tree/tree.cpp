
#include "tree.h"
#include "core/engine.h"

using namespace m1;

Tree::Tree(int resX, int resY, glm::vec3 position)
{
	setVertices(generateVertices(resX, resY));
	setIndices(generateIndices());
	setPosition(position);
	setAngleOx(0);
	setAngleOy(0);
	setAngleOz(0);
	setScale(0);
}

Tree::~Tree()
{
}

//  Getters
vector<VertexFormat>& Tree::getVertices() {
	return this->vertices;
}

vector<unsigned int>& Tree::getIndices() {
	return this->indices;
}

glm::vec3 Tree::getPosition() {
	return this->position;
}

float Tree::getAngleOx() {
	return this->angleOx;
}

float Tree::getAngleOy() {
	return this->angleOy;
}

float Tree::getAngleOz() {
	return this->angleOz;
}

float Tree::getScale() {
	return this->scale;
}

//  Setters
void Tree::setVertices(vector<VertexFormat> vertices) {
	this->vertices = vertices;
}

void Tree::setIndices(vector<unsigned int> indices) {
	this->indices = indices;
}

void Tree::setPosition(glm::vec3 position) {
	this->position = position;
}

void Tree::setAngleOx(float angle) {
	this->angleOx = angle;
}

void Tree::setAngleOy(float angle) {
	this->angleOy = angle;
}

void Tree::setAngleOz(float angle) {
	this->angleOz = angle;
}

void Tree::setScale(float scale) {
	this->scale = scale;
}

//  Generating Tree vertices
vector<VertexFormat> Tree::generateVertices(int resX, int resY) {
	vector<VertexFormat> vertices;

	//  Tree trunk
	//  Base
	float alpha = 0.0f;
	float radius = 0.3f;
	float y = 0.0f;

	for (int i = 0; i <= CIRCLE; i++) {
		float xi = radius * cos(alpha);
		float zi = radius * sin(alpha);
		vertices.push_back(VertexFormat(glm::vec3(xi, y, zi), TREETRUNK, NORM));

		alpha += (float)2 * M_PI / CIRCLE;
	}

	//  Top
	alpha = 0.0f;
	y = 3.0f;

	for (int i = 0; i <= CIRCLE; i++) {
		float xi = radius * cos(alpha);
		float zi = radius * sin(alpha);
		vertices.push_back(VertexFormat(glm::vec3(xi, y, zi), TREETRUNK, NORM));

		alpha += (float)2 * M_PI / CIRCLE;
	}

	//  First leaf cone
	alpha = 0.0f;
	radius = 1.3f;
	y = 1.8f;

	for (int i = 0; i <= CIRCLE; i++) {
		float xi = radius * cos(alpha);
		float zi = radius * sin(alpha);
		vertices.push_back(VertexFormat(glm::vec3(xi, y, zi), TREELEAVES1, NORM));

		alpha += (float)2 * M_PI / CIRCLE;
	}

	y = 4.0f;
	vertices.push_back(VertexFormat(glm::vec3(0, y, 0), TREELEAVES2, NORM));

	//  Second leaf cone
	alpha = 0.0f;
	radius = 1.0f;
	y = 2.7f;

	for (int i = 0; i <= CIRCLE; i++) {
		float xi = radius * cos(alpha);
		float zi = radius * sin(alpha);
		vertices.push_back(VertexFormat(glm::vec3(xi, y, zi), TREELEAVES2, NORM));

		alpha += (float)2 * M_PI / CIRCLE;
	}

	y = 4.5f;
	vertices.push_back(VertexFormat(glm::vec3(0, y, 0), TREELEAVES3, NORM));

	return vertices;
}

//  Generating Tree indices
vector<unsigned int> Tree::generateIndices() {
	vector<unsigned int> indices;

	//  Trunk
	for (unsigned int i = 0; i < CIRCLE; i++) {
		indices.push_back(i);
		indices.push_back(i + CIRCLE + 1);
		indices.push_back(i + CIRCLE + 2);

		indices.push_back(i + 1);
		indices.push_back(i);
		indices.push_back(i + CIRCLE + 2);
	}

	indices.push_back(CIRCLE);
	indices.push_back(2 * CIRCLE + 1);
	indices.push_back(CIRCLE + 1);

	indices.push_back(0);
	indices.push_back(CIRCLE);
	indices.push_back(CIRCLE + 1);

	//  First leaf cone
	for (unsigned int i = 2 * CIRCLE + 2; i < 3 * CIRCLE + 2; i++) {
		indices.push_back(i);
		indices.push_back(3 * CIRCLE + 3);
		indices.push_back(i + 1);
	}

	indices.push_back(3 * CIRCLE + 2);
	indices.push_back(3 * CIRCLE + 3);
	indices.push_back(2 * CIRCLE + 2);

	//  Second leaf cone
	for (unsigned int i = 3 * CIRCLE + 4; i < 4 * CIRCLE + 4; i++) {
		indices.push_back(i);
		indices.push_back(4 * CIRCLE + 5);
		indices.push_back(i + 1);
	}

	indices.push_back(4 * CIRCLE + 4);
	indices.push_back(4 * CIRCLE + 5);
	indices.push_back(3 * CIRCLE + 4);

	return indices;
}

vector<VertexFormat> Tree::generateVertices2D(int resX, int resY) {
	vector<VertexFormat> vertices;

	vertices.push_back(VertexFormat(glm::vec3(0, 1.8f, 0), TREELEAVES4, NORM));

	float alpha = 0.0f;
	float radius = 1.3f;
	float y = 1.8f;

	for (int i = 0; i <= CIRCLE; i++) {
		float xi = radius * cos(alpha);
		float zi = radius * sin(alpha);
		vertices.push_back(VertexFormat(glm::vec3(xi, y, zi), TREELEAVES4, NORM));

		alpha += (float)2 * M_PI / CIRCLE;
	}

	return vertices;
}

//  Generating Tree indices
vector<unsigned int> Tree::generateIndices2D() {
	vector<unsigned int> indices;

	//  Trunk
	for (unsigned int i = 1; i < CIRCLE; i++) {
		indices.push_back(i);
		indices.push_back(0);
		indices.push_back(i + 1);
	}

	indices.push_back(CIRCLE);
	indices.push_back(0);
	indices.push_back(1);

	return indices;
}


bool Tree::checkCollision(glm::vec3 position) {
	//  Cone limits
	float baseHeight = this->position.y + 1.8f * this->scale;
	float topHeight = this->position.y + 4.5f * this->scale;

	//  Cylinder collision when object is under the cone
	//  Check object height relative to leaves
	if (position.y < baseHeight) {
		//  Check trunk bounds
		float radius = 0.3f * this->scale;

		float dX = this->position.x - position.x;
		float dZ = this->position.z - position.z;
		float distance = sqrt(dX * dX + dZ * dZ);

		if (radius >= distance) {
			return true;
		}
		else {
			return false;
		}
	}

	//  Cone collision
	//  Check object height relative to leaves
	if (position.y > topHeight) {
		return false;
	}

	//  Check cone bounds
	float radius = 1.3f * this->scale;
	float objHeight = position.y - baseHeight;
	float maxHeight = topHeight - baseHeight;
	float yRadius = radius * (1.0f - objHeight / maxHeight);

	float dX = this->position.x - position.x;
	float dZ = this->position.z - position.z;
	float distance = sqrt(dX * dX + dZ * dZ);

	if (yRadius >= distance) {
		return true;
	}

	return false;
}