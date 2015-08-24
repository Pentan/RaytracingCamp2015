#ifndef R1H_BSDFBASE_H
#define R1H_BSDFBASE_H

#include <vector>
#include <memory>
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

typedef std::shared_ptr<BSDF> BSDFRef;

}
#endif
