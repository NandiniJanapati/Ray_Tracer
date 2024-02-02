#pragma once
#include <glm/glm.hpp>

class Shape
{
public:
	Shape(void);
	~Shape(void);

	Shape(glm::vec3 _kd, glm::vec3 _ks, glm::vec3 _ka, glm::vec3 _km, float _n) : kd(_kd), ks(_ks), ka(_ka), km(_km), n(_n) {}

	virtual bool intersect(glm::vec3 O, glm::vec3 d, double t0, double t1, double& t) { return 0; }
	virtual Shape* intersect(glm::vec3 O, glm::vec3 d, double t0, double t1, double& t, bool& Hit) { return 0; }
	virtual glm::vec3 GetNormal(glm::vec3& Point) { return { 0, 0, 0 }; }

	glm::vec3 Getkd() { return kd; }
	glm::vec3 Getks() { return ks; }
	glm::vec3 Getka() { return ka; }
	glm::vec3 Getkm() { return km; }
	float Getn() { return n; }

protected:
	glm::vec3 kd;
	glm::vec3 ks;
	glm::vec3 ka;
	glm::vec3 km;
	float n;
	
};
