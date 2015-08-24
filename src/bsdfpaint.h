#ifndef R1H_BSDFPAINT_H
#define R1H_BSDFPAINT_H

#include "bsdfbase.h"

namespace r1h {

class PaintBSDF : public BSDF {
public:
	PaintBSDF(const R1hFPType iior=1.5);
    ~PaintBSDF();
    void makeNextRays(const Ray &ray, const Intersection &hp, const int depth, Random *rnd, std::vector<Ray> *outvecs);
	
	void setIor(R1hFPType nior);
	
private:
	R1hFPType ior;
};

}
#endif
