#ifndef R1H_MATERIALBASE_H
#define R1H_MATERIALBASE_H

#include <memory>
#include "r1htypes.h"
#include "intersection.h"
//#include "sceneobject.h"
#include "random.h"
#include "asset.h"

namespace r1h {

class SceneObject;

class Material : public Asset {
public:
    Material() {}
    virtual ~Material() {}
	
    virtual Color getReflectance(const SceneObject *obj, const Intersection &isect) const = 0;
    virtual Color getEmittance(const SceneObject *obj, const Intersection &isect) const = 0;
	// must be ray.weight = reflectance;
	virtual void makeNextRays(const Ray &ray, const SceneObject *obj, const Intersection &isect, const int depth, Random *rnd, std::vector<Ray> *outvecs) const = 0;
};

typedef std::shared_ptr<Material> MaterialRef;

/// Stub
class StubMaterial : public Material {
public:
	StubMaterial(const std::string& refid) : Material() {
		assetId = refid;
		assetType = Asset::Type::kStub;
	}
	~StubMaterial() {}
	
	virtual Color getReflectance(const SceneObject *obj, const Intersection &isect) const { return Color(0.0); };
	virtual Color getEmittance(const SceneObject *obj, const Intersection &isect) const { return Color(1.0, 0.0, 0.0); };
	virtual void makeNextRays(const Ray &ray, const SceneObject *obj, const Intersection &isect, const int depth, Random *rnd, std::vector<Ray> *outvecs) const {};
};

}

#endif
