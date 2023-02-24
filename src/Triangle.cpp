#include "Triangle.h"

Triangle::~Triangle() {

}

Triangle::Triangle() {

}


bool Triangle::intersection(glm::vec3 o, glm::vec3 d, float t0, float t1, glm::vec3& t, material& rec, float& tf, glm::vec3& norm) {
	
	//for some reason t0 does not work, this is a work around
	float e = .00001f;
	//vert2 - vert1
	glm::vec3 v21 = vert2 - vert1;
	//vert3 - vert1
	glm::vec3 v31 = vert3 - vert1;

	glm::vec3 pvec = glm::cross(d, v31);
	//determinant
	float deter = glm::dot(v21, pvec);
	if (deter > -e && deter < e) {
		return false;
	}
	glm::vec3 tvec = o - vert1;
	auto u = glm::dot(tvec, pvec) / deter;
	//checking if valid barycentric
	if (u < 0.0f || u > 1.0f) {
		return false;
	}
	glm::vec3 qvec = glm::cross(tvec, v21);
	float v = glm::dot(d, qvec) / deter;
	//barycentric verification, does the point actually intersect?
	if (v < 0.0f || u + v > 1.0f) {
		return false;
	}
	float ts = glm::dot(v31, qvec) / deter;
	//is ts valid and does not hover
	if (ts < e ) {
		return false;
	}
	tf = ts;
	glm::vec3 _t = o + (d * ts);
	t = _t;
	norm = giveNorm({(1-u-v), u, v});
	return true;

}

glm::vec3 Triangle::giveNorm(const glm::vec3 ray) {
	//glm::vec3 normal = glm::normalize(glm::cross(vert2 - vert1, vert3 - vert1));

	return { glm::normalize(norm1 * ray.x + norm2 * ray.y + norm3 * ray.z) };

}