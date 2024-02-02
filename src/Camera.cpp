#include "Camera.h"



Camera::Camera()
{
}

Camera::~Camera()
{
}

Camera::Camera(int widthRes, int heightRes)
{
	this->widthRes = widthRes;
	this->heightRes = heightRes;
	renderedImage = new float[widthRes * heightRes * 3];
}

Camera::Camera(int widthRes, int heightRes, glm::vec3 _eye, glm::vec3 _center, glm::vec3 _up, float _fovy, float focalD, Scene* _scene) : 
	eye(_eye), 
	center(_center),
	up(_up),
	fovy(_fovy),
	focalDist(focalD)
{
	scene = _scene;
	this->widthRes = widthRes;
	this->heightRes = heightRes;
	renderedImage = new float[widthRes * heightRes * 3];


	Ic = eye + (glm::normalize(center - eye) * focalDist);//right now, everything is in world space,,,

	ly = 2 * focalDist * tan(fovy / 2);
	float temp = (float)widthRes / (float)heightRes;
	lx = ly * temp;
	Pw = ly / (float)heightRes;

	w = glm::normalize(eye-center);
	u = glm::normalize(glm::cross(up, w));
	v = glm::cross(w, u);

	O = Ic - (lx / 2) * u - (ly / 2) * v;
}

void Camera::ComputePrimaryRay(glm::vec3& rayOrigin, glm::vec3& rayDirection, float i, float j) {
	rayOrigin = eye;
	/*
	 glm::vec3 Ic = eye + (glm::normalize(center - eye) * focalDist);//right now, everything is in world space,,,

	float ly = 2 * focalDist * tan(fovy / 2);
	float temp = (float) widthRes / (float) heightRes;
	float lx = ly * temp;
	float Pw = ly / (float) heightRes;

	glm::vec3 b = up;
	glm::vec3 a = eye - center;
	glm::vec3 w = glm::normalize(a);
	glm::vec3 u = glm::normalize(glm::cross(b, w));
	glm::vec3 v = glm::cross(w, u);

	glm::vec3 O = Ic - (lx / 2) * u - (ly / 2) * v;
	
	*/
	
	
	i += 0.5;
	j += 0.5;
	glm::vec3 Pc = O + (Pw * i) * u + (Pw * j) * v; //primary ray as a vector (but it's in world space)
	rayDirection = glm::normalize(Pc - eye); //Pc gives us the direction of the ray. Camera's position in space is the ray origin.

	//glm::mat4 view = glm::lookAt(eye, center, up);
}

void Camera::ComputeShadowRay(glm::vec3& rayOrigin, glm::vec3& rayDirection, double& tlight, int lightIndex, glm::vec3 Point) {
	rayOrigin = Point;
	glm::vec3 lightPosition = scene->lights_at(lightIndex)->GetPosition();
	rayDirection = glm::normalize(lightPosition - Point);
	tlight = (lightPosition.x - Point.x) / rayDirection.x;
}


float max(float a, float b) {
	return (a > b) ? a : b;
}

glm::vec3 Camera::SpecularDiffuse(glm::vec3 Pos, int shape_index, int light_index) {
	Light* light = scene->lights_at(light_index);
	Shape* shape = scene->shapes_at(shape_index);
	
	glm::vec3 L = glm::normalize(light->GetPosition() - Pos);
	glm::vec3 E = glm::normalize(eye - Pos);
	glm::vec3 N = shape->GetNormal(Pos);
	glm::vec3 R = 2 * (dot(L, N)) * N - L;
	R = glm::normalize(R);
	return light->GetColor() * (shape->Getkd() * max(0, glm::dot(L, N)) + shape->Getks() * pow(max(0, glm::dot(R, E)), shape->Getn()));
	//lights[i].color * (kd * max(0, dot(L, N)) + ks * pow(max(0, dot(R,E)), s));
}

glm::vec3 Camera::SpecularDiffuse(glm::vec3 Pos, Shape* rec, int light_index) {
	Light* light = scene->lights_at(light_index);
	//Shape* shape = scene->shapes_at(shape_index);

	glm::vec3 L = glm::normalize(light->GetPosition() - Pos);
	glm::vec3 E = glm::normalize(eye - Pos);
	glm::vec3 N = rec->GetNormal(Pos);
	glm::vec3 R = 2 * (dot(L, N)) * N - L;
	R = glm::normalize(R);
	return light->GetColor() * (rec->Getkd() * max(0, glm::dot(L, N)) + rec->Getks() * pow(max(0, glm::dot(R, E)), rec->Getn()));
	//lights[i].color * (kd * max(0, dot(L, N)) + ks * pow(max(0, dot(R,E)), s));
}

