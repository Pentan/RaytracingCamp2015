#ifndef R1H_RAY_H
#define R1H_RAY_H

#include "r1htypes.h"

namespace r1h {

struct Ray {
    Vector3 origin;
    Vector3 direction;
    Color weight;
	
    Ray() :
        origin(0.0, 0.0, 0.0),
		direction(0.0, 0.0, -1.0),
		weight(1.0)
    {};
    
    Ray(const Vector3 &org, const Vector3 &dir):
		origin(org),
		direction(dir),
		weight(1.0)
    {};
    
    Ray(const Vector3 &org, const Vector3 &dir, const Color &col):
		origin(org),
		direction(dir),
		weight(col)
    {};
	
    void smallOffset(const Vector3 &n) {
        origin += n * ((Vector3::dot(direction, n) > 0.0)? kEPS : -kEPS);
    }
	
	Ray makeTransformed(Matrix4 m) const {
		Ray ret = *this;
		ret.origin = Matrix4::transformV3(m, origin);
		ret.direction = Matrix4::mulV3(m, direction);
		ret.direction.normalize();
		return ret;
	}
};

}
#endif
