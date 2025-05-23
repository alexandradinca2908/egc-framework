#include "terrain.h"
#include "core/engine.h"

using namespace m1;

Terrain::Terrain(int resX, int resY)
{
	setHeightMap(generateHeightMap(resX, resY));
	setVertices(generateVertices(heightMap));
	setIndices(generateIndices((unsigned int) vertices.size()));
}

Terrain::~Terrain()
{
}

//  Getters
vector<VertexFormat>& Terrain::getHeightMap() {
	return this->heightMap;
}

vector<VertexFormat>& Terrain::getVertices() {
	return this->vertices;
}

vector<unsigned int>& Terrain::getIndices() {
	return this->indices;
}

unsigned int& Terrain::getVBO() {
	return this->VBO;
}

float Terrain::getENDX() {
	return (float) this->ENDX;
}

float Terrain::getENDY() {
	return (float) this->ENDY;
}

float Terrain::getPDIST() {
	return this->PDIST;
}

//  Setters
void Terrain::setHeightMap(vector<VertexFormat> heightMap) {
	this->heightMap = heightMap;
}

void Terrain::setVertices(vector<VertexFormat> vertices) {
	this->vertices = vertices;
}

void Terrain::setIndices(vector<unsigned int> indices) {
	this->indices = indices;
}

//  Calculates y value for given x on terrain
float Terrain::terrainGenerator(float x, float elevate) {
	return A1 * sin(O1 * x) + A2 * sin(O2 * x) + A3 * sin(O3 * x) + elevate;
}

//  Generating height map
vector<VertexFormat> Terrain::generateHeightMap(int resX, int resY) {
	vector<VertexFormat> heightMap;

	for (float x = (float) STARTX; x < (float) ENDX; x += PDIST) {
		//  Calculate y
		float y = terrainGenerator(x, (float) resY / 100);

		//  Convert from x, y coordinates to resolution offset
		float offsetX = (((float)resX) / (ENDX - 1)) * x;
		float offsetY = ((float)resY) / (ENDY - 1) * y;

		//  Add to height map
		heightMap.push_back(VertexFormat(glm::vec3(offsetX, offsetY, 0), NORM, TERRAINCOLOUR1));
	}

	return heightMap;
}

//  Generating terrain vertices
vector<VertexFormat> Terrain::generateVertices(vector<VertexFormat> heightMap) {
	vector<VertexFormat> vertices;

	for (VertexFormat vertex : heightMap) {
		//  For every height vertex, project a vertex on x		
		vertices.push_back(vertex);
		vertices.push_back(VertexFormat(glm::vec3(vertex.position.x, 0, 0), NORM, TERRAINCOLOUR2));
	}

	return vertices;
}

//  Generating terrain indices
vector<unsigned int> Terrain::generateIndices(unsigned int size) {
	vector<unsigned int> indices;

	for (unsigned int i = 0; i < size; i++) {
		//  Add indices in groups of 3	
		indices.push_back(i - 2);
		indices.push_back(i - 1);
		indices.push_back(i);
	}

	return indices;
}
//  Even out terrain with heights exceeding threshold
void Terrain::levelTerrain(int resX, int resY, float deltaTimeSeconds) {
	bool up;
	constexpr float threshold = glm::radians(35.0f);

	//  Level slopes
	for (int i = 0; i < heightMap.size() - 1; i++) {
		VertexFormat v1 = heightMap[i];
		VertexFormat v2 = heightMap[i + 1];

		float pointDiff = v1.position.y - v2.position.y;

		if (pointDiff < 0) {
			up = true;
		}
		else {
			up = false;
		}

		float pointCount = 0.0f;

		float x = v2.position.x - v1.position.x;
		float y = v2.position.y - v1.position.y;

		float slope = fabs(atan2(y, x));

		if (slope > threshold) {
			//  Calculate epsilon factor for regulating slopes
			float newY = x * tan(threshold);
			float epsilon = fabs((y - newY) / 2.0f);

			//  Increase speed for slower frames
			if (deltaTimeSeconds <= 0.5f) {
				deltaTimeSeconds *= 1.05f;
			}

			//  Upward slopes: increase - decrease
			if (up) {
				epsilon *= deltaTimeSeconds;
			}
			//  Downward slopes: decrease - increase
			else {
				epsilon *= -deltaTimeSeconds;
			}

			heightMap[i].position.y += epsilon;
			heightMap[i + 1].position.y -= epsilon;

			vertices[2 * i].position.y += epsilon;
			vertices[2 * (i + 1)].position.y -= epsilon;
		}
	}

	//  Soften corners
	for (int i = 1; i < heightMap.size() - 1; i++) {
		VertexFormat v1 = heightMap[i];
		VertexFormat v2 = heightMap[i - 1];
		VertexFormat v3 = heightMap[i + 1];

		if (v1.position.y > v2.position.y && v1.position.y > v3.position.y) {
			float averageHeight = (v1.position.y - v2.position.y + v1.position.y - v3.position.y) / 2.0f;

			heightMap[i].position.y -= averageHeight;
			vertices[2 * i].position.y -= averageHeight;

			heightMap[i - 1].position.y -= averageHeight / 2.0f;
			vertices[2 * (i - 1)].position.y -= averageHeight / 2.0f;

			heightMap[i + 1].position.y -= averageHeight / 2.0f;
			vertices[2 * (i + 1)].position.y -= averageHeight / 2.0f;
		} else if (v1.position.y < v2.position.y && v1.position.y < v3.position.y) {
			float averageHeight = (v2.position.y - v1.position.y + v3.position.y - v1.position.y) / 2.0f;

			heightMap[i].position.y += averageHeight;
			vertices[2 * i].position.y += averageHeight;

			heightMap[i - 1].position.y += averageHeight / 2.0f;
			vertices[2 * (i - 1)].position.y += averageHeight / 2.0f;

			heightMap[i + 1].position.y += averageHeight / 2.0f;
			vertices[2 * (i + 1)].position.y += averageHeight / 2.0f;
		}
	}
}

