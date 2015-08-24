
#include "materialsinglebsdf.h"

using namespace r1h;

SingleBSDFMaterial::SingleBSDFMaterial(BSDFRef bsdf):
	Material(),
	bsdf(bsdf)
{
}

SingleBSDFMaterial::~SingleBSDFMaterial() {
}

void SingleBSDFMaterial::setAlbedoColor(const Color col) {
	albedoTex = TextureRef(new ConstantColorTexture(col));
}
void SingleBSDFMaterial::setAlbedoTexture(TextureRef tex) {
	albedoTex = tex;
}
Texture* SingleBSDFMaterial::getAlbedoTexture() const {
	return albedoTex.get();
}

void SingleBSDFMaterial::setEmissionColor(const Color col) {
	emissionTex = TextureRef(new ConstantColorTexture(col));
}
void SingleBSDFMaterial::setEmissionTexture(TextureRef tex) {
	emissionTex = tex;
}
Texture* SingleBSDFMaterial::getEmissionTexture() const {
	return emissionTex.get();
}

void SingleBSDFMaterial::setBSDF(BSDFRef newbsdf) {
	bsdf = newbsdf;
}

BSDF* SingleBSDFMaterial::getBSDF() const {
	return bsdf.get();
}

Color SingleBSDFMaterial::albedo(const SceneObject *obj, const Intersection &isect) const {
	return albedoTex->sample(obj, &isect);
}

Color SingleBSDFMaterial::emission(const SceneObject *obj, const Intersection &isect) const {
	return emissionTex->sample(obj, &isect);
}

void SingleBSDFMaterial::makeNextRays(const Ray &ray, const Intersection &isect, const int depth, Random *rnd, std::vector<Ray> *outvecs) const {
	bsdf->makeNextRays(ray, isect, depth, rnd, outvecs);
}


