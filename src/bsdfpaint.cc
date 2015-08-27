
#include "bsdfpaint.h"

using namespace r1h;

PaintBSDF::PaintBSDF(const R1hFPType iior): ior(iior)
{
}

PaintBSDF::~PaintBSDF()
{
}

void PaintBSDF::setIor(R1hFPType nior) {
	ior = nior;
}

void PaintBSDF::makeNextRays(const Ray &ray, const Intersection &hp, const int depth, Random *rnd, std::vector<Ray> *outvecs) {
	const Ray reflection_ray = Ray(hp.position, ray.direction - hp.normal * 2.0 * Vector3::dot(hp.normal, ray.direction));
	const Vector3 orienting_normal = hp.orientingNormal(ray);
	
	// Snell's law
	const double nc = 1.0;
	const double nt = ior;
	const double nnt = nc / nt;
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
	
	// diffuse
	Vector3 u, v;
	
	if(fabs(orienting_normal.x) > kEPS) {
		u = Vector3::normalized(Vector3::cross(Vector3(0.0, 1.0, 0.0), orienting_normal));
	} else {
		u = Vector3::normalized(Vector3::cross(Vector3(1.0, 0.0, 0.0), orienting_normal));
	}
	v = Vector3::cross(orienting_normal, u);
	
	// importance sampling using cosine.
	const double r1 = 2.0 * kPI * rnd->next01();
	const double r2 = rnd->next01();
	const double r2s = sqrt(r2);
	Vector3 dir = Vector3::normalized((
                                       u * cos(r1) * r2s +
                                       v * sin(r1) * r2s +
                                       orienting_normal * sqrt(1.0 - r2)
                                       ));
	
	Ray diffuse_ray;
	diffuse_ray = Ray(hp.position, dir, Color(1.0, 1.0, 1.0));
    
    
	// Schlick' Fresnel aproximation
	const double a = nt - nc, b = nt + nc;
	const double R0 = (a * a) / (b * b);
	
	const double c = 1.0 + ddn;
	const double Re = R0 + (1.0 - R0) * pow(c, 5.0);
	const double nnt2 = nnt * nnt;
	const double Tr = (1.0 - Re) * nnt2;
	
	// In deep step case, choose reflect or refract by russian roulette.
	const double probability = 0.25 + 0.5 * Re;
	if(depth > 2) {
		Ray nxtray;
		if(rnd->next01() < probability) { // reflection
			nxtray = reflection_ray;
			nxtray.weight = Color(Re / (1.0 - probability));
		} else { // refraction
			nxtray = diffuse_ray;
			nxtray.weight = Color(Tr / (1.0 - probability));
		}
		outvecs->push_back(nxtray);
	} else { // shallow case, calculate both
		Ray reray;
		reray = reflection_ray;
		reray.weight = Color(Re);
		outvecs->push_back(reray);
		
		Ray dfsray;
		dfsray = diffuse_ray;
		dfsray.weight = Color(Tr);
		outvecs->push_back(dfsray);
	}
}


