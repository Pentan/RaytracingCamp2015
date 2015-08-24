
#ifndef R1H_MATERIAL_SINGLE_BSDF_H
#define R1H_MATERIAL_SINGLE_BSDF_H

#include "r1htypes.h"
#include "material.h"
#include "bsdf.h"
#include "texture.h"

namespace r1h {

class SingleBSDFMaterial : public Material{
public:
	SingleBSDFMaterial(BSDFRef bsdf=nullptr);
	~SingleBSDFMaterial();
	
	void setAlbedoColor(const Color col);
	void setAlbedoTexture(TextureRef tex);
	Texture* getAlbedoTexture() const;
	
	void setEmissionColor(const Color col);
	void setEmissionTexture(TextureRef tex);
	Texture* getEmissionTexture() const;
	
	void setBSDF(BSDFRef newbsdf);
	BSDF* getBSDF() const;
	
	virtual Color albedo(const SceneObject *obj, const Intersection &isect) const;
	virtual Color emission(const SceneObject *obj, const Intersection &isect) const;
	virtual void makeNextRays(const Ray &ray, const Intersection &isect, const int depth, Random *rnd, std::vector<Ray> *outvecs) const;
	
private:
	BSDFRef bsdf;
	
	TextureRef albedoTex;
	TextureRef emissionTex;
};

}

#endif
