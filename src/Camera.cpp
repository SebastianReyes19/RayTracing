#include "Camera.h"
#include <iostream>

Camera::Camera()
{
}

Camera::~Camera()
{
}

Camera::Camera(int widthRes, int heightRes, glm::vec3 eye, glm::vec3 lookAt, glm::vec3 up, float fov, float focal, int rays)
{
	this->widthRes = widthRes;
	this->heightRes = heightRes;
	renderedImage = new float[widthRes * heightRes * 3];
	this->eye = eye;
	this->lookAt = lookAt;
	this->up = up;
	this->fov = fov;
	this->focal = focal;
	this->rays = rays;

	auto a = eye - lookAt;
	auto b = up;
	this->w = a / glm::length(a);
	this->u = glm::cross(b, this->w);
	glm::normalize(this->u);
	this->v = glm::cross(w, u);

	
}

void Camera::computePrimaryRay(float x, float y, glm::vec3& ray) {
	float ly = 2*focal * glm::tan(fov/2);
	float lx = ly * float(float(widthRes) / float(heightRes));
	float Pw = ly / float(heightRes);

	glm::vec3 O = (glm::normalize(lookAt - eye) * focal) - ((lx / 2) * u) - ((ly / 2) * v) + eye;

	glm::vec3 Pc = O + ((Pw * (x + .5f)) * u) + ((Pw * (y + .5f)) * v);

	ray = Pc;

};


//t0, min start..., t1, max end. think of it as the caps so we dont do too much ray computing..
glm::vec3 Camera::computeRayColor(Scene* scene, glm::vec3 o, glm::vec3 d, float t0, float t1, int depth, int curr) {
	// o is the initial 
	//initial color to return
	glm::vec3 color(0.0f);
	//termination depending on depth
	if (depth <= curr) {
		return color;
	}
	int ncurr = curr + 1;
	// intitial vars
	glm::vec3 v_hit; // rt that hit some object
	material rec; // material of hit object
	glm::vec3 norm; // hit normal

	auto lights = scene->giveLights();

	if (scene->Hit(o, d, t0, t1, rec, v_hit, norm)) {
		
		glm::vec3 ori = v_hit; // represents the origin of the hit
		glm::vec3 N = norm; // normal of the hit
		glm::vec3 dir = glm::normalize(glm::reflect(d, N)); // direction the hit is going

		color = rec.ka;
		for (auto i = 0; i < scene->numLights(); i++) {
			//getting light items
			glm::vec3 lightCol = lights[i]->giveLightCol();
			glm::vec3 lightPos = lights[i]->givePos();
			//getting t_light parameter
			
			//getting Li
			glm::vec3 Li = glm::normalize(lightPos - v_hit);
			glm::vec3 Ri = (glm::reflect(-Li, N));
			glm::vec3 E = glm::normalize(eye - v_hit); 
			
			float t_light = glm::dot(lightPos - ori, Li);

			// trash variables
			glm::vec3 temp;
			glm::vec3 t;
			material srec;
			//
			if (!scene->Hit(v_hit, Li, t0, t_light, srec, t, temp)) {
				color += lightCol * (( rec.kd * glm::max(0.0f, glm::dot(Li, N)))
					+ (rec.ks * glm::pow(glm::max(0.0f, glm::dot(Ri, E)), rec.s)));
			}
		}
		auto reflected = computeRayColor(scene, ori, dir, t0, t1, depth, ncurr);
		color += (rec.km * reflected);
	} 

	return color;
}

void Camera::TakePicture(Scene *scene)
{
	for (int y = 0; y < heightRes; y++) {
		for (int x = 0; x < widthRes; x++) {
			glm::vec3 P_ray;
			computePrimaryRay(x, y, P_ray);
			glm::vec3 o = eye;
			glm::vec3 d = P_ray - eye;

			glm::vec3 color = computeRayColor(scene, o, d, 0.001f, FLT_MAX, rays, 0);

			renderedImage[(y * widthRes * 3) + (x * 3) + 0] = color.x;
			renderedImage[(y * widthRes * 3) + (x * 3) + 1] = color.y;
			renderedImage[(y * widthRes * 3) + (x * 3) + 2] = color.z;
		}
	}
	//memset(renderedImage, 0, sizeof(float) * widthRes * heightRes * 3);
}


