
#include "intersection.h"

using namespace r1h;

Intersection::Intersection():
	objectId(kNoIntersected),
	distance(kINF),
	normal(),
	position()
{}

Vector3 Intersection::orientingNormal(const Ray &ray) const {
	return (Vector3::dot(normal, ray.direction) < 0.0)? normal : (normal * -1.0);
}

void Intersection::clear() {
    distance = kINF;
    objectId = -1;
}
