
#include "bsdfdiffuse.h"

using namespace r1h;

DiffuseBSDF::~DiffuseBSDF() {
}

void DiffuseBSDF::makeNextRays(const Ray &ray, const Intersection &hp, const int depth, Random *rnd, std::vector<Ray> *outvecs) {
	Vector3 w, u, v;
	w = hp.orientingNormal(ray);
	
	if(fabs(w.x) > kEPS) {
		u = Vector3::normalized(Vector3::cross(Vector3(0.0, 1.0, 0.0), w));
	} else {
		u = Vector3::normalized(Vector3::cross(Vector3(1.0, 0.0, 0.0), w));
	}
	v = Vector3::cross(w, u);
	
	// importance sampling using cosine.
	const double r1 = 2.0 * kPI * rnd->next01();
	const double r2 = rnd->next01();
	const double r2s = sqrt(r2);
	Vector3 dir = Vector3::normalized((
		u * cos(r1) * r2s +
		v * sin(r1) * r2s +
		w * sqrt(1.0 - r2)
	));
	
	Ray nextray;
	nextray = Ray(hp.position, dir, Color(1.0, 1.0, 1.0));
	outvecs->push_back(nextray);
}
