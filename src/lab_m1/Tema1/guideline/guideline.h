
#pragma once

#include <vector>

#include "utils/glm_utils.h"
#include <core/gpu/vertex_format.h>
#include "components/simple_scene.h"

using namespace std;

namespace m1
{
	class Guideline : public gfxc::SimpleScene
	{
	public:
		Guideline(int resX, int resY);
		~Guideline();
		vector<VertexFormat>& getVertices();
		vector<unsigned int>& getIndices();
		glm::vec3 getPosition();
		glm::vec3 getDirection();
		float getAngle();
		int getCollisions();
		void setVertices(vector<VertexFormat> vertices);
		void setIndices(vector<unsigned int> indices);
		void setPosition(glm::vec3 position);
		void setDirection(glm::vec3 direction);
		void setAngle(float angle);
		bool inScreen(int resX, int resY);
		glm::vec3 initialStart(int resY);

	private:
		//  Guideline functions
		vector<VertexFormat> generateVertices(int resX, int resY);
		vector<unsigned int> generateIndices();

	protected:
		//  Interval for x and y within Guideline limits
		const int STARTX = 0;
		const int ENDX = 80;
		const int STARTY = 0;
		const int ENDY = 30;

		//  Guideline colour
		//const glm::vec3 GUIDELINECOLOUR = glm::vec3(0.682f, 0.878f, 1.0f);
		const glm::vec3 GUIDELINECOLOUR = glm::vec3(0.5f, 0.5f, 0.5f);

		// Normal vector
		const glm::vec3 NORM = glm::vec3(0, 1, 0);

		//  Guideline
		std::vector<VertexFormat> vertices;
		std::vector<unsigned int> indices;
		glm::vec3 position;
		glm::vec3 direction;
		float angle;
		int collisions = 0;
	};
}
