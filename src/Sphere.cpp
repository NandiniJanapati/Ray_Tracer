#include "Sphere.h"

Sphere::Sphere() : Shape(), radius(), center()
{
}

Sphere::~Sphere()
{
}

Sphere::Sphere(glm::vec3 _kd, glm::vec3 _ks, glm::vec3 _ka, glm::vec3 _km, float _n, float _radius, glm::vec3 _center)
	: Shape(_kd, _ks, _ka, _km, _n),
	radius(_radius), 
	center(_center) 
{}

double min(double a, double b) {
	return (a < b) ? a : b;
}

bool Sphere::intersect(glm::vec3 O, glm::vec3 d, double t0, double t1, double& t) {
	//d => Ray's direction
	//O => Ray's origin
	double aq = glm::dot(d, d);
	double bq = 2 * glm::dot(d, O - center);
	double cq = glm::dot(O - center, O - center) - (radius * radius);

	double solnCheck = (bq * bq) - 4 * aq * cq;
	if (solnCheck < 0) {
		return false; //no solutions
	}
	else if (solnCheck == 0) { //one solution
		double temp = (-bq + sqrt(solnCheck)) / (2 * aq);
		if ((temp >= t0) && (temp <= t1)) {
			if (temp >= 0.0001) {
				t = temp;
				return true;
			}
		}
		return false; //behind camera or not within time frame
		
	}
	else{  //two solutions
		double t1 = (-bq + sqrt(solnCheck)) / (2 * aq);
		double t2 = (-bq - sqrt(solnCheck)) / (2 * aq);
		double temp = min(t1, t2);
		if ((temp >= t0) && (temp <= t1)) {
			if (temp >= 0.0001) {
				t = temp;
				return true;
			}
		}
		else if (t1 < 0.0001 && t2 < 0.0001) {
			return false;
		}
		else {
			temp = (t1 < 0.0001) ? t2 : t1; //using epsilons instead of 0
			if ((temp >= t0) && (temp <= t1)) {
				t = temp;
				return true;
			}
			return false;
		}
	}
}

Shape* Sphere::intersect(glm::vec3 O, glm::vec3 d, double t0, double t1, double& t, bool& Hit) {
	//d => Ray's direction
	//O => Ray's origin
	double aq = glm::dot(d, d);
	double bq = 2 * glm::dot(d, O - center);
	double cq = glm::dot(O - center, O - center) - (radius * radius);

	double solnCheck = (bq * bq) - 4 * aq * cq;
	if (solnCheck < 0) {
		Hit = false; //no solutions
		return nullptr;

	}
	else if (solnCheck == 0) { //one solution
		double temp = (-bq + sqrt(solnCheck)) / (2 * aq);
		if ((temp >= t0) && (temp <= t1)) {
			if (temp >= 0.0001) {
				t = temp;
				Hit = true;
				return this;
			}
		}
		Hit = false; //behind camera or not within time frame
		return nullptr;
		

	}
	else {  //two solutions
		double t1 = (-bq + sqrt(solnCheck)) / (2 * aq);
		double t2 = (-bq - sqrt(solnCheck)) / (2 * aq);
		double temp = min(t1, t2);
		if ((temp >= t0) && (temp <= t1)) {
			if (temp >= 0.0001) {
				t = temp;
				Hit = true;
				return this;
			}
		}
		else if (t1 < 0.0001 && t2 < 0.0001) {
			Hit = false; 
			return nullptr;
		}
		else {
			temp = (t1 < 0.0001) ? t2 : t1; //using epsilons instead of 0
			if ((temp >= t0) && (temp <= t1)) {
				t = temp;
				Hit = true;
				return this;
			}
			Hit = false;
			return nullptr;
		}
	}
}

glm::vec3 Sphere::GetNormal(glm::vec3& Point) {
	return glm::normalize(Point - center);
}





