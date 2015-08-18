#ifndef R1H_INTERSECTION_H
#define R1H_INTERSECTION_H

#include <vector>
#include "r1htypes.h"
#include "ray.h"
//#include "material.h"

namespace r1h {

class Intersection {
public:
    enum {
        kNoIntersected = -1
    };
    
    R1hFPType distance;
	Vector3 varyingWeight;
    int faceId;
	
    Vector3 normal;
    Vector3 position;
    
    int materialId;
    int objectId;
	
	/*
	 struct {
	 Vector3 position;
	 Vector3 normal;
	 Vector3 tangent;
	 } world, local;
	 */
	
    //std::vector<Vector3> attributes;
    
    //Material *material;
    
    Intersection();
    void clear();
	Vector3 orientingNormal(const Ray &ray) const;
};

}

#endif
