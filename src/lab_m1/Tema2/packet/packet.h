#pragma once

#include <vector>

#include "utils/glm_utils.h"
#include <core/gpu/vertex_format.h>
#include "components/simple_scene.h"

using namespace std;

namespace m1
{
	class Packet : public gfxc::SimpleScene
	{
	public:
		Packet(int resX, int resY);
		~Packet();
		vector<VertexFormat>& getVertices();
		vector<unsigned int>& getIndices();
		vector<VertexFormat> generateVertices2D(int resX, int resY);
		vector<unsigned int> generateIndices2D();
		glm::vec3 getPosition();
		glm::vec3 getStartPosition();
		float getAngleOx();
		float getAngleOy();
		float getAngleOz();
		float getScale();
		void setVertices(vector<VertexFormat> vertices);
		void setIndices(vector<unsigned int> indices);
		void setPosition(glm::vec3 position);
		void setStartPosition(glm::vec3 position);
		void setAngleOx(float angle);
		void setAngleOy(float angle);
		void setAngleOz(float angle);
		void setScale(float scale);
		bool checkCollision(glm::vec3 position);

		//  Packet area
		const float HITBOX = 0.6f;

	private:
		//  Packet functions
		vector<VertexFormat> generateVertices(int resX, int resY);
		vector<unsigned int> generateIndices();

	protected:
		//  Packet colour
		const glm::vec3 PACKETCOLOUR1 = glm::vec3(0.82f, 0.608f, 0.329f);
		const glm::vec3 PACKETCOLOUR2 = glm::vec3(1.0f, 1.0f, 0.729f);
		const glm::vec3 PACKETCOLOUR3 = glm::vec3(1.0f, 1.0f, 0);

		// Normal vector
		const glm::vec3 NORM = glm::vec3(0, 1, 0);

		//  Packet
		std::vector<VertexFormat> vertices;
		std::vector<unsigned int> indices;
		glm::vec3 position;
		glm::vec3 startPosition;
		float angleOx;
		float angleOy;
		float angleOz;
		float scale;
	};
}


