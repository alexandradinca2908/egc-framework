#pragma once

#include <vector>

#include "utils/glm_utils.h"
#include <core/gpu/vertex_format.h>
#include "components/simple_scene.h"

using namespace std;

namespace m1
{
	class Tree : public gfxc::SimpleScene
	{
	public:
		Tree(int resX, int resY, glm::vec3 position);
		~Tree();
		vector<VertexFormat>& getVertices();
		vector<unsigned int>& getIndices();
		vector<VertexFormat> generateVertices2D(int resX, int resY);
		vector<unsigned int> generateIndices2D();
		glm::vec3 getPosition();
		float getAngleOx();
		float getAngleOy();
		float getAngleOz();
		float getScale();
		void setVertices(vector<VertexFormat> vertices);
		void setIndices(vector<unsigned int> indices);
		void setPosition(glm::vec3 position);
		void setAngleOx(float angle);
		void setAngleOy(float angle);
		void setAngleOz(float angle);
		void setScale(float scale);
		bool checkCollision(glm::vec3 position);

	private:
		//  Tree functions
		vector<VertexFormat> generateVertices(int resX, int resY);
		vector<unsigned int> generateIndices();

	protected:
		//  Tree trunk accuracy
		const int STARTX = 0;
		const unsigned int ENDX = 50;
		const int STARTY = 0;
		const unsigned int ENDY = 50;
		const unsigned int CIRCLE = 80;

		//  Tree colour
		const glm::vec3 TREETRUNK = glm::vec3(0.459f, 0.235f, 0.024f);
		const glm::vec3 TREELEAVES1 = glm::vec3(0.503f, 0.80f, 0.209f);
		const glm::vec3 TREELEAVES2 = glm::vec3(0.729f, 0.941f, 0.408f);
		const glm::vec3 TREELEAVES3 = glm::vec3(0.839f, 0.484f, 0.259f);
		const glm::vec3 TREELEAVES4 = glm::vec3(0, 0.484f, 0.1f);

		// Normal vector
		const glm::vec3 NORM = glm::vec3(0, 1, 0);

		//  Tree
		std::vector<VertexFormat> vertices;
		std::vector<unsigned int> indices;
		glm::vec3 position;
		float angleOx;
		float angleOy;
		float angleOz;
		float scale;
	};
}


