#ifndef R1H_BSDFDIFFUSE_H
#define R1H_BSDFDIFFUSE_H

#include "bsdfbase.h"

namespace r1h {

class DiffuseBSDF : public BSDF {
public:
    ~DiffuseBSDF();
    void makeNextRays(const Ray &ray, const Intersection &hp, const int depth, Random *rnd, std::vector<Ray> *outvecs);
};

}
#endif
