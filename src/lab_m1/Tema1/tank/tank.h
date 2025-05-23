#pragma once

#include <vector>

#include "utils/glm_utils.h"
#include <core/gpu/vertex_format.h>
#include "components/simple_scene.h"

using namespace std;

namespace m1
{
	class Tank : public gfxc::SimpleScene
	{
	public:
		Tank(int resX, int resY, int index, glm::vec3 position);
		~Tank();
		vector<VertexFormat>& getVertices();
		vector<unsigned int>& getIndices();
		glm::vec3 getPosition();
		float getAngle();
		int getHits();
		void setVertices(vector<VertexFormat> vertices);
		void setIndices(vector<unsigned int> indices);
		void setPosition(glm::vec3 position);
		void setAngle(float angle);
		void setHits(int hits);

	private:
		//  Tank functions
		vector<VertexFormat> generateVertices(int resX, int resY, int index);
		vector<unsigned int> generateIndices();

	protected:
		//  Interval for x and y within tank limits
		const int STARTX = 0;
		const int ENDX = 80;
		const int STARTY = 0;
		const int ENDY = 30;

		//  Tank colour
		const glm::vec3 TANKCOLOURMAIN1 = glm::vec3(0.58f, 0.749f, 0.318f);
		const glm::vec3 TANKCOLOURSHADOW1 = glm::vec3(0.455f, 0.459f, 0.216f);
		const glm::vec3 TANKCOLOURMAIN2 = glm::vec3(0.949f, 0.576f, 0.933f);
		const glm::vec3 TANKCOLOURSHADOW2 = glm::vec3(0.788f, 0.396f, 0.773f);

		//  Tank circle precision
		const int CIRCLE = 80;

		// Normal vector
		const glm::vec3 NORM = glm::vec3(0, 1, 0);

		//  Tank
		std::vector<VertexFormat> vertices;
		std::vector<unsigned int> indices;
		glm::vec3 position;
		float angle;
		int hits;
	};
}
