
#include "materialsky.h"

using namespace r1h;

SkyMaterial::SkyMaterial() :
	texture(nullptr),
	color(1.0)
{
	transform.setIdentity();
}

SkyMaterial::~SkyMaterial() {
}

Color SkyMaterial::skyColor(const Ray &ray) const {
	Color ret = color;
	
	if(texture.get() != nullptr) {
		Vector3 dirv = Matrix4::mulV3(transform, ray.direction);
		dirv.normalize();

		Vector3 coord;
		coord.x = atan2(dirv.x, -dirv.z) / (kPI * 2.0) + 0.5;
		coord.y = acos(dirv.y) / kPI;
		coord.z = 0.0;
		ret = Color::mul(ret, texture->sample(coord));
	}
	
	return ret;
}

void SkyMaterial::setTexture(TextureRef tex) {
	texture = tex;
}
void SkyMaterial::setColor(const Color& col) {
	color = col;
}
void SkyMaterial::setTransform(const Matrix4& m) {
	transform = m;
}

Color SkyMaterial::getReflectance(const SceneObject *obj, const Intersection &isect) const {
	return Color(0.0);
}

Color SkyMaterial::getEmittance(const SceneObject *obj, const Intersection &isect) const {
	return Color(0.0);
}

void SkyMaterial::makeNextRays(const Ray &ray, const SceneObject *obj, const Intersection &isect, const int depth, Random *rnd, std::vector<Ray> *outvecs) const {
	// noop
}
