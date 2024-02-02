#pragma once
#include <glm/glm.hpp>

class Light
{
public:
	Light();
	~Light();

	Light(glm::vec3 pos, glm::vec3 col);

	glm::vec3 GetPosition() { return position; }
	glm::vec3 GetColor() { return color; }

private:
	glm::vec3 position;
	glm::vec3 color;

};
