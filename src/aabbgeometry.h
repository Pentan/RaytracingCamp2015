#ifndef R1H_AABBGEOMETRY_H
#define R1H_AABBGEOMETRY_H

#include "r1htypes.h"
#include "ray.h"
#include "intersection.h"
#include "geometry.h"
#include "aabb.h"

namespace r1h {

class AABBGeometry : public Geometry {
public:
    static const int kTypeID;
    
    AABB aabb;
    
    AABBGeometry(const Vector3 min, const Vector3 max);
    AABBGeometry(const AABB inaabb);
    AABB getAABB(const Matrix4& tm) const;
    
    bool isIntersect(const Ray &ray, Intersection *intersect) const;
};

}
#endif
