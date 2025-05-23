
#pragma once

#include <vector>

#include "utils/glm_utils.h"
#include <core/gpu/vertex_format.h>
#include "components/simple_scene.h"

using namespace std;

namespace m1
{
	class Landscape : public gfxc::SimpleScene
	{
	public:
		Landscape(int resX, int resY);
		~Landscape();
		vector<VertexFormat>& getVertices();
		vector<unsigned int>& getIndices();
		void setVertices(vector<VertexFormat> vertices);
		void setIndices(vector<unsigned int> indices);

	private:
		//  Landscape functions
		vector<VertexFormat> generateVertices(int resX, int resY);
		vector<unsigned int> generateIndices();

	protected:
		//  Landscape colour
		const glm::vec3 LANDSCAPELIGHT= glm::vec3(0.427f, 0, 0.612f);	//  Purple
		const glm::vec3 LANDSCAPEDARK = glm::vec3(0.125f, 0, 0.612f);	//  Blue

		// Normal vector
		const glm::vec3 NORM = glm::vec3(0, 1, 0);

		//  Landscape
		std::vector<VertexFormat> vertices;
		std::vector<unsigned int> indices;
	};
}
