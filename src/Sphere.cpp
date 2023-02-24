#include "Sphere.h"

Sphere::Sphere(void):Shape()
{
}

bool Sphere::intersection(glm::vec3 o, glm::vec3 d, float t0, float t1, glm::vec3& t, material& rec, float& tf, glm::vec3& n) {
	float a = glm::dot(d, d);
	float b = 2 * glm::dot((o - pos), d);
	float c = glm::dot((o - pos), (o - pos)) - (radius * radius);

	float dis = (b * b) - (4 * a * c);

	if (dis < 0) {
		return false;
	}
	if (dis == 0.0f) {
		rec = mat;
		float t_s = (-b) / (2*a);
		if (t_s < t0 || t_s > t1) {
			return false;
		}
		tf = t_s;
		t = o + (t_s * d);
		glm::vec3 _t = t;
		n = giveNorm(_t);
		return true;
	}
	if (dis > 0){
		rec = mat;
		float ts1 = (-b + sqrt(dis)) / (2 * a);
		float ts2 = (-b - sqrt(dis)) / (2 * a);

		if (ts1 < ts2) {
			if (ts1 < t0 || ts1 > t1) {
				if (ts2 < t0 || ts2 > t1) {
					return false;
				}
				else {
					tf = ts2;
					t = o + (ts2 * d);
					glm::vec3 _t = t;
					n = giveNorm(_t);
					return true;
				}
			}
			tf = ts1;
			t = o + (ts1 * d);
			glm::vec3 _t = t;
			n = giveNorm(_t);
			return true;	
		}
		else {
			if (ts2 < t0 || ts2 > t1) {
				if (ts1 < t0 || ts1 > t1) {
					return false;
				}
				else {
					tf = ts1;
					t = o + (ts1 * d);
					glm::vec3 _t = t;
					n = giveNorm(_t);
					return true;
				}
			}
			tf = ts2;
			t = o + (ts2 * d);
			glm::vec3 _t = t;
			n = giveNorm(_t);
			return true;
		}
	}
}

Sphere::Sphere(glm::vec3 pos, float radius, material m):Shape(pos, m){
	this->radius = radius;
}

Sphere::~Sphere()
{
}

