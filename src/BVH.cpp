#include "BVH.h"

BVH::BVH() : Shape(), xmin(), ymin(), zmin(), xmax(), ymax(), zmax() {
}
BVH::~BVH() {
	/*for (int i = 0; i < triangles.size(); i++) {
		delete triangles[i];
	}*/
	if (left == nullptr && right == nullptr) {
		for (int i = 0; i < triangles.size(); i++) {
			delete triangles[i];
		}
	}
	else {
		delete left;
		delete right;
	}
	
}

BVH::BVH(glm::vec3 _kd, glm::vec3 _ks, glm::vec3 _ka, glm::vec3 _km, float _n, std::vector<Triangle*>& _triangles, float _xmin, float _ymin, float _zmin, float _xmax, float _ymax, float _zmax) :
	Shape(_kd, _ks, _ka, _km, _n),
	xmin(_xmin), 
	ymin(_ymin), 
	zmin(_zmin), 
	xmax(_xmax), 
	ymax(_ymax), 
	zmax(_zmax) 
	{
	right = nullptr;
	left = nullptr;
	parent = nullptr;
	for (int i = 0; i < _triangles.size(); i++) {
		triangles.push_back(_triangles[i]);
	}

}

BVH::BVH(glm::vec3 _kd, glm::vec3 _ks, glm::vec3 _ka, glm::vec3 _km, float _n, std::vector<Triangle*>& _triangles) : Shape(_kd, _ks, _ka, _km, _n)
{
	float bun_xmin = INT_MAX;
	float bun_ymin = INT_MAX;
	float bun_zmin = INT_MAX;
	float bun_xmax = INT_MIN;
	float bun_ymax = INT_MIN;
	float bun_zmax = INT_MIN;
	right = nullptr;
	left = nullptr;
	parent = nullptr;
	for (int i = 0; i < _triangles.size(); i++) {
		triangles.push_back(_triangles[i]);
		if (_triangles[i]->maxZ() > bun_zmax) {
			bun_zmax = _triangles[i]->maxZ();
		}
		if (_triangles[i]->minZ() < bun_zmin) {
			bun_zmin = _triangles[i]->minZ();
		}
		if (_triangles[i]->maxY() > bun_ymax) {
			bun_ymax = _triangles[i]->maxY();
		}
		if (_triangles[i]->minY() < bun_ymin) {
			bun_ymin = _triangles[i]->minY();
		}
		if (_triangles[i]->maxX() > bun_xmax) {
			bun_xmax = _triangles[i]->maxX();
		}
		if (_triangles[i]->minX() < bun_xmin) {
			bun_xmin = _triangles[i]->minX();
		}
	}
	xmin = bun_xmin;
	xmax = bun_xmax;
	ymin = bun_ymin;
	ymax = bun_ymax;
	zmin = bun_zmin;
	zmax = bun_zmax;
}

BVH::BVH(std::vector<Triangle*>& _triangles) {
	float bun_xmin = INT_MAX;
	float bun_ymin = INT_MAX;
	float bun_zmin = INT_MAX;
	float bun_xmax = INT_MIN;
	float bun_ymax = INT_MIN;
	float bun_zmax = INT_MIN;
	right = nullptr;
	left = nullptr;
	parent = nullptr;
	for (int i = 0; i < _triangles.size(); i++) {
		triangles.push_back(_triangles[i]);
		if (_triangles[i]->maxZ() > bun_zmax) {
			bun_zmax = _triangles[i]->maxZ();
		}
		if (_triangles[i]->minZ() < bun_zmin) {
			bun_zmin = _triangles[i]->minZ();
		}
		if (_triangles[i]->maxY() > bun_ymax) {
			bun_ymax = _triangles[i]->maxY();
		}
		if (_triangles[i]->minY() < bun_ymin) {
			bun_ymin = _triangles[i]->minY();
		}
		if (_triangles[i]->maxX() > bun_xmax) {
			bun_xmax = _triangles[i]->maxX();
		}
		if (_triangles[i]->minX() < bun_xmin) {
			bun_xmin = _triangles[i]->minX();
		}
	}
	xmin = bun_xmin;
	xmax = bun_xmax;
	ymin = bun_ymin;
	ymax = bun_ymax;
	zmin = bun_zmin;
	zmax = bun_zmax;
	
	kd = _triangles[0]->Getkd();
	ks = _triangles[0]->Getks();
	ka = _triangles[0]->Getka();
	km = _triangles[0]->Getkm();
	n = _triangles[0]->Getn();

}

