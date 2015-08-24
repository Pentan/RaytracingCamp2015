#include <iostream>

#include "random.h"
#include "texturebase.h"

#include "intersection.h"
#include "sceneobject.h"
#include "geometry.h"
#include "mesh.h"

using namespace r1h;

/// base class

Texture::Texture():
	mapType(kWorld),
	isUseTransform(true)
{}

Texture::~Texture() {}

//virtual Color sample(const Vector3 &p) const = 0;
Color Texture::sample(const SceneObject *obj, const Intersection *isect) {
	Vector3 p(0.0);
	
	switch (mapType) {
		case kUV:
		{
			// TODO: UV is vertex attribute 0.
			Geometry *geom = obj->getGeometry();
			if(geom->getAssetType() == Mesh::kTypeID) {
				const Mesh *mesh = reinterpret_cast<Mesh*>(geom);
				p = mesh->getVaryingAttr(isect->faceId, 0, isect->varyingWeight);
			}
		}
			break;
		case kWorld:
			p = isect->position;
			break;
		case kLocal:
			p = obj->toLocalPosition(isect->position);
			break;
	}
	
	applyTransform(p);
	return sample(p);
}

void Texture::setIsUseTransform(const bool isuse) {
 isUseTransform = isuse;
}
void Texture::setTransform(const Matrix4 m){
	transform = m;
}

Vector3 Texture::applyTransform(const Vector3 &p) const {
    if(!isUseTransform) {
        return p;
    }
    return Matrix4::transformV3(transform, p);
}

void Texture::setMapType(MapType type) {
	mapType = type;
}

int Texture::getMapType() const {
	return mapType;
}

/// constant color
ConstantColorTexture::ConstantColorTexture(const Color col): color(col) {
}

ConstantColorTexture::~ConstantColorTexture()
{}

Color ConstantColorTexture::sample(const Vector3 &p) const {
	return color;
}

void ConstantColorTexture::setColor(const Color& col) {
	color = col;
}
