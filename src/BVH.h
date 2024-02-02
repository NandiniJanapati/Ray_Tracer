#pragma once

#include "Shape.h"
#include "Triangle.h"
#include <vector>
//#include "math.h"


class BVH :
	public Shape
{
public:
	BVH();
	~BVH();

	BVH(glm::vec3 _kd, glm::vec3 _ks, glm::vec3 _ka, glm::vec3 _km, float _n, 
		std::vector<Triangle*>& _triangles, float xmin, float ymin, float zmin, float xmax, float ymax, float zmax);
	BVH(glm::vec3 _kd, glm::vec3 _ks, glm::vec3 _ka, glm::vec3 _km, float _n, std::vector<Triangle*>& _triangles);
	BVH(std::vector<Triangle*>& _triangles);

	bool intersect(glm::vec3 Rayorigin, glm::vec3 Raydirection, double t0, double t1, double& t);
	Shape* intersect(glm::vec3 Rayorigin, glm::vec3 Raydirection, double t0, double t1, double& t, bool& Hit);
	glm::vec3 GetNormal(glm::vec3& Point);

	void SetParent(BVH* node) { parent = node; }
	void SetLeft(BVH* node) { left = node; }
	void SetRight(BVH* node) { right = node; }

private:
	float xmin; //planes
	float ymin;
	float zmin;
	float xmax;
	float ymax;
	float zmax;

	std::vector<Triangle*> triangles;

	BVH* parent;
	BVH* left;
	BVH* right;

};
