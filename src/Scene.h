#pragma once
#include <vector>
#include "Shape.h"
#include "Light.h"


class Scene
{
public:
	Scene();
	~Scene();

	Scene(std::vector<Shape*>& _shapes, std::vector<Light*>& _lights);
	void addShape(Shape* s);
	void addLight(Light* l);

	int shapes_size() { return shapes.size(); }
	int lights_size() { return lights.size(); }

	Shape* shapes_at(int i) { return shapes[i]; }
	Light* lights_at(int i) { return lights[i]; }

	bool Hit(glm::vec3 rayOrigin, glm::vec3 rayDirection, double t0, double t1, int& rec, double& t); //rec is the index of the closest shape that hit, t is the point on the ray

	bool Scene::Hit(glm::vec3 rayOrigin, glm::vec3 rayDirection, double t0, double t1, Shape*& rec, double& t);

private:
	std::vector<Shape*> shapes;
	std::vector<Light*> lights;

};
