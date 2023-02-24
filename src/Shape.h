#pragma once
#include <glm/glm.hpp>

struct material {
	glm::vec3 ka, kd, ks, km;
	float s;
	material() {};
	material(glm::vec3 _ka, glm::vec3 _kd, glm::vec3 _ks, glm::vec3 _km, float _s) : ka(_ka), kd(_kd), ks(_ks), km(_km),s(_s) {}
};

class Shape
{
public:
	Shape(void);
	Shape(glm::vec3 pos, material mat);
	~Shape(void);

	const glm::vec3 givePos() { return pos; }
	const material giveMat() { return mat; }
	virtual bool intersection(glm::vec3 o, glm::vec3 d, float t0, float t1, glm::vec3& t, material& rec, float& tf, glm::vec3& n);
	virtual glm::vec3 giveNorm(const glm::vec3 ray) { return glm::vec3(0.0f); }

protected:
	glm::vec3 pos;
	material mat;
};
