
#include "landscape.h"
#include "core/engine.h"

using namespace m1;

Landscape::Landscape(int resX, int resY)
{
	setVertices(generateVertices(resX, resY));
	setIndices(generateIndices());
}

Landscape::~Landscape()
{
}

//  Getters
vector<VertexFormat>& Landscape::getVertices() {
	return this->vertices;
}

vector<unsigned int>& Landscape::getIndices() {
	return this->indices;
}

//  Setters
void Landscape::setVertices(vector<VertexFormat> vertices) {
	this->vertices = vertices;
}

void Landscape::setIndices(vector<unsigned int> indices) {
	this->indices = indices;
}

//  Generating Landscape vertices
vector<VertexFormat> Landscape::generateVertices(int resX, int resY) {
	vector<VertexFormat> vertices
	{
		//  Base
		VertexFormat(glm::vec3(0, 0, 0), NORM, LANDSCAPEDARK),
		VertexFormat(glm::vec3(resX, 0, 0), NORM, LANDSCAPEDARK),

		//  Top
		VertexFormat(glm::vec3(0, resY, 0), NORM, LANDSCAPELIGHT),
		VertexFormat(glm::vec3(resX, resY, 0), NORM, LANDSCAPELIGHT),
	};

	return vertices;
}

//  Generating Landscape indices
vector<unsigned int> Landscape::generateIndices() {
	vector<unsigned int> indices =
	{
		//  Base
		0, 1, 2,
		1, 2, 3
	};

	return indices;
}