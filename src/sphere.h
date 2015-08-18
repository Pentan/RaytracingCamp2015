#ifndef R1H_SPHERE_H
#define R1H_SPHERE_H

#include "r1htypes.h"
#include "ray.h"
#include "intersection.h"
#include "geometry.h"
#include "aabb.h"

namespace r1h {

class Sphere : public Geometry {
    
public:
    //static const int kTypeID = 'SHPR';
    
    R1hFPType radius;
    Vector3 position;
    
	Sphere();
	Sphere(const R1hFPType r, const Vector3 &pos);
    ~Sphere();
    
    AABB getAABB(const Matrix4& tm) const;
	bool isIntersect(const Ray &ray, Intersection *intersect) const;
};

}
#endif
