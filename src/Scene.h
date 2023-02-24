#pragma once
#include <vector>
#include "Shape.h"
#include "Sphere.h"
#include "Plane.h"
#include "Light.h"


class Scene
{
public:
	Scene();
	~Scene();
	void addLight(Light* light);
	void addShape(Shape* shape);
	bool Hit(glm::vec3 o, glm::vec3 d, float t0, float t1, material& rec, glm::vec3& v_hit, glm::vec3& n);

	std::vector<Shape*> giveShapes() { return this->shapes; };
	std::vector<Light*> giveLights() { return this->lights; };
	size_t numShapes() { return shapes.size(); };
	size_t numLights() { return lights.size(); };

private:
	std::vector<Shape*> shapes;
	std::vector<Light*> lights;

};
