
#include "materialsinglebsdf.h"

using namespace r1h;

SingleBSDFMaterial::SingleBSDFMaterial(BSDFRef bsdf):
	Material(),
	bsdf(bsdf)
{
}

SingleBSDFMaterial::~SingleBSDFMaterial() {
}

void SingleBSDFMaterial::setReflectanceColor(const Color col) {
	reflectanceTex = TextureRef(new ConstantColorTexture(col));
}
void SingleBSDFMaterial::setReflectanceTexture(TextureRef tex) {
	reflectanceTex = tex;
}
Texture* SingleBSDFMaterial::getReflectanceTexture() const {
	return reflectanceTex.get();
}

void SingleBSDFMaterial::setEmittanceColor(const Color col) {
	emittanceTex = TextureRef(new ConstantColorTexture(col));
}
void SingleBSDFMaterial::setEmittanceTexture(TextureRef tex) {
	emittanceTex = tex;
}
Texture* SingleBSDFMaterial::getEmittanceTexture() const {
	return emittanceTex.get();
}

void SingleBSDFMaterial::setBSDF(BSDFRef newbsdf) {
	bsdf = newbsdf;
}

BSDF* SingleBSDFMaterial::getBSDF() const {
	return bsdf.get();
}

Color SingleBSDFMaterial::getReflectance(const SceneObject *obj, const Intersection &isect) const {
	return reflectanceTex->sample(obj, &isect);
}

Color SingleBSDFMaterial::getEmittance(const SceneObject *obj, const Intersection &isect) const {
	return emittanceTex->sample(obj, &isect);
}

void SingleBSDFMaterial::makeNextRays(const Ray &ray, const SceneObject *obj, const Intersection &isect, const int depth, Random *rnd, std::vector<Ray> *outvecs) const {
	bsdf->makeNextRays(ray, isect, depth, rnd, outvecs);
	for(size_t i = 0; i < outvecs->size(); i++) {
		Ray &tmpray = outvecs->at(i);
		tmpray.weight = Vector3::mul(tmpray.weight, reflectanceTex->sample(obj, &isect));
	}
}


