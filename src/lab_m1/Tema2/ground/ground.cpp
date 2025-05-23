 

#include "ground.h"
#include "core/engine.h"

using namespace m1;

Ground::Ground(int resX, int resY)
{
	setVertices(generateVertices(resX, resY));
	setIndices(generateIndices());
}

Ground::~Ground()
{
}

//  Getters
vector<VertexFormat>& Ground::getVertices() {
	return this->vertices;
}

vector<unsigned int>& Ground::getIndices() {
	return this->indices;
}

//  Setters
void Ground::setVertices(vector<VertexFormat> vertices) {
	this->vertices = vertices;
}

void Ground::setIndices(vector<unsigned int> indices) {
	this->indices = indices;
}

//  Generating ground vertices
vector<VertexFormat> Ground::generateVertices(int resX, int resY) {
	vector<VertexFormat> vertices;

	for (int z = 0; z < ENDX; z++) {
		for (int x = 0; x < ENDX; x++) {
			vertices.push_back(VertexFormat(glm::vec3(x, 0, z), GROUNDCOLOUR, NORM));
		}
	}

	return vertices;
}

//  Generating ground indices
vector<unsigned int> Ground::generateIndices() {
	vector<unsigned int> indices;

	for (unsigned int i = 0; i < ENDX - 1; i++) {
		for (unsigned int x = 0; x < ENDX - 1; x++) {
			//  First triangle
			indices.push_back(x + i * ENDX);
			indices.push_back(x + (i + 1) * ENDX);
			indices.push_back(x + i * ENDX + 1);

			//  Second triangle
			indices.push_back(x + (i + 1) * ENDX);
			indices.push_back(x + (i + 1) * ENDX + 1);
			indices.push_back(x + i * ENDX + 1);
		}
	}

	return indices;
}

float Ground::random(glm::vec2 st) {
	return glm::fract(sin(dot(st, glm::vec2(12.9898f, 78.233f))) * 129.5f);
}

float Ground::calculateHeight(float x, float z) {
	glm::vec2 st = glm::vec2(x * 0.1f, z * 0.1f);

	glm::vec2 i = glm::floor(st);
	glm::vec2 f = glm::fract(st);

	float a = random(i);
	float b = random(i + glm::vec2(1.0f, 0.0f));
	float c = random(i + glm::vec2(0.0f, 1.0f));
	float d = random(i + glm::vec2(1.0f, 1.0f));

	glm::vec2 u = f * f * (3.0f - 2.0f * f);

	return (glm::mix(a, b, u.x) +
		(c - a) * u.y * (1.0f - u.x) +
		(d - b) * u.x * u.y) * 4;
}