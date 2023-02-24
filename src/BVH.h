#pragma once
#include "Shape.h"
#include "Triangle.h"
#include <vector>

class BVH : public Shape
{
public:
	BVH();
	BVH(char* filename, material m);

	bool intersection(glm::vec3 o, glm::vec3 d, float t0, float t1, glm::vec3& t, material& rec, float& tf, glm::vec3& norm) override;

	void LoadModel(char* name);
	void MinMaxBox(Triangle* tri);
	bool HitsBox(glm::vec3 o, glm::vec3 d);

	glm::vec3 giveNorm(glm::vec3 ray) override;
	~BVH();

private:
	std::vector<float> posBuff;
	std::vector<float> norBuff;
	std::vector<Triangle*> triangles;

	float X_min = FLT_MAX, Y_min = FLT_MAX, Z_min = FLT_MAX;
	float X_max = FLT_MIN, Y_max = FLT_MIN, Z_max = FLT_MIN;

	glm::vec3 boxMin;
	glm::vec3 boxMax;

};