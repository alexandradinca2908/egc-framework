#pragma once

#include <vector>

#include "utils/glm_utils.h"
#include <core/gpu/vertex_format.h>
#include "components/simple_scene.h"

using namespace std;

namespace m1
{
	class Terrain : public gfxc::SimpleScene
	{
	public:
		Terrain(int rexX, int resY);
		~Terrain();
		vector<VertexFormat>& getHeightMap();
		vector<VertexFormat>& getVertices();
		vector<unsigned int>& getIndices();
		unsigned int& getVBO();
		float getENDX();
		float getENDY();
		float getPDIST();
		void setHeightMap(vector<VertexFormat> heightMap);
		void setVertices(vector<VertexFormat> vertices);
		void setIndices(vector<unsigned int> indices);
		void levelTerrain(int resX, int resY, float deltaTimeSeconds);
		glm::vec3 spawnTank(int resX, int index);
		glm::vec3 updateTankHeight(int resX, glm::vec3 position);
		float updateTankAngle(int resX, glm::vec3 position);
		bool projectileCollision(float pX, float pY, int resX, int resY);

	private:
		//  Terrain functions
		float terrainGenerator(float x, float elevate);
		vector<VertexFormat> generateHeightMap(int resX, int resY);
		vector<VertexFormat> generateVertices(vector<VertexFormat> heightMap);
		vector<unsigned int> generateIndices(unsigned int size);

	protected:
		//  Parameters for terrain function
		const float A1 = 0.7f;
		const float A2 = 1.0f;
		const float A3 = 1.0f;
		const float O1 = 1.0f;
		const float O2 = 0.7f;
		const float O3 = 1.3f;

		//  Interval for x and y within terrain function
		const int STARTX = 0;
		const int ENDX = 40;
		const int STARTY = 0;
		const int ENDY = 18;

		//  Point data
		const float PDIST = 0.1f;				//  Point distance
		const float TOTALPOINTS = ENDX / PDIST;	//  Total points

		//  Terrain colour
		const glm::vec3 TERRAINCOLOUR1 = glm::vec3(0.455f, 0.635f, 0.702f); //  Grey blue
		const glm::vec3 TERRAINCOLOUR2 = glm::vec3(0.18f, 0.496f, 0.612f);	//  Blue

		// Normal vector
		const glm::vec3 NORM = glm::vec3(0, 1, 0);

		//  Terrain
		std::vector<VertexFormat> heightMap;
		std::vector<VertexFormat> vertices;
		std::vector<unsigned int> indices;
		unsigned int VBO = 0;
	};
}
