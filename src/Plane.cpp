#include "Plane.h"


Plane::Plane()
{
}

bool Plane::intersection(glm::vec3 o, glm::vec3 d, float t0, float t1, glm::vec3& t, material& rec, float& tf, glm::vec3& n) {
	float bot = glm::dot(d, norm);
	if (bot != 0) {
		float t_s = glm::dot((pos - o), norm) / bot;
		if (t_s < t0 || t_s > t1) {
			return false;
		}
		tf = t_s;
		t = o + (d * t_s);
		glm::vec3 _t = t;
		n = norm;
		rec = mat;
		return true;
	}
	return false;
}

Plane::Plane(glm::vec3 pos, glm::vec3 norm, material m):Shape(pos, m) {
	this->norm = norm;
}
Plane::~Plane()
{
}