//glm::vec3 Camera::ComputeRayColor(glm::vec3 rayOrigin, glm::vec3 rayDirection, double t0, double t1, int counter) {
//	int rec = -1; //index of the shape that was intersected TODO:reinstate
//	double t; //point on the primary ray
//	//Shape* rec;
//	if (scene->Hit(rayOrigin, rayDirection, t0, t1, rec, t)) {
//		glm::vec3 color = scene->shapes_at(rec)->Getka(); //TODO:reinstate
//		//glm::vec3 color = rec->Getka();
//		glm::vec3 intersectPos = rayOrigin + (float) t * rayDirection; //the (x,y,z) point in the scene we are finding the color of
//
//		
//		for (int j = 0; j < scene->lights_size(); j++) { //for each light
//			glm::vec3 shadowRayOrigin, shadowRayDir;
//			double tlight;
//			ComputeShadowRay(shadowRayOrigin, shadowRayDir, tlight, j, intersectPos);
//			int srec = -1; //index of the shape that the shadow ray hit (not useful) TODO:reinstate
//			//Shape* srec;
//			double s_t; // position on the the shadow ray that was hit (not useful)
//			bool inShadow = scene->Hit(shadowRayOrigin, shadowRayDir, 0.001, tlight, srec, s_t);
//			if (!inShadow) {
//				color = color + SpecularDiffuse(intersectPos, rec, j);
//			}
//		}
//		glm::vec3 zero = { 0, 0, 0 };
//		if ((scene->shapes_at(rec)->Getkm() != zero) && (counter < 5)) {
//
//			glm::vec3 V = glm::normalize(rayOrigin - intersectPos);
//			glm::vec3 N = scene->shapes_at(rec)->GetNormal(intersectPos);
//			glm::vec3 R = glm::normalize(2 * (dot(V, N)) * N - V);
//			//note: reflectedRayOrigin = intersectPos;
//			//note: reflectedRayDirection = R;
//			color = color + scene->shapes_at(rec)->Getkm() * ComputeRayColor(intersectPos, R, 0.0001, INT_MAX, counter + 1);
//		}
//		return color;
//	}
//	else {
//		glm::vec3 black(0, 0, 0);
//		return black;
//	}
//
//}


glm::vec3 Camera::ComputeRayColor(glm::vec3 rayOrigin, glm::vec3 rayDirection, double t0, double t1, int counter) {
	
	double t; //point on the primary ray
	Shape* rec;
	if (scene->Hit(rayOrigin, rayDirection, t0, t1, rec, t)) {
		glm::vec3 color = rec->Getka();
		glm::vec3 intersectPos = rayOrigin + (float)t * rayDirection; //the (x,y,z) point in the scene we are finding the color of


		for (int j = 0; j < scene->lights_size(); j++) { //for each light
			glm::vec3 shadowRayOrigin, shadowRayDir;
			double tlight;
			ComputeShadowRay(shadowRayOrigin, shadowRayDir, tlight, j, intersectPos);
			
			Shape* srec;
			double s_t; // position on the the shadow ray that was hit (not useful)
			bool inShadow = scene->Hit(shadowRayOrigin, shadowRayDir, 0.001, tlight, srec, s_t);
			if (!inShadow) {
				color = color + SpecularDiffuse(intersectPos, rec, j);
			}
		}
		glm::vec3 zero = { 0, 0, 0 };
		if ((rec->Getkm() != zero) && (counter < 5)) {

			glm::vec3 V = glm::normalize(rayOrigin - intersectPos);
			glm::vec3 N = rec->GetNormal(intersectPos);
			glm::vec3 R = glm::normalize(2 * (dot(V, N)) * N - V);
			//note: reflectedRayOrigin = intersectPos;
			//note: reflectedRayDirection = R;
			color = color + rec->Getkm() * ComputeRayColor(intersectPos, R, 0.0001, INT_MAX, counter + 1);
		}
		return color;
	}
	else {
		glm::vec3 black(0, 0, 0);
		return black;
	}

}

void Camera::TakePicture(Scene *scene)
{
	memset(renderedImage, 0, sizeof(float) * widthRes * heightRes * 3);
	for (int px = 0; px < widthRes; px++) { //pixels in x
		for (int py = 0; py < heightRes; py++) { //pixels in y
			glm::vec3 O; // camera's eye
			glm::vec3 d;
			ComputePrimaryRay(O, d, px, py);
			glm::vec3 color = ComputeRayColor(O, d, 0, INT_MAX, 0);
			

			//for each object in scene
			//check for intersections and store the cloest one to the camera
			//do phong shading on the cloest point
			renderedImage[widthRes * py * 3 + px * 3 + 0] = color.r;
			renderedImage[widthRes * py * 3 + px * 3 + 1] = color.g;
			renderedImage[widthRes * py * 3 + px * 3 + 2] = color.b;
		}
	}
}
