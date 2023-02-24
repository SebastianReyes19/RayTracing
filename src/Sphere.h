#pragma once
#include "Shape.h"


class Sphere : public Shape
{
public:
	Sphere(void);
	Sphere(glm::vec3 pos, float radius, material m);
	bool intersection(glm::vec3 o, glm::vec3 d, float t0, float t1, glm::vec3& t, material& rec, float& tf, glm::vec3& n) override;
	glm::vec3 giveNorm(const glm::vec3 ray) override { return glm::normalize(ray - pos); } 
	~Sphere(void);

private:
	float radius;

};
