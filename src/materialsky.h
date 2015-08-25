#ifndef R1H_SKY_MATERIAL_H
#define R1H_SKY_MATERIAL_H

#include "material.h"
#include "texture.h"

namespace r1h {

class SkyMaterial : public Material {
public:
	SkyMaterial();
	virtual ~SkyMaterial();
	
	Color skyColor(const Ray &ray) const;
	
	void setTexture(TextureRef tex);
	void setColor(const Color& col);
	void setTransform(const Matrix4& m);
	
	// These methods are noop
	virtual Color getReflectance(const SceneObject *obj, const Intersection &isect) const;
	virtual Color getEmittance(const SceneObject *obj, const Intersection &isect) const;
	virtual void makeNextRays(const Ray &ray, const SceneObject *obj, const Intersection &isect, const int depth, Random *rnd, std::vector<Ray> *outvecs) const;
	
private:
	TextureRef texture;
	Color color;
	Matrix4 transform;
};

typedef std::shared_ptr<SkyMaterial> SkyMaterialRef;

}

#endif