bool BVH::intersect(glm::vec3 Rayorigin, glm::vec3 Raydirection, double t0, double t1, double& t) {
	double txmin = INT_MIN;
	double tymin = INT_MIN;
	double tzmin = INT_MIN;
	double txmax = INT_MAX;
	double tymax = INT_MAX;
	double tzmax = INT_MAX;

	if (Raydirection.x != 0) {
		txmax = (xmax - Rayorigin.x) / Raydirection.x;
		txmin =(xmin - Rayorigin.x) / Raydirection.x;
	}
	if (Raydirection.y != 0) {
		tymax = (ymax - Rayorigin.y) / Raydirection.y;
		tymin = (ymin - Rayorigin.y) / Raydirection.y;
	}
	if (Raydirection.z != 0) {
		tzmax = (zmax - Rayorigin.z) / Raydirection.z;
		tzmin = (zmin - Rayorigin.z) / Raydirection.z;
	}

	double ihatethis = std::min(tymax, txmax);
	double tmax = std::min(ihatethis, tzmax);
	ihatethis = std::max(tymin, txmin);
	double tmin = std::max(ihatethis, tzmin );
	
	if (tmax < tmin) {
		return false;
	}
	if (tmax < 0) {
		return false;
	}
	else {

		double minty = INT_MAX;// closest intersection point found it scene
		bool wasHit = false;
		for (int i = 0; i < triangles.size(); i++) {
			double triangles_t = 0; //t value found for this triangle
			bool isHit = triangles[i]->intersect(Rayorigin, Raydirection, t0, t1, triangles_t);
			if (isHit) { //this triangle has a hit
				if ((triangles_t >= t0) && (triangles_t <= t1)) {
					if (triangles_t < minty) {
						wasHit = true;
						minty = triangles_t;
						//rec = i; //	HOW DO I TELL WHICH TRIANGLE IT IS
					}
				}
			}
		}
		t = minty;
		return wasHit;
	}

}