//  Calculates spawn coordinates for tanks
glm::vec3 Terrain::spawnTank(int resX, int index) {
	float x;
	float elemNr;
	//  First tank spawn point ~ first quarter
	if (index == 1) {
		x = 6.30f;

	}
	//  Second tank spawn point ~ third quarter
	else {
		x = 35.0f;
	}

	//  Function is drawn on interval [0, 40]
	//  Height map has 400 elements, one for every multiple of 0.1
	//  For a given x, translate its value in height map index
	elemNr = x / PDIST;

	float offsetX = (((float)resX) / (ENDX - 1)) * x;
	float offsetY = heightMap[(unsigned int) elemNr].position.y;

	return glm::vec3(offsetX, offsetY, 0);
}

glm::vec3 Terrain::updateTankHeight(int resX, glm::vec3 position) {
	//  Calculate x in xOy
	float offsetX = position.x;
	float x = offsetX * (ENDX - 1) / resX;

	//  Function is drawn on interval [0, 40]
	//  Height map has 400 elements, one for every multiple of 0.1
	//  For a given x, translate its value in height map index
	float elemNr = x / PDIST;

	float offsetY = heightMap[(unsigned int) elemNr].position.y;

	return glm::vec3(offsetX, offsetY, 0);
}

float Terrain::updateTankAngle(int resX, glm::vec3 position) {
	//  Calculate x in xOy
	float offsetX = position.x;
	float xCenter = offsetX * (ENDX - 1) / resX;

	float xLeft = xCenter - 1;
	float xRight = xCenter + 1;

	if (xLeft < 0) {
		xLeft = 0;
	}
	if (xRight > resX) {
		xRight = (float) resX;
	}

	//  Function is drawn on interval [0, 40]
	//  Height map has 400 elements, one for every multiple of 0.1
	//  For a given x, translate its value in height map index
	float elemNrLeft = xLeft / PDIST;
	float elemNrRight = xRight / PDIST;

	float offsetX1 = heightMap[(unsigned int) elemNrLeft].position.x;
	float offsetY1 = heightMap[(unsigned int) elemNrLeft].position.y;
	float offsetX2 = heightMap[(unsigned int) elemNrRight].position.x;
	float offsetY2 = heightMap[(unsigned int) elemNrRight].position.y;

	return atan2((offsetY2 - offsetY1), (offsetX2 - offsetX1));
}

bool Terrain::projectileCollision(float pX, float pY, int resX, int resY) {
	float x = pX * (ENDX - 1) / resX;

	//  Calculate x's neighbours
	float xLeft = floor(x * 10) / 10;
	float xRight = xLeft + PDIST;

	//  Extract pixel height
	float elemNrLeft = xLeft / PDIST;
	float elemNrRight = xRight / PDIST;

	if (elemNrLeft < 0) {
		elemNrLeft = 0;
	}

	if (elemNrRight >= heightMap.size()) {
		elemNrRight = (float) (heightMap.size() - 1);
	}

	float offsetY1 = heightMap[(unsigned int) elemNrLeft].position.y;
	float offsetY2 = heightMap[(unsigned int) elemNrRight].position.y;

	//  Convert to xOy coordinates
	float yLeft = offsetY1 * (ENDY - 1) / resY;
	float yRight = offsetY2 * (ENDY - 1) / resY;

	//  Interpolate
	float t = (x - xLeft) / (xRight - xLeft);
	float y = yLeft * (1 - t) + yRight * t;

	//  Convert
	float height = ((float)resY) / (ENDY - 1) * y;

	//  Compare and establish if a collision occured
	float threshold = resY * 0.0001f;
	if (pY - height <= threshold) {
		//  Create blast
		float blast = resX / ((ENDX - 1) / PDIST) * 20.0f;

		//  Calculate AOE in coordinates
		float startBlast = (pX - blast) * (ENDX - 1) / resX;
		startBlast = startBlast < 0 ? 0 : startBlast;
		float endBlast = (pX + blast) * (ENDX - 1) / resX;
		endBlast = endBlast > ENDX ? ENDX : endBlast;
		
		//  Find coordinates in height map
		elemNrLeft = startBlast / PDIST;
		elemNrRight = endBlast / PDIST;

		//  Build a semi-circle around the center
		for (int i = (int) elemNrLeft; i <= (int) elemNrRight; i++) {
			float crtX = heightMap[i].position.x;

			//  Calculate distance from the blast center
			float dx = crtX - pX;

			//  Calculate angle between point and center
			float theta = acos(dx / blast);

			//  Calculate new y
			float newY = pY - blast * sin(theta);

			//  Update
			if (heightMap[i].position.y > newY) {
				heightMap[i].position.y = newY;
				vertices[2 * i].position.y = newY;
			}

			if (heightMap[i].position.y < 0) {
				heightMap[i].position.y = 0;
				vertices[2 * i].position.y = 0;
			}
		}
		return true;
	}

	return false;
}