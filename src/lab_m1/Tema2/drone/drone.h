#pragma once

#include <vector>

#include "utils/glm_utils.h"
#include <core/gpu/vertex_format.h>
#include "components/simple_scene.h"

using namespace std;

namespace m1
{
	class Drone : public gfxc::SimpleScene
	{
	public:
		Drone(int resX, int resY, glm::vec3 position);
		~Drone();
		vector<VertexFormat>& getVertices();
		vector<unsigned int>& getIndices();
		glm::vec3 getPosition();
		float getAngleOx();
		float getAngleOy();
		float getAngleOz();
		void setVertices(vector<VertexFormat> vertices);
		void setIndices(vector<unsigned int> indices);
		void setPosition(glm::vec3 position);
		void setAngleOx(float angle);
		void setAngleOy(float angle);
		void setAngleOz(float angle);
		glm::vec3 calculateRotorOffset(int resX, int resY, int index);

		//  Drone area
		const float HITBOX = 0.6f;

	private:
		//  Drone functions
		vector<VertexFormat> generateVertices(int resX, int resY);
		vector<unsigned int> generateIndices();

	protected:

		//  Drone colour
		const glm::vec3 DRONEBODY = glm::vec3(0.7f, 0.7f, 0.7f);
		const glm::vec3 DRONESUPPORT = glm::vec3(0.4f, 0.4f, 0.4f);

		// Normal vector
		const glm::vec3 NORM = glm::vec3(0, 1, 0);

		//  Drone
		std::vector<VertexFormat> vertices;
		std::vector<unsigned int> indices;
		glm::vec3 position;
		float angleOx;
		float angleOy;
		float angleOz;
	};
}

