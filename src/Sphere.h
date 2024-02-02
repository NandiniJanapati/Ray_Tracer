#pragma once
#include "Shape.h"


class Sphere : 
	public Shape
{
public:
	Sphere();
	~Sphere();

	Sphere(glm::vec3 _kd, glm::vec3 _ks, glm::vec3 _ka, glm::vec3 _km, float _n, float _radius, glm::vec3 _center);
	bool intersect(glm::vec3 Rayorigin, glm::vec3 Raydirection, double t0, double t1, double& t);
	Shape* intersect(glm::vec3 Rayorigin, glm::vec3 Raydirection, double t0, double t1, double& t, bool& Hit);
	glm::vec3 GetNormal(glm::vec3& Point);



private:
	float radius;
	glm::vec3 center;

};
