 
#pragma once

#include <vector>

#include "utils/glm_utils.h"
#include <core/gpu/vertex_format.h>
#include "components/simple_scene.h"

using namespace std;

namespace m1
{
	class Turret : public gfxc::SimpleScene
	{
	public:
		Turret(int resX, int resY);
		~Turret();
		vector<VertexFormat>& getVertices();
		vector<unsigned int>& getIndices();
		glm::vec3 getPosition();
		float getAngle();
		void setVertices(vector<VertexFormat> vertices);
		void setIndices(vector<unsigned int> indices);
		void setPosition(glm::vec3 position);
		void setAngle(float angle);
		glm::vec3 initialStart(int resY);

	private:
		//  Turret functions
		vector<VertexFormat> generateVertices(int resX, int resY);
		vector<unsigned int> generateIndices();

	protected:
		//  Interval for x and y within turret limits
		const int STARTX = 0;
		const int ENDX = 80;
		const int STARTY = 0;
		const int ENDY = 30;

		//  Turret colour
		const glm::vec3 TURRETCOLOUR = glm::vec3(1, 1, 1);

		// Normal vector
		const glm::vec3 NORM = glm::vec3(0, 1, 0);

		//  Turret
		std::vector<VertexFormat> vertices;
		std::vector<unsigned int> indices;
		glm::vec3 position;
		float angle;
	};
}
