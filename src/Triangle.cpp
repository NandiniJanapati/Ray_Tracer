#include "Triangle.h"


Triangle::Triangle() : Shape(), v0(), v1(), v2(), norm0(), norm1(), norm2()
{
}


Triangle::~Triangle()
{
}

Triangle::Triangle(glm::vec3 _kd, glm::vec3 _ks, glm::vec3 _ka, glm::vec3 _km, float _n, glm::vec3 _norm0, glm::vec3 _norm1, glm::vec3 _norm2, glm::vec3 _v0, glm::vec3 _v1, glm::vec3 _v2)
	: Shape(_kd, _ks, _ka, _km, _n),
	norm0(_norm0), 
	norm1(_norm1), 
	norm2(_norm2),
	v0(_v0), 
	v1(_v1), 
	v2(_v2)
{}

bool Triangle::intersect(glm::vec3 Rayorigin, glm::vec3 Raydirection, double t0, double t1, double& t) {
	glm::vec3 E1 = v1 - v0;
	glm::vec3 E2 = v2 - v0;
	glm::vec3 S = Rayorigin - v0;
	glm::vec3 S1 = glm::cross(Raydirection, E2); //h on wikipedia
	glm::vec3 S2 = glm::cross(S, E1); //q

	float denominator = glm::dot(S1, E1); //a
	if (denominator > -0.0001f && denominator < 0.0001f) {
		return false;
	}
	denominator = 1.0f / denominator;
	float b1 = denominator * glm::dot(S1, S); //u
	if (b1 < 0 || b1 > 1) {
		return false;
	}
	float b2 = denominator * glm::dot(S2, Raydirection); //v
	if (b2 < 0 || b2 > 1) {
		return false;
	}
	float temp = denominator * glm::dot(S2, E2);
	if ((temp >= t0) && (temp <= t1)) {
		if (temp >= 0.0001f) {
			t = temp;
			return true;
		}
	}
	else {
		return false;
	}
}

Shape* Triangle::intersect(glm::vec3 Rayorigin, glm::vec3 Raydirection, double t0, double t1, double& t, bool& Hit) {
	glm::vec3 E1 = v1 - v0;
	glm::vec3 E2 = v2 - v0;
	glm::vec3 S = Rayorigin - v0;
	glm::vec3 S1 = glm::cross(Raydirection, E2); //h on wikipedia
	glm::vec3 S2 = glm::cross(S, E1); //q

	float denominator = glm::dot(S1, E1); //a
	if (denominator > -0.0001f && denominator < 0.0001f) { //denominator is close to 0
		Hit = false;
		return nullptr;
	}
	float fraction = 1.0f / denominator;//denominator is not the denominator
	float b1 = fraction * glm::dot(S1, S); //u
	if (b1 < 0 || b1 > 1) {
		Hit = false;
		return nullptr;
	}
	float b2 = fraction * glm::dot(S2, Raydirection); //v
	if (b2 < 0 || b2 > 1) {
		Hit = false;
		return nullptr;
	}
	float b3 = 1 - b1 - b2;
	if (b3 < 0 || b3 > 1) {
		Hit = false;
		return nullptr;
	}
	float temp = fraction * glm::dot(S2, E2);
	if ((temp >= t0) && (temp <= t1)) {
		if (temp >= 0.0001f) {
			t = temp;
			Hit = true;
			return this;
		}
	}
	else {
		Hit = false;
		return nullptr;
	}
}

glm::vec3 Triangle::GetNormal(glm::vec3& Point) {
	float xA = v0.x; float yA = v0.y;
	float xB = v1.x; float yB = v1.y;
	float xC = v2.x; float yC = v2.y;

	float x = Point.x;
	float y = Point.y;

	float alpha = (-1 * (x - xB) * (yC - yB) + (y - yB) * (xC - xB)) / (-1 * (xA - xB) * (yC - yB) + (yA - yB) * (xC - xB));
	float beta = (-1 * (x - xC) * (yA - yC) + (y - yC) * (xA - xC)) / (-1 * (xB - xC) * (yA - yC) + (yB - yC) * (xA - xC));
	float gamma = 1 - alpha - beta;

	glm::vec3 normal;
	normal.x = alpha * norm0.x + beta * norm1.x + gamma * norm2.x;
	normal.y = alpha * norm0.y + beta * norm1.y + gamma * norm2.y;
	normal.z = alpha * norm0.z + beta * norm1.z + gamma * norm2.z;
	return normal;
}

float minimum(float a, float b, float c) {
	float t = (a < b) ? a : b;
	return (t < c) ? t : c;
}
float maximum(float a, float b, float c) {
	float t = (a > b) ? a : b;
	return (t > c) ? t : c;
}
float Triangle::minX() {
	return minimum(v0.x, v1.x, v2.x);
}
float Triangle::minY() {
	return minimum(v0.y, v1.y, v2.y);
}
float Triangle::minZ() {
	return minimum(v0.z, v1.z, v2.z);
}
float Triangle::maxX() {
	return maximum(v0.x, v1.x, v2.x);
}
float Triangle::maxY() {
	return maximum(v0.y, v1.y, v2.y);
}
float Triangle::maxZ() {
	return maximum(v0.z, v1.z, v2.z);
}
