#pragma once

#include <vector>

#include "utils/glm_utils.h"
#include <core/gpu/vertex_format.h>
#include "components/simple_scene.h"

using namespace std;

namespace m1
{
	class Alien : public gfxc::SimpleScene
	{
	public:
		Alien(int resX, int resY, glm::vec3 position);
		~Alien();
		vector<VertexFormat>& getVertices();
		vector<unsigned int>& getIndices();
		glm::vec3 getPosition();
		float getAngle();
		bool getLeft();
		bool getGrabbed();
		void setVertices(vector<VertexFormat> vertices);
		void setIndices(vector<unsigned int> indices);
		void setPosition(glm::vec3 position);
		void setAngle(float angle);
		void setLeft(bool left);
		void setGrabbed(bool grabbed);

	private:
		//  Alien functions
		vector<VertexFormat> generateVertices(int resX, int resY);
		vector<unsigned int> generateIndices();

	protected:
		//  Interval for x and y within alien limits
		const int STARTX = 0;
		const int ENDX = 80;
		const int STARTY = 0;
		const int ENDY = 30;

		//  Alien colour
		const glm::vec3 UFOCOLOUR1 = glm::vec3(0.043f, 0.8f, 0.0f);
		const glm::vec3 UFOCOLOUR2 = UFOCOLOUR1 - glm::vec3(0.0f, 0.4f, 0.0f);
		const glm::vec3 GLASSCOLOUR1 = glm::vec3(0.4f, 0.4f, 0.6f);
		const glm::vec3 GLASSCOLOUR2 = GLASSCOLOUR1 + glm::vec3(0.3f, 0.3f, 0.3f);
		const glm::vec3 ALIENCOLOUR = glm::vec3(0.9f, 0.9f, 0.9f);
		const glm::vec3 ALIENEYES = glm::vec3(0, 0, 0);
		const glm::vec3 ALIENLIGHT1 = glm::vec3(0.8f, 0.8f, 0);		//  Light
		const glm::vec3 ALIENLIGHT2 = glm::vec3(0.300f, 0, 0.612f); //  Dark

		//  Alien circle precision
		const int CIRCLE = 80;

		// Normal vector
		const glm::vec3 NORM = glm::vec3(0, 1, 0);

		//  Alien
		std::vector<VertexFormat> vertices;
		std::vector<unsigned int> indices;
		glm::vec3 position;
		float angle;
		bool left;
		bool grabbed;
	};
}

