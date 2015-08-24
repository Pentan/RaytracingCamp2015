#ifndef R1H_BSDFTRANSLUCENT_H
#define R1H_BSDFTRANSLUCENT_H

#include "bsdfbase.h"

namespace r1h {

class TranslucentBSDF : public BSDF {
public:
    ~TranslucentBSDF();
    void makeNextRays(const Ray &ray, const Intersection &hp, const int depth, Random *rnd, std::vector<Ray> *outvecs);
};

}
#endif
