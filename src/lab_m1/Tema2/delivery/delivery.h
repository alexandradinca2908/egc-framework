#pragma once

#include <vector>

#include "utils/glm_utils.h"
#include <core/gpu/vertex_format.h>
#include "components/simple_scene.h"

using namespace std;

namespace m1
{
	class Delivery : public gfxc::SimpleScene
	{
	public:
		Delivery(int resX, int resY);
		~Delivery();
		vector<VertexFormat>& getVertices();
		vector<unsigned int>& getIndices();
		glm::vec3 getPosition();
		float getAngleOx();
		float getAngleOy();
		float getAngleOz();
		float getScale();
		void setVertices(vector<VertexFormat> vertices);
		void setIndices(vector<unsigned int> indices);
		void setPosition(glm::vec3 position);
		void setAngleOx(float angle);
		void setAngleOy(float angle);
		void setAngleOz(float angle);
		void setScale(float scale);
		bool checkCollision(glm::vec3 position);

	private:
		//  Delivery functions
		vector<VertexFormat> generateVertices(int resX, int resY);
		vector<unsigned int> generateIndices();

	protected:
		//  Delivery trunk accuracy
		const int STARTX = 0;
		const unsigned int ENDX = 50;
		const int STARTY = 0;
		const unsigned int ENDY = 50;
		const unsigned int CIRCLE = 80;
		const unsigned int RADIUS1 = 2.9f;
		const unsigned int RADIUS2 = 3.5f;

		//  Delivery colour
		const glm::vec3 DELIVERYCOLOUR = glm::vec3(1.0f, 0.5f, 0);

		// Normal vector
		const glm::vec3 NORM = glm::vec3(0, 1, 0);

		//  Delivery
		std::vector<VertexFormat> vertices;
		std::vector<unsigned int> indices;
		glm::vec3 position;
		float angleOx;
		float angleOy;
		float angleOz;
		float scale;
	};
}


