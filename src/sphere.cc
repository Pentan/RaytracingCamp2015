
#include "sphere.h"

using namespace r1h;

Sphere::Sphere(): radius(1.0), position(0.0)
{
}

Sphere::Sphere(const R1hFPType r, const Vector3 &pos):
	radius(r), position(pos)
{
}

Sphere::~Sphere() {
	//printf("Sphere %p destructed\n", this);
}

AABB Sphere::getAABB(const Matrix4& tm) const {
	//Vector3 minv = position - Vector3(radius);
	//Vector3 maxv = position + Vector3(radius);
	//return AABB(minv, maxv);
	
	// oriented AABB
	Vector3 p[8] = {
		position + Vector3( radius,  radius,  radius),
		position + Vector3(-radius,  radius,  radius),
		position + Vector3( radius, -radius,  radius),
		position + Vector3(-radius, -radius,  radius),
		position + Vector3( radius,  radius, -radius),
		position + Vector3(-radius,  radius, -radius),
		position + Vector3( radius, -radius, -radius),
		position + Vector3(-radius, -radius, -radius)
	};
	AABB ret;
	for(int i = 0; i < 8; i++) {
		ret.expand(Matrix4::transformV3(tm, p[i]));
	}
	return ret;
}

bool Sphere::isIntersect(const Ray &ray, Intersection *intersect) const {
	const Vector3 p_o = position - ray.origin;
	const double b = Vector3::dot(p_o, ray.direction);
	const double D4 = b * b - Vector3::dot(p_o, p_o) + radius * radius;
	
	if(D4 < 0.0) return false;
	
	const double sqrt_D4 = sqrt(D4);
	const double t1 = b - sqrt_D4, t2 = b + sqrt_D4;
	
	if(t1 < kEPS && t2 < kEPS) {
		return false;
	}
	
	if(t1 > kEPS) {
		intersect->distance = t1;
	} else {
		intersect->distance = t2;
	}
	
	intersect->position = ray.origin + intersect->distance * ray.direction;
	intersect->normal = Vector3::normalized(intersect->position - position);
	intersect->materialId = 0;
	
	return true;
}
