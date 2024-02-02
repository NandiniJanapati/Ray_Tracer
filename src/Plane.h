#pragma once
#include "Shape.h"

class Plane :
	public Shape
{
public:
	Plane();
	~Plane();

	Plane(glm::vec3 _kd, glm::vec3 _ks, glm::vec3 _ka, glm::vec3 _km, float _n, glm::vec3 _norm, glm::vec3 _center);
	bool intersect(glm::vec3 Rayorigin, glm::vec3 Raydirection, double t0, double t1, double& t);
	Shape* intersect(glm::vec3 Rayorigin, glm::vec3 Raydirection, double t0, double t1, double& t, bool& Hit);
	glm::vec3 GetNormal(glm::vec3& Point);

private:
	glm::vec3 center;
	glm::vec3 normal;

};
