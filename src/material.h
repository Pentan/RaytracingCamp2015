#ifndef R1H_MATERIAL_H
#define R1H_MATERIAL_H

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
	
	virtual Color skyColor(const Ray &ray) const { return Color(0.0); }; // for background
    
    virtual Color albedo(const SceneObject *obj, const Intersection &isect) const = 0;
    virtual Color emission(const SceneObject *obj, const Intersection &isect) const = 0;
    virtual void makeNextRays(const Ray &ray, const Intersection &isect, const int depth, Random *rnd, std::vector<Ray> *outvecs) const = 0;
};

typedef std::shared_ptr<Material> MaterialRef;

}

#endif
