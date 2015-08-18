
#include "bsdfspecular.h"

using namespace r1h;

SpecularBSDF::~SpecularBSDF()
{
}

void SpecularBSDF::makeNextRays(const Ray &ray, const Intersection &isect, const int depth, Random *rnd, std::vector<Ray> *outvecs) {
	Ray nxtray;
	nxtray = Ray(isect.position, ray.direction - isect.normal * 2.0 * Vector3::dot(isect.normal, ray.direction));
	nxtray.weight = Color(1.0);
	outvecs->push_back(nxtray);
}
