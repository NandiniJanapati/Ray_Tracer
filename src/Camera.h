#pragma once
#include <vector>
#include "Scene.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>


class Camera
{
public:
	Camera();
	~Camera();

	Camera(int widthRes, int heightRes);

	Camera(int widthRes, int heightRes, glm::vec3 _eye, glm::vec3 _center, glm::vec3 _up, float _fovy, float focalD, Scene* scene);
	void ComputePrimaryRay(glm::vec3& rayOrigin, glm::vec3& rayDirection, float i, float j);
	void ComputeShadowRay(glm::vec3& rayOrigin, glm::vec3& rayDirection, double& tlight, int lightIndex, glm::vec3 Point);
	glm::vec3 ComputeRayColor(glm::vec3 rayOrigin, glm::vec3 rayDirection, double t0, double t1, int counter);
	glm::vec3 Camera::SpecularDiffuse(glm::vec3 Pos, int shape_index, int light_index);
	glm::vec3 Camera::SpecularDiffuse(glm::vec3 Pos, Shape* rec, int light_index);

	void TakePicture(Scene *scene);
	float* GetRenderedImage() { return renderedImage; };

private:

	int widthRes;
	int heightRes;

	glm::vec3 eye;
	glm::vec3 center; //lookat
	glm::vec3 up;
	float fovy;
	float focalDist;

	Scene* scene; //the scene the camera is located in

	glm::vec3 Ic;
	float ly;
	float lx;
	float Pw;

	glm::vec3 w;
	glm::vec3 u;
	glm::vec3 v;
	glm::vec3 O;



	float *renderedImage;


};
