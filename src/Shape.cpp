#include "Shape.h"

Shape::Shape(void)
{
}

bool Shape::intersection(glm::vec3 o, glm::vec3 d, float t0, float t1, glm::vec3& t, material& rec, float& tf, glm::vec3& n) {
	return false;
}

Shape::Shape(glm::vec3 pos, material mat) {
	this->pos = pos;
	this->mat = mat;
}
Shape::~Shape(void)
{
}

