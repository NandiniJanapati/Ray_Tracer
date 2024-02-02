#include "Scene.h"

Scene::~Scene()
{
	for (int j = 0; j < shapes.size(); j++) {
		delete shapes[j];
	}
	for (int g = 0; g < lights.size(); g++) {
		delete lights[g];
	}
	
}

Scene::Scene()
{
	
}

Scene::Scene(std::vector<Shape*>& _shapes, std::vector<Light*>& _lights) {

	for (int j = 0; j < _shapes.size(); j++) {
		shapes.push_back(_shapes[j]);
	}
	for (int g = 0; g < _lights.size(); g++) {
		lights.push_back(_lights[g]);
	}

}

void Scene::addShape(Shape* s) {
	shapes.push_back(s);
}

void Scene::addLight(Light* l) {
	lights.push_back(l);
}


bool Scene::Hit(glm::vec3 rayOrigin, glm::vec3 rayDirection, double t0, double t1, int& rec, double& t) {
	double min_t = INT_MAX;// closest intersection point found it scene (im very sad that I can't call this variable minty. i would probably get confused later if i did)
	bool wasHit = false;
	for (int i = 0; i < shapes.size(); i++) {
		double shape_t = 0; //t value found for this shape
		bool isHit = shapes[i]->intersect(rayOrigin, rayDirection, t0, t1, shape_t);
		if (isHit) { //there was an intersection in the scene found
			if ((shape_t >= t0) && (shape_t <= t1)) { //check if intersection is between ray start and end
				if (shape_t < min_t) { //check if intersection is closer than closest found so far
					wasHit = true;
					min_t = shape_t;
					rec = i;
				}
			}
		}
	}
	t = min_t;
	return wasHit;
}




bool Scene::Hit(glm::vec3 rayOrigin, glm::vec3 rayDirection, double t0, double t1, Shape*& rec, double& t) {
	double min_t = INT_MAX;// closest intersection point found it scene (im very sad that I can't call this variable minty. i would probably get confused later if i did)
	bool wasHit = false;
	for (int i = 0; i < shapes.size(); i++) {
		double shape_t = 0; //t value found for this shape
		Shape* shapes_rec = nullptr;
		bool isHit;
		shapes_rec = shapes[i]->intersect(rayOrigin, rayDirection, t0, t1, shape_t, isHit);
		if (isHit) { //there was an intersection in the scene found
			if ((shape_t >= t0) && (shape_t <= t1)) { //check if intersection is between ray start and end
				if (shape_t < min_t) { //check if intersection is closer than closest found so far
					wasHit = true;
					min_t = shape_t;
					//*rec = shapes[i];
					rec = shapes_rec;
				}
			}
		}
	}
	t = min_t;
	return wasHit;
}
