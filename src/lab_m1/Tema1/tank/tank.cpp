 
#include "tank.h"
#include "core/engine.h"

using namespace m1;

Tank::Tank(int resX, int resY, int index, glm::vec3 position)
{
	setVertices(generateVertices(resX, resY, index));
	setIndices(generateIndices());
	setPosition(position);
	setHits(0);
}

Tank::~Tank()
{
}

//  Getters
vector<VertexFormat>& Tank::getVertices() {
	return this->vertices;
}

vector<unsigned int>& Tank::getIndices() {
	return this->indices;
}

glm::vec3 Tank::getPosition() {
	return this->position;
}

float Tank::getAngle() {
	return this->angle;
}

int Tank::getHits() {
	return this->hits;
}

//  Setters
void Tank::setVertices(vector<VertexFormat> vertices) {
	this->vertices = vertices;
}

void Tank::setIndices(vector<unsigned int> indices) {
	this->indices = indices;
}

void Tank::setPosition(glm::vec3 position) {
	this->position = position;
}

void Tank::setAngle(float angle) {
	this->angle = angle;
}

void Tank::setHits(int hits) {
	this->hits = hits;
}

//  Generating tank vertices
vector<VertexFormat> Tank::generateVertices(int resX, int resY, int index) {
	glm::vec3 tankColourMain;
	glm::vec3 tankColourShadow;

	if (index == 1) {
		tankColourMain = TANKCOLOURMAIN1;
		tankColourShadow = TANKCOLOURSHADOW1;
	}
	else {
		tankColourMain = TANKCOLOURMAIN2;
		tankColourShadow = TANKCOLOURSHADOW2;
	}

	//  Circle data
	float x = (-2.5f * resX / ENDX + 2.5f * resX / ENDX) / 2;	//  0
	float y = (float) resY / ENDY;
	float r = (0.5f * resX / ENDX + 4.5f * resX / ENDX) / 5.5f;

	vector<VertexFormat> vertices
	{
		//  Base
		VertexFormat(glm::vec3(-1.5 * resX / ENDX - resX / ENDX / 2, resY / ENDY / 4, 0), NORM, tankColourShadow),
		VertexFormat(glm::vec3(-0.5 * resX / ENDX - resX / ENDX, -resY / ENDY / 4, 0), NORM, tankColourShadow),
		VertexFormat(glm::vec3(0.5 * resX / ENDX + resX / ENDX, -resY / ENDY / 4, 0), NORM, tankColourShadow),
		VertexFormat(glm::vec3(1.5 * resX / ENDX + resX / ENDX / 2, resY / ENDY / 4, 0), NORM, tankColourShadow),

		//  Top
		VertexFormat(glm::vec3(-2.5f * resX / ENDX, resY / ENDY / 4, 0), NORM, tankColourMain),
		VertexFormat(glm::vec3(-2.0f * resX / ENDX, resY / ENDY, 0), NORM, tankColourMain),
		VertexFormat(glm::vec3(2.0f * resX / ENDX,  resY / ENDY, 0), NORM, tankColourMain),
		VertexFormat(glm::vec3(2.5f * resX / ENDX, resY / ENDY / 4, 0), NORM, tankColourMain),

		//  Circle center
		VertexFormat(glm::vec3(x, y, 0), NORM, tankColourMain),
	};
	
	//  Add circle
	float alpha = 0;
	for (int i = 0; i <= CIRCLE / 2; i++) {
		float xi = x + r * cos(alpha);
		float yi = y + r * sin(alpha);
		vertices.push_back(VertexFormat(glm::vec3(xi , yi, 0), NORM, tankColourMain));

		alpha += (float) M_PI / (CIRCLE / 2);
	}

	return vertices;
}

//  Generating tank indices
vector<unsigned int> Tank::generateIndices() {
	vector<unsigned int> indices =
	{
		//  Base
		0, 1, 2,
		0, 2, 3,
		//  Top
		4, 5, 6,
		4, 6, 7
	};

	unsigned int center = 8;

	for (unsigned int i = 9; i < vertices.size() - 1; i++) {
		indices.push_back(i);
		indices.push_back(center);
		indices.push_back(i + 1);
	}

	return indices;
}