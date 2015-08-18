#ifndef R1H_AABB_H
#define R1H_AABB_H

#include <cfloat>
#include "r1htypes.h"
#include "ray.h"

namespace r1h {

class AABB {
public:
    Vector3 min;
    Vector3 max;
    Vector3 centroid;
    int dataId;
	
	AABB();
    AABB(const R1hFPType minval, const R1hFPType maxval);
    AABB(const Vector3 minvec, const Vector3 maxvec);
	
    void clear();
    Vector3 getSize() const;
    
    void expand(const Vector3 &p);
    void expand(const AABB &aabb);
    
    void updateCentroid();
	
    bool isInside(const Vector3 &p) const;
    bool isIntersect(const Ray &ray, R1hFPType *outmin=0, int *outaxis=0) const;
};

}
#endif
