#pragma once
#include <vector>
#include "Scene.h"



class Camera
{
public:
	Camera();
	~Camera();

	Camera(int widthRes, int heightRes, glm::vec3 eye, glm::vec3 lookAt, glm::vec3 up, float fov, float focal, int rays);

	void TakePicture(Scene *scene);
	float* GetRenderedImage() { return renderedImage; };
	void computePrimaryRay(float x, float y, glm::vec3& ray);
	glm::vec3 computeRayColor(Scene* scene, glm::vec3 o, glm::vec3 d, float t0, float t1, int depth, int curr);

private:

	int widthRes, heightRes, rays;
	glm::vec3 eye, lookAt, up, w, u, v;
	float fov, focal;
	float *renderedImage;

};
