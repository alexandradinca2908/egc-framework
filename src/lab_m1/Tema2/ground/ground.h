#pragma once

#include <vector>

#include "utils/glm_utils.h"
#include <core/gpu/vertex_format.h>
#include "components/simple_scene.h"

using namespace std;

namespace m1
{
	class Ground : public gfxc::SimpleScene
	{
	public:
		Ground(int resX, int resY);
		~Ground();
		vector<VertexFormat>& getVertices();
		vector<unsigned int>& getIndices();
		void setVertices(vector<VertexFormat> vertices);
		void setIndices(vector<unsigned int> indices);
		float calculateHeight(float x, float y);

	private:
		//  Ground functions
		vector<VertexFormat> generateVertices(int resX, int resY);
		vector<unsigned int> generateIndices();
		float random(glm::vec2 st);

	protected:
		//  Interval for x and y within ground limits
		const int STARTX = 0;
		const unsigned int ENDX = 100;
		const int STARTY = 0;
		const unsigned int ENDY = 0;

		//  Ground colour
		const glm::vec3 GROUNDCOLOUR = glm::vec3(0.329f, 0.729f, 0.369f);

		// Normal vector
		const glm::vec3 NORM = glm::vec3(0, 1, 0);

		//  Ground
		std::vector<VertexFormat> vertices;
		std::vector<unsigned int> indices;
	};
}


