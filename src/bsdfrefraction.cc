
#include "bsdfrefraction.h"

using namespace r1h;

RefractionBSDF::RefractionBSDF(const R1hFPType iior): ior(iior)
{
}

RefractionBSDF::~RefractionBSDF()
{
}

void RefractionBSDF::makeNextRays(const Ray &ray, const Intersection &hp, const int depth, Random *rnd, std::vector<Ray> *outvecs) {
	const Ray reflection_ray = Ray(hp.position, ray.direction - hp.normal * 2.0 * Vector3::dot(hp.normal, ray.direction));
	const Vector3 orienting_normal = hp.orientingNormal(ray);
	const bool into = Vector3::dot(hp.normal, orienting_normal) > 0.0;
	
	// Snell's law
	const double nc = 1.0;
	const double nt = ior;
	const double nnt = into ? (nc / nt) : (nt / nc);
	const double ddn = Vector3::dot(ray.direction, orienting_normal);
	const double cos2t = 1.0 - nnt * nnt * (1.0 - ddn * ddn);
	
	// full reflection
	if(cos2t < 0.0) {
		Ray nxtray;
		nxtray = reflection_ray;
		nxtray.weight = Color(1.0, 1.0, 1.0);
		outvecs->push_back(nxtray);
		return;
	}
	
	// refraction
	const Ray refraction_ray = Ray(
		hp.position,
		Vector3::normalized(ray.direction * nnt - hp.normal * (into ? 1.0 : -1.0) * (ddn * nnt + sqrt(cos2t)))
		);
	
	// Schlick' Fresnel aproximation
	const double a = nt - nc, b = nt + nc;
	const double R0 = (a * a) / (b * b);
	
	const double c = 1.0 - (into ? -ddn : Vector3::dot(refraction_ray.direction, -1.0 * orienting_normal));
	const double Re = R0 + (1.0 - R0) * pow(c, 5.0);
	const double nnt2 = pow(into ? (nc / nt) : (nt / nc), 2.0);
	const double Tr = (1.0 - Re) * nnt2;
	
	// In deep step case, choose reflect or refract by russian roulette.
	const double probability = 0.25 + 0.5 * Re;
	if(depth > 2) {
		Ray nxtray;
		if(rnd->next01() < probability) { // reflection
			nxtray = reflection_ray;
			nxtray.weight = Color(Re / (1.0 - probability));
		} else { // refraction
			nxtray = refraction_ray;
			nxtray.weight = Color(Tr / (1.0 - probability));
		}
		outvecs->push_back(nxtray);
	} else { // shallow case, calculate both
		Ray reray;
		reray = reflection_ray;
		reray.weight = Color(Re);
		outvecs->push_back(reray);
		
		Ray trray;
		trray = refraction_ray;
		trray.weight = Color(Tr);
		outvecs->push_back(trray);
	}
}

void RefractionBSDF::setIor(R1hFPType nior) {
	ior = nior;
}
