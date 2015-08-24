#ifndef R1H_BSDFREFRACTION_H
#define R1H_BSDFREFRACTION_H

#include "bsdfbase.h"

namespace r1h {

class RefractionBSDF : public BSDF {
public:
	RefractionBSDF(const R1hFPType iior=1.5);
    ~RefractionBSDF();
    void makeNextRays(const Ray &ray, const Intersection &hp, const int depth, Random *rnd, std::vector<Ray> *outvecs);
	
	void setIor(R1hFPType nior);
	
private:
	R1hFPType ior;
};

}
#endif
