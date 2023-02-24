#include "Scene.h"
#include <iostream>

Scene::~Scene()
{
}

void Scene::addLight(Light* light) {
	lights.push_back(light);
}

void Scene::addShape(Shape* shape) {
	shapes.push_back(shape);
}

bool Scene::Hit(glm::vec3 o, glm::vec3 d, float t0, float t1, material& rec, glm::vec3& v_hit, glm::vec3& n) {
	//for all shapes..
	float min_t = FLT_MAX;
	glm::vec3 norm;
	material cMat;
	glm::vec3 phit;
	bool hit = false;

	for (auto i = 0; i < numShapes(); i++) {
		glm::vec3 _t; //
		glm::vec3 _n;
		float tf;
		if (shapes[i]->intersection(o, d, t0, t1, _t, rec, tf, _n)) {
			if (min_t > tf) {
				hit = true;
				min_t = tf;
				phit = o + (d*tf);
				norm = _n;
				cMat = shapes[i]->giveMat();
			}
		}
	}
	//if we hit something
	if (hit == false) {
		return false;
	}
	v_hit = phit;
	rec = cMat;
	n = norm;
	return true;
	
};

Scene::Scene()
{
	
}
