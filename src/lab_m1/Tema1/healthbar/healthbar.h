#pragma once

#include <vector>

#include "utils/glm_utils.h"
#include <core/gpu/vertex_format.h>
#include "components/simple_scene.h"

using namespace std;

namespace m1
{
	class Healthbar : public gfxc::SimpleScene
	{
	public:
		Healthbar(int resX, int resY, int index, glm::vec3 position);
		~Healthbar();
		vector<VertexFormat>& getVerticesFull();
		vector<VertexFormat>& getVerticesEmpty();
		vector<unsigned int>& getIndicesFull();
		vector<unsigned int>& getIndicesEmpty();
		unsigned int& getVBOFull();
		glm::vec3 getPosition();
		void setVertices(vector<VertexFormat> verticesFull, vector<VertexFormat> verticesEmpty);
		void setIndices(vector<unsigned int> indicesFull, vector<unsigned int> indicesEmpty);
		void setPosition(glm::vec3 position);
		float barLength(int resX);

	private:
		//  Healthbar functions
		vector<VertexFormat> generateVerticesFull(int resX, int resY, int index);
		vector<VertexFormat> generateVerticesEmpty(int resX, int resY, int index);
		vector<unsigned int> generateIndicesFull();
		vector<unsigned int> generateIndicesEmpty();

	protected:
		//  Interval for x and y within Healthbar limits
		const int STARTX = 0;
		const int ENDX = 80;
		const int STARTY = 0;
		const int ENDY = 30;

		//  Healthbar colour
		const glm::vec3 HEALTHBARCOLOURMAIN1 = glm::vec3(0.58f, 0.749f, 0.318f);
		const glm::vec3 HEALTHBARCOLOURMAIN2 = glm::vec3(0.949f, 0.576f, 0.933f);

		// Normal vector
		const glm::vec3 NORM = glm::vec3(0, 1, 0);

		//  Healthbar
		std::vector<VertexFormat> verticesFull;
		std::vector<VertexFormat> verticesEmpty;
		std::vector<unsigned int> indicesFull;
		std::vector<unsigned int> indicesEmpty;
		glm::vec3 position;
		unsigned int VBO_full = 0;
	};
}
