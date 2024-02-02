#pragma once

#include "Shape.h"


class Triangle :
	public Shape
{
public:
	Triangle();
	~Triangle();

	Triangle(glm::vec3 _kd, glm::vec3 _ks, glm::vec3 _ka, glm::vec3 _km, float _n, glm::vec3 _norm0, glm::vec3 _norm1, glm::vec3 _norm2, glm::vec3 _v0, glm::vec3 _v1, glm::vec3 _v2);
	bool intersect(glm::vec3 Rayorigin, glm::vec3 Raydirection, double t0, double t1, double& t);
	Shape* intersect(glm::vec3 Rayorigin, glm::vec3 Raydirection, double t0, double t1, double& t, bool& Hit);
	glm::vec3 GetNormal(glm::vec3& Point);

	glm::vec3 GetV0() { return v0; }
	glm::vec3 GetV1() { return v1; }
	glm::vec3 GetV2() { return v2; }

	float avgX() {
		return ((v0.x + v1.x + v2.x) / 3);
	}
	float avgY() {
		return ((v0.y + v1.y + v2.y) / 3);
	}
	float avgZ() {
		return ((v0.z + v1.z + v2.z) / 3);
	}
	float minX();
	float minY();
	float minZ();

	float maxX();
	float maxY();
	float maxZ();


private:
	glm::vec3 v0;
	glm::vec3 v1;
	glm::vec3 v2;
	glm::vec3 norm0;
	glm::vec3 norm1;
	glm::vec3 norm2;



};