Shape* BVH::intersect(glm::vec3 Rayorigin, glm::vec3 Raydirection, double t0, double t1, double& t, bool& Hit) {
	//step 1: determine if this bbox is hit
	bool BBoxHit = false; //flag to set if we determine this bbox was hit.
	double txmin = INT_MIN;
	double tymin = INT_MIN;
	double tzmin = INT_MIN;
	double txmax = INT_MAX;
	double tymax = INT_MAX;
	double tzmax = INT_MAX;

	if (!((Raydirection.x > -0.0001f) && (Raydirection.x < 0.0001f))) { //if ray direction.x is not equal to 0
		double t0 = (xmax - Rayorigin.x) / Raydirection.x;
		double t1 = (xmin - Rayorigin.x) / Raydirection.x;
		txmin = std::min(t0, t1);
		txmax = std::max(t0, t1);
	}
	if (!((Raydirection.y > -0.0001f) && (Raydirection.y < 0.0001f))) {
		double t0 = (ymax - Rayorigin.y) / Raydirection.y;
		double t1 = (ymin - Rayorigin.y) / Raydirection.y;
		tymin = std::min(t0, t1);
		tymax = std::max(t0, t1);
		
	}
	if (!((Raydirection.z > -0.0001f) && (Raydirection.z < 0.0001f))) {
		double t0 = (zmax - Rayorigin.z) / Raydirection.z;
		double t1 = (zmin - Rayorigin.z) / Raydirection.z;
		tzmin = std::min(t0, t1);
		tzmax = std::max(t0, t1);
	}

	double temp = std::min(tymax, txmax);
	double tmax = std::min(temp, tzmax);
	temp = std::max(tymin, txmin);
	double tmin = std::max(temp, tzmin);

	//checking conditions to see if this bbox was hit
	if (tmax < tmin) { //this bbox was not hit
		Hit = false;
		return nullptr;
	}
	if (tmax < 0) { //this bbox was not hit
		Hit = false;
		return nullptr;
	}
	if ((tmin < 0) && (tmax > 0)) { //this bbox was hit
		BBoxHit = true;
		//Shape* return_triangle = nullptr;
		//double minty = INT_MAX;// closest intersection point found it scene
		//bool wasHit = false;
		//for (int i = 0; i < triangles.size(); i++) {
		//	double triangles_t = 0; //t value found for this triangle
		//	bool isHit = false;
		//	Shape* A_triangle = triangles[i]->intersect(Rayorigin, Raydirection, t0, t1, triangles_t, isHit);
		//	if (isHit) { //this triangle has a hit
		//		if ((triangles_t >= t0) && (triangles_t <= t1)) {
		//			if (triangles_t < minty) {
		//				wasHit = true;
		//				minty = triangles_t;
		//				return_triangle = A_triangle; //	HOW DO I TELL WHICH TRIANGLE IT IS
		//			}
		//		}
		//	}
		//}
		//t = minty;
		//Hit = wasHit;
		//return return_triangle;
	}
	else if ((tmin > 0) && (tmax > 0)) { //this bbox was hit
		BBoxHit = true;
		//Shape* return_triangle = nullptr;
		//double minty = INT_MAX;// closest intersection point found it scene
		//bool wasHit = false;
		//for (int i = 0; i < triangles.size(); i++) {
		//	double triangles_t = 0; //t value found for this triangle
		//	bool isHit = false;
		//	Shape* A_triangle = triangles[i]->intersect(Rayorigin, Raydirection, t0, t1, triangles_t, isHit);
		//	if (isHit) { //this triangle has a hit
		//		if ((triangles_t >= t0) && (triangles_t <= t1)) {
		//			if (triangles_t < minty) {
		//				wasHit = true;
		//				minty = triangles_t;
		//				return_triangle = A_triangle; //	HOW DO I TELL WHICH TRIANGLE IT IS
		//			}
		//		}
		//	}
		//}
		//t = minty;
		//Hit = wasHit;
		//return return_triangle;
	}
	else { //I honestly have no idea how it could possibly reach this else but,,,, 
		Hit = false; 
		return nullptr;
	}
	if (BBoxHit == true) {
		if (left == nullptr && right == nullptr) { //this BBox is a leaf node, so run through all triangles

			Shape* return_triangle = nullptr;
			double minty = INT_MAX;// closest intersection point found it scene
			bool wasHit = false;
			for (int i = 0; i < triangles.size(); i++) {
				double triangles_t = 0; //t value found for this triangle
				bool isHit = false;
				Shape* A_triangle = triangles[i]->intersect(Rayorigin, Raydirection, t0, t1, triangles_t, isHit);
				if (isHit) { //this triangle has a hit
					if ((triangles_t >= t0) && (triangles_t <= t1)) {
						if (triangles_t < minty) {
							wasHit = true;
							minty = triangles_t;
							return_triangle = A_triangle;
						}
					}
				}
			}
			t = minty;
			Hit = wasHit;
			return return_triangle;

		}
		else { //this BBox is not a leaf node, so we check intersect function against left and right child.
			bool leftnodeHit = false;
			double leftnodet;
			Shape* LefthitShape = left->intersect(Rayorigin, Raydirection, t0, t1, leftnodet, leftnodeHit);
			bool rightnodeHit = false;
			double rightnodet;
			Shape* RighthitShape = right->intersect(Rayorigin, Raydirection, t0, t1, rightnodet, rightnodeHit);
			if (leftnodeHit && rightnodeHit) { //hit both left and right node
				Hit = true;
				if (leftnodet < rightnodet) {
					t = leftnodet;
					return LefthitShape;
				}
				else {
					t = rightnodet;
					return RighthitShape;
				}
			}
			else if (leftnodeHit) { //only leftnodeHit
				Hit = true;
				t = leftnodet;
				return LefthitShape;
			}
			else if (rightnodeHit) {
				Hit = true;
				t = rightnodet;
				return RighthitShape;
			}
			else {//neither were hit
				Hit = false;
				return nullptr;
			}
		}
	}
}

glm::vec3 BVH::GetNormal(glm::vec3& Point) {
	glm::vec3 norm = { 1, 0, 0 };
	
	return norm;
}