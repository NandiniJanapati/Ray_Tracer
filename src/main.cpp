#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
//#include <pair>
#include <algorithm>
#include <iterator>
#include <chrono>
#include <iostream>
#include "Camera.h"
#include "Scene.h"
#include "Shape.h"
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"
#include "BVH.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 1200

float frameBuffer[WINDOW_HEIGHT][WINDOW_WIDTH][3];
GLFWwindow *window;

//std::vector<Shape*> SHAPES;
std::vector<Light*> LIGHTS;
std::vector<Shape*> SHAPES0;//scene 1
std::vector<Shape*> SHAPES1;//scene 2 (bunny)

std::vector<glm::vec3> vertices;
std::vector<glm::vec3> normals;
std::vector<Triangle*> triangles; //all the triangles in the bunny

std::vector<std::pair<float, int>> xaxisavg;
std::vector<std::pair<float, int>> yaxisavg;
std::vector<std::pair<float, int>> zaxisavg;

//float bun_xmin = INT_MAX;
//float bun_ymin = INT_MAX;
//float bun_zmin = INT_MAX;
//float bun_xmax = INT_MIN;
//float bun_ymax = INT_MIN;
//float bun_zmax = INT_MIN;

bool bunnyScene = true;

void ClearFrameBuffer()
{
	memset(&frameBuffer, 0, WINDOW_HEIGHT * WINDOW_WIDTH * 3 * sizeof(float));
}

//void SwitchScene() 
//{
//	std::cout << "rendering scene..." << std::endl;
//	
//	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//	ClearFrameBuffer();
//	if (bunnyScene) {
//		Scene scene = Scene(SHAPES1, LIGHTS);
//		glm::vec3 eye = { 0.0, 0.0, 7.0 };
//		glm::vec3 center = { 0.0, 0.0, 0.0 };
//		glm::vec3 up = { 0.0, 1.0, 0.0 };
//		float fovy = 45;
//		float f = 1;
//
//		Camera camera = Camera(WINDOW_WIDTH, WINDOW_HEIGHT, eye, center, up, fovy, f, &scene);
//
//		auto start = std::chrono::high_resolution_clock::now();
//		camera.TakePicture(&scene);
//		auto stop = std::chrono::high_resolution_clock::now();
//		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
//		std::cout << "Time passed (ms): " << duration.count() << std::endl;
//
//		float* renderedImage = camera.GetRenderedImage();
//		memcpy(frameBuffer, renderedImage, sizeof(float) * WINDOW_HEIGHT * WINDOW_WIDTH * 3);
//		
//	}
//	else {
//		Scene scene = Scene(SHAPES0, LIGHTS);
//		glm::vec3 eye = { 0.0, 0.0, 7.0 };
//		glm::vec3 center = { 0.0, 0.0, 0.0 };
//		glm::vec3 up = { 0.0, 1.0, 0.0 };
//		float fovy = 45;
//		float f = 1;
//
//		Camera camera = Camera(WINDOW_WIDTH, WINDOW_HEIGHT, eye, center, up, fovy, f, &scene);
//
//		auto start = std::chrono::high_resolution_clock::now();
//		camera.TakePicture(&scene);
//		auto stop = std::chrono::high_resolution_clock::now();
//		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
//		std::cout << "Time passed (ms): " << duration.count() << std::endl;
//
//		float* renderedImage = camera.GetRenderedImage();
//		memcpy(frameBuffer, renderedImage, sizeof(float) * WINDOW_HEIGHT * WINDOW_WIDTH * 3);
//
//	}
//
//
//}

//void CharacterCallback(GLFWwindow* lWindow, unsigned int key) {
//	switch (key)
//	{
//	case 's':
//		bunnyScene = !bunnyScene;
//		SwitchScene();
//		break;
//	default:
//		std::cout << "press 's' to switch scene." << std::endl;
//		break;
//	}
//}

