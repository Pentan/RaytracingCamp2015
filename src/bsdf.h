#ifndef R1H_BSDF_H
#define R1H_BSDF_H

#include <vector>
#include "r1htypes.h"
#include "ray.h"
#include "intersection.h"
#include "random.h"

namespace r1h {

/// abstract class
class BSDF {
public:
    virtual ~BSDF() {}
    virtual void makeNextRays(const Ray &ray, const Intersection &hp, const int depth, Random *rnd, std::vector<Ray> *outvecs) = 0;
};

}
#endif
