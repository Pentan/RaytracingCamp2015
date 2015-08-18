#ifndef R1H_BVHNODE_H
#define R1H_BVHNODE_H

#include "r1htypes.h"
#include <vector>
#include "aabb.h"
#include "intersection.h"

namespace r1h {

class BVHLeaf {
public:
	virtual ~BVHLeaf() {};
	virtual bool isIntersectLeaf(int dataid, const Ray &ray, Intersection *intersect) const = 0;
};

class BVHNode {
public:
    BVHNode *children;
    int childNum;
    AABB aabb;
    union {
        int axis;
        int dataId;
    };
    
    BVHNode();
    ~BVHNode();
    
    void allocChildren(const int chnum=2);
    bool isLeaf() const;
    int buildAABBTree(AABB *aabbArray, const int aabbnum);
    
public:
	static bool isIntersectBVHTree(const BVHLeaf *leafobj, const BVHNode &node, const Ray &ray, Intersection *intersect);
	
private:
    struct AABBAxisComparator {
        int axisId;
        AABBAxisComparator(const int a);
        bool operator() (const AABB &aabb1, const AABB &aabb2);
    };
	
    int recurseBuildTree(BVHNode &node, AABB *aabbArray, const int aabbnum, const int depth);
};


}
#endif
