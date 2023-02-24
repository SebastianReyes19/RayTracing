#pragma once
#include "Shape.h"

class Plane : public Shape
{
public:
	Plane(void);
	Plane(glm::vec3 pos, glm::vec3 norm, material m);
	bool intersection(glm::vec3 o, glm::vec3 d, float t0, float t1, glm::vec3& t, material& rec, float& tf, glm::vec3& n) override;

	glm::vec3 giveNorm(const glm::vec3 ray) override { return glm::normalize(norm); }
	~Plane(void);

private:

	glm::vec3 norm;

};
