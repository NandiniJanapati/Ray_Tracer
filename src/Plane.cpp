#include "Plane.h"


Plane::Plane() : Shape(), center(), normal() 
{
}


Plane::~Plane()
{
}

Plane::Plane(glm::vec3 _kd, glm::vec3 _ks, glm::vec3 _ka, glm::vec3 _km, float _n, glm::vec3 _norm, glm::vec3 _center)
	: Shape(_kd, _ks, _ka, _km, _n),
	normal(_norm),
	center(_center)
{}

bool Plane::intersect(glm::vec3 O, glm::vec3 d, double t0, double t1, double& t) {
	double denominator = glm::dot(d, normal);
	if (!(denominator > -0.0001 && denominator < 0.0001)) {
		double temp = (glm::dot((center - O), normal)) / denominator;
		if ((temp >= t0) && (temp <= t1)) {
			if (temp >= 0) {
				t = temp;
				return true;
			}
		}
		return false;
	}
	return false;
}

Shape* Plane::intersect(glm::vec3 O, glm::vec3 d, double t0, double t1, double& t, bool& Hit) {
	double denominator = glm::dot(d, normal);
	if (!(denominator > -0.0001 && denominator < 0.0001)) {
		double temp = (glm::dot((center - O), normal)) / denominator;
		if ((temp >= t0) && (temp <= t1)) {
			if (temp >= 0) {
				t = temp;
				Hit = true;
				return this;
			}
		}
		Hit = false;
		return nullptr;
	}
	Hit = false;
	return nullptr;
}

glm::vec3 Plane::GetNormal(glm::vec3& Point) {
	return normal;
}

