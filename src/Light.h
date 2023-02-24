#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Light
{
public:
	Light();
	Light(glm::vec3 pos, glm::vec3 col);
	glm::vec3 givePos() { return pos; }
	glm::vec3 giveLightCol() { return col; };
	~Light();

private:
	glm::vec3 pos;
	glm::vec3 col;

};
