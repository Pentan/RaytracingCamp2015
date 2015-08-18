
#include "aabbgeometry.h"

using namespace r1h;

const int AABBGeometry::kTypeID = 'AABB';


AABBGeometry::AABBGeometry(const Vector3 min, const Vector3 max):
	Geometry(), aabb(min, max)
{}

AABBGeometry::AABBGeometry(const AABB inaabb):
	Geometry(), aabb(inaabb)
{}

AABB AABBGeometry::getAABB(const Matrix4& tm) const {
	Vector3 p[8] = {
		Vector3(aabb.min.x, aabb.min.y, aabb.min.z),
		Vector3(aabb.max.x, aabb.min.y, aabb.min.z),
		Vector3(aabb.min.x, aabb.max.y, aabb.min.z),
		Vector3(aabb.max.x, aabb.max.y, aabb.min.z),
		Vector3(aabb.min.x, aabb.min.y, aabb.max.z),
		Vector3(aabb.max.x, aabb.min.y, aabb.max.z),
		Vector3(aabb.min.x, aabb.max.y, aabb.max.z),
		Vector3(aabb.max.x, aabb.max.y, aabb.max.z)
	};
	AABB ret;
	for(int i = 0; i < 8; i++) {
		ret.expand(Matrix4::transformV3(tm, p[i]));
	}
	return ret;
}

bool AABBGeometry::isIntersect(const Ray &ray, Intersection *intersect) const {
	double t;
	int axis;
	
	if(aabb.isIntersect(ray, &t, &axis)) {
		intersect->distance = t;
		intersect->position = ray.origin + ray.direction * t;
		intersect->materialId = 0;
		
		double d = (ray.direction.v[axis] > 0.0)? -1.0 : 1.0;
		switch(axis) {
			case 0:
				intersect->normal = Vector3(d, 0.0, 0.0);
				break;
			case 1:
				intersect->normal = Vector3(0.0, d, 0.0);
				break;
			case 2:
				intersect->normal = Vector3(0.0, 0.0, d);
				break;
		}
		
		return true;
	}
	return false;
}