//BVH* MakeNode(std::vector<Triangle*>& vect, int axis, BVH* parent, float avgAxisForVect) {
//	if (vect.size() <= 3) {
//		BVH* node = new BVH(vect);
//		node->SetLeft(nullptr);
//		node->SetRight(nullptr);
//		node->SetParent(parent);
//		return node;
//	}
//	
//	BVH* node = new BVH(vect);
//	std::vector<Triangle*> leftVect;
//	float sumaxisLeft = 0;
//	std::vector<Triangle*> rightVect;
//	float sumaxisRight = 0;
//	int nextAxis = (axis == 2) ? 0 : (axis+1);
//
//	for (int i = 0; i < vect.size(); i++) {
//		if (axis == 0) {
//			if (vect[i]->avgX() < avgAxisForVect) {
//				leftVect.push_back(vect[i]);
//				sumaxisLeft = sumaxisLeft + vect[i]->GetV0()[1] + vect[i]->GetV1()[1] + vect[i]->GetV2()[1];
//			}
//			else {
//				rightVect.push_back(vect[i]);
//				sumaxisRight = sumaxisRight + vect[i]->GetV0()[1] + vect[i]->GetV1()[1] + vect[i]->GetV2()[1];
//			}
//		}
//		else if (axis == 1) {
//			if (vect[i]->avgY() < avgAxisForVect) {
//				leftVect.push_back(vect[i]);
//				sumaxisLeft = sumaxisLeft + vect[i]->GetV0()[2] + vect[i]->GetV1()[2] + vect[i]->GetV2()[2];
//			}
//			else {
//				rightVect.push_back(vect[i]);
//				sumaxisRight = sumaxisRight + vect[i]->GetV0()[2] + vect[i]->GetV1()[2] + vect[i]->GetV2()[2];
//			}
//		}
//		else {
//			if (vect[i]->avgZ() < avgAxisForVect) {
//				leftVect.push_back(vect[i]);
//				sumaxisLeft = sumaxisLeft + vect[i]->GetV0()[0] + vect[i]->GetV1()[0] + vect[i]->GetV2()[0];
//			}
//			else {
//				rightVect.push_back(vect[i]);
//				sumaxisRight = sumaxisRight + vect[i]->GetV0()[0] + vect[i]->GetV1()[0] + vect[i]->GetV2()[0];
//			}
//		}
//	}
//	node->SetParent(parent);
//	float avgAxisforLeft = sumaxisLeft / (leftVect.size() * 3);
//	BVH* leftnode = MakeNode(leftVect, nextAxis, node, avgAxisforLeft);
//	float avgAxisforRight = sumaxisRight / (rightVect.size() * 3);
//	BVH* rightnode = MakeNode(rightVect, nextAxis, node, avgAxisforRight);
//	node->SetLeft(leftnode);
//	node->SetRight(rightnode);
//	return node;
//	
//}

BVH* MakeNode(std::vector<int>& vect, int axis, BVH* parent) { //vect indices for all triangles that will be in this BBox
	std::vector<Triangle*> t;
	for (int i = 0; i < vect.size(); i++) {
		t.push_back(triangles[vect[i]]);
	}
	BVH* node = new BVH(t);
	node->SetParent(parent);
	
	if (vect.size() <= 3) {
		//BVH* node = new BVH(t);
		node->SetLeft(nullptr);
		node->SetRight(nullptr);
		//node->SetParent(parent);
		return node;
	}
	
	std::vector<std::pair<float, int>> avgvalEachTri; //avg value from each triangle based on the axis given.
	for (int i = 0; i < vect.size(); i++) {
		Triangle* temp = triangles[vect[i]];
		if (axis == 0) {
			avgvalEachTri.push_back(std::make_pair(temp->avgX(), vect[i]));
		}
		else if(axis == 1) {
			avgvalEachTri.push_back(std::make_pair(temp->avgY(), vect[i]));
		}
		else {
			avgvalEachTri.push_back(std::make_pair(temp->avgZ(), vect[i]));
		}
	}

	//sort the avgvalEachTri vector to ascending order
	std::sort(std::begin(avgvalEachTri), std::end(avgvalEachTri),
		[&](std::pair<float, int>& a, std::pair<float, int>& b)
		{
			return a.first < b.first;
		});
	
	int nextAxis = (axis == 2) ? 0 : (axis + 1);

	std::vector<int> leftSideTriangles;
	std::vector<int> rightSideTriangles;

	int middle = avgvalEachTri.size() / 2;

	for (int i = 0; i < middle; i++) {
		leftSideTriangles.push_back(avgvalEachTri[i].second);
	}
	for (int i = middle; i < avgvalEachTri.size(); i++) {
		rightSideTriangles.push_back(avgvalEachTri[i].second);
	}

	BVH* leftnode = MakeNode(leftSideTriangles, nextAxis, node);
	BVH* rightnode = MakeNode(rightSideTriangles, nextAxis, node);
	node->SetLeft(leftnode);
	node->SetRight(rightnode);
	return node;
}

void Display()
{	
	glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_FLOAT, frameBuffer);
}

void LoadModel(char* name)
{
	// Taken from Shinjiro Sueda with slight modification
	std::string meshName(name);
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string errStr;
	bool rc = tinyobj::LoadObj(&attrib, &shapes, &materials, &errStr, meshName.c_str());
	if (!rc) {
		std::cerr << errStr << std::endl;
	}
	else {
		// Some OBJ files have different indices for vertex positions, normals,
		// and texture coordinates. For example, a cube corner vertex may have
		// three different normals. Here, we are going to duplicate all such
		// vertices.
		// Loop over shapes
		for (size_t s = 0; s < shapes.size(); s++) {
			// Loop over faces (polygons)
			size_t index_offset = 0;
			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
				size_t fv = shapes[s].mesh.num_face_vertices[f];
				// Loop over vertices in the face.
				for (size_t v = 0; v < fv; v++) {
					// access to vertex
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
					vertices.push_back(glm::vec3(attrib.vertices[3 * idx.vertex_index + 0],
						attrib.vertices[3 * idx.vertex_index + 1],
						attrib.vertices[3 * idx.vertex_index + 2]));
					if (!attrib.normals.empty()) {
					normals.push_back(glm::vec3(attrib.normals[3 * idx.normal_index + 0],
						attrib.normals[3 * idx.vertex_index + 1],
						attrib.normals[3 * idx.vertex_index + 2]));
					}
				}
				index_offset += fv;
				// per-face material (IGNORE)
				shapes[s].mesh.material_ids[f];
			}
		}
	}
}

void Init()
{
	
	glfwInit();
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Assignment5 - <Nandini Janapati>", NULL, NULL);
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();

	//glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	//glfwSetCharCallback(window, CharacterCallback);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	ClearFrameBuffer();
	
	Scene scene = Scene(SHAPES0, LIGHTS); //change to SHAPES1 for the bunny scene
	glm::vec3 eye = { 0.0, 0.0, 7.0 };
	glm::vec3 center = { 0.0, 0.0, 0.0 };
	glm::vec3 up = { 0.0, 1.0, 0.0 };
	float fovy = 45;
	float f = 1;

	Camera camera = Camera(WINDOW_WIDTH, WINDOW_HEIGHT, eye, center, up, fovy, f, &scene);
	
	auto start = std::chrono::high_resolution_clock::now();
	camera.TakePicture(&scene);
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
	std::cout << "Time passed (ms): " << duration.count() << std::endl;
	
	float *renderedImage = camera.GetRenderedImage();
	memcpy(frameBuffer, renderedImage, sizeof(float) * WINDOW_HEIGHT * WINDOW_WIDTH * 3);
	//SwitchScene();
}


int main()
{
	//make lights
	glm::vec3 pos = { 0.0, 3.0, -2.0 };
	glm::vec3 color = { 0.2, 0.2, 0.2 };
	Light* l1 = new Light(pos, color);
	LIGHTS.push_back(l1);

	pos = { -2.0, 1.0, 4.0 };
	color = { 0.5, 0.5, 0.5 };
	Light* l2 = new Light(pos, color);
	LIGHTS.push_back(l2);


	glm::vec3 ka(0.1, 0.1, 0.1);
	glm::vec3 kd(0.2, 1.0, 0.2);
	glm::vec3 ks(1.0, 1.0, 1.0);
	glm::vec3 km(0.0, 0.0, 0.0);
	float n = 100;
	pos = { -1.0, -0.7, 3.0 };
	float radius = 0.3;
	Sphere* s1 = new Sphere(kd, ks, ka, km, n, radius, pos);
	SHAPES0.push_back(s1);

	ka = { 0.1, 0.1, 0.1 };
	kd = { 0.0, 0.0, 1.0 };
	ks = { 1.0, 1.0, 1.0 };
	km = { 0.0, 0.0, 0.0 };
	n = 10;
	pos = { 1.0, -0.5, 3.0 };
	radius = 0.5;
	Sphere* s2 = new Sphere(kd, ks, ka, km, n, radius, pos);
	SHAPES0.push_back(s2);

	ka = { 0.0, 0.0, 0.0 };
	kd = { 0.0, 0.0, 0.0 };
	ks = { 0.0, 0.0, 0.0 };
	km = { 1.0, 1.0, 1.0 };
	n = 0.0;
	pos = { -1.0, 0.0, -0.0 };
	radius = 1.0;
	Sphere* s3 = new Sphere(kd, ks, ka, km, n, radius, pos);
	SHAPES0.push_back(s3);

	km = { 0.8, 0.8, 0.8 };
	pos = { 1.0, 0.0, -1.0 };
	radius = 1.0;
	Sphere* s4 = new Sphere(kd, ks, ka, km, n, radius, pos);
	SHAPES0.push_back(s4);
	
	pos = { 0.0, -1.0, 0.0 };
	glm::vec3 normal(0.0, 1.0, 0.0);
	ka = { 0.1, 0.1, 0.1 };
	kd = { 1.0, 1.0, 1.0 };
	ks = { 0.0, 0.0, 0.0 };
	km = { 0.0, 0.0, 0.0 };
	n = 0;
	Plane* p1 = new Plane(kd, ks, ka, km, n, normal, pos);
	SHAPES0.push_back(p1);

	pos = { 0.0, 0.0, -3.0 };
	normal = { 0.0, 0.0, 1.0 };
	Plane* p2 = new Plane(kd, ks, ka, km, n, normal, pos);
	SHAPES0.push_back(p2);

	LoadModel("../obj/bunny.obj");

	ka = { 0.1, 0.1, 0.1 };
	kd = { 0.0, 0.0, 1.0 };
	ks = { 1.0, 1.0, 0.5 };
	km = { 0.0, 0.0, 0.0 };
	n = 100;
	
	std::vector<int> rootTriangleIndices;
	int index = 0;
	for (int i = 0; i < vertices.size() / 3; i++) {
		Triangle* t = new Triangle(kd, ks, ka, km, n, 
									normals[i * 3 + 0], normals[i * 3 + 1], normals[i * 3 + 2],
									vertices[i * 3 + 0], vertices[i * 3 + 1], vertices[i * 3 + 2]);
		triangles.push_back(t);
		rootTriangleIndices.push_back(i);
		//if (vertices[i * 3 + 2][2] > bun_zmax) {
		//	bun_zmax = vertices[i * 3 + 2][2];
		//}
		//if (vertices[i * 3 + 2][2] < bun_zmin) {
		//	bun_zmin = vertices[i * 3 + 2][2];
		//}
		//if (vertices[i * 3 + 2][1] > bun_ymax) {
		//	bun_ymax = vertices[i * 3 + 2][1];
		//}
		//if (vertices[i * 3 + 2][1] < bun_ymin) {
		//	bun_ymin = vertices[i * 3 + 2][1];
		//}
		//if (vertices[i * 3 + 2][0] > bun_xmax) {
		//	bun_xmax = vertices[i * 3 + 2][0];
		//}
		//if (vertices[i * 3 + 2][0] < bun_xmin) {
		//	bun_xmin = vertices[i * 3 + 2][0];
		//}
		
	}
	/*for (int i = 0; i < tri_vector.size(); i++) {
		xaxisavg.push_back(std::make_pair(tri_vector[i]->avgX(), i));
		yaxisavg.push_back(std::make_pair(tri_vector[i]->avgY(), i));
		zaxisavg.push_back(std::make_pair(tri_vector[i]->avgZ(), i));
	}

	std::sort(std::begin(xaxisavg), std::end(xaxisavg),
		[&](std::pair<float, int>& a, std::pair<float, int>& b)
		{
			return a.first < b.first;
		});

	std::sort(std::begin(yaxisavg), std::end(yaxisavg),
		[&](std::pair<float, int>& a, std::pair<float, int>& b)
		{
			return a.first < b.first;
		});

	std::sort(std::begin(zaxisavg), std::end(zaxisavg),
		[&](std::pair<float, int>& a, std::pair<float, int>& b)
		{
			return a.first < b.first;
		});*/

	//BVH* boundingvolume = new BVH(kd, ks, ka, km, n, triangles, bun_xmin, bun_ymin, bun_zmin, bun_xmax, bun_ymax, bun_zmax);
	BVH* root = MakeNode(rootTriangleIndices, 0, nullptr);
	SHAPES1.push_back(root);
	

	Init();
	while ( glfwWindowShouldClose(window) == 0) 
	{
		glClear(GL_COLOR_BUFFER_BIT);
		Display();
		glFlush();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//for (int i = 0; i < triangles.size(); i++) {
	//	delete triangles[i];
	//}
	glfwTerminate();

	return 0;
}