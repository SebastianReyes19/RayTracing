#include "Light.h"

Light::Light(void)
{
}

Light::Light(glm::vec3 pos, glm::vec3 col) {
	this->pos = pos;
	this->col = col;
}

Light::~Light(void)
{
}
