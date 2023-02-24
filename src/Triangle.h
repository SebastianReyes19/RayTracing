#pragma once
#include "Shape.h"

class Triangle : public Shape
{
public:
	Triangle(void);

	Triangle(glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3, glm::vec3 norm1, glm::vec3 norm2, glm::vec3 norm3) :
		vert1(pos1), vert2(pos2), vert3(pos3), norm1(norm1), norm2(norm2), norm3(norm3) {};
	
	bool intersection(glm::vec3 o, glm::vec3 d, float t0, float t1, glm::vec3& t, material& rec, float& tf, glm::vec3& norm) override;

	glm::vec3 giveVert1() { return vert1; }
	glm::vec3 giveVert2() { return vert2; }
	glm::vec3 giveVert3() { return vert3; }

	glm::vec3 giveNorm(const glm::vec3 ray) override;
	~Triangle();

private:
	glm::vec3 vert1, vert2, vert3;
	glm::vec3 norm1, norm2, norm3;
};
