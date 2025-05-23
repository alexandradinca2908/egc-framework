#pragma once

#include <vector>

#include "utils/glm_utils.h"
#include <core/gpu/vertex_format.h>
#include "components/simple_scene.h"

using namespace std;

namespace m1
{
	class Rotor : public gfxc::SimpleScene
	{
	public:
		Rotor(int resX, int resY, glm::vec3 position);
		~Rotor();
		vector<VertexFormat>& getVertices();
		vector<unsigned int>& getIndices();
		glm::vec3 getPosition();
		float getAngleOy();
		void setVertices(vector<VertexFormat> vertices);
		void setIndices(vector<unsigned int> indices);
		void setPosition(glm::vec3 position);
		void setAngleOy(float angle);

	private:
		//  Rotor functions
		vector<VertexFormat> generateVertices(int resX, int resY);
		vector<unsigned int> generateIndices();

	protected:
		//  Rotor colour
		const glm::vec3 ROTORCOLOUR = glm::vec3(0, 0, 0);

		// Normal vector
		const glm::vec3 NORM = glm::vec3(0, 1, 0);

		//  Rotor
		std::vector<VertexFormat> vertices;
		std::vector<unsigned int> indices;
		glm::vec3 position;
		float angleOy;
	};
}


