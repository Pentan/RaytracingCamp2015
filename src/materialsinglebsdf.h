
#ifndef R1H_MATERIAL_SINGLE_BSDF_H
#define R1H_MATERIAL_SINGLE_BSDF_H

#include <vector>

#include "r1htypes.h"
#include "material.h"
#include "bsdf.h"
#include "texture.h"
#include "ray.h"

namespace r1h {

class SingleBSDFMaterial : public Material{
public:
	SingleBSDFMaterial(BSDFRef bsdf=nullptr);
	~SingleBSDFMaterial();
	
	void setReflectanceColor(const Color col);
	void setReflectanceTexture(TextureRef tex);
	Texture* getReflectanceTexture() const;
	
	void setEmittanceColor(const Color col);
	void setEmittanceTexture(TextureRef tex);
	Texture* getEmittanceTexture() const;
	
	void setBSDF(BSDFRef newbsdf);
	BSDF* getBSDF() const;
	
	virtual Color getReflectance(const SceneObject *obj, const Intersection &isect) const;
	virtual Color getEmittance(const SceneObject *obj, const Intersection &isect) const;
	virtual void makeNextRays(const Ray &ray, const SceneObject *obj, const Intersection &isect, const int depth, Random *rnd, std::vector<Ray> *outvecs) const;
	
private:
	BSDFRef bsdf;
	
	TextureRef reflectanceTex;
	TextureRef emittanceTex;
};

}

#endif
