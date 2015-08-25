
#include "sceneobject.h"
#include "intersection.h"

using namespace r1h;

SceneObject::SceneObject():
	geometry(nullptr)
{}

SceneObject::~SceneObject() {}

void SceneObject::setGeometry(GeometryRef geom) {
	geometry = geom;
}
Geometry* SceneObject::getGeometry() const {
	return geometry.get();
}

GeometryRef SceneObject::getGeometryRef() {
	return geometry;
}

int SceneObject::addMaterial(MaterialRef matref) {
	materials.push_back(matref);
	return (int)materials.size() - 1;
}

Material* SceneObject::getMaterialById(int matid) const {
	return materials[matid].get();
}

size_t SceneObject::getMaterialCount() const {
	return materials.size();
}

void SceneObject::replaceMaterial(int matid, MaterialRef matref) {
	materials[matid] = matref;
}

void SceneObject::prepareRendering() {
	geometry->prepareRendering();
}

void SceneObject::setTransform(const Matrix4& m) {
	transform = m;
	iTransform = Matrix4::inverted(m, nullptr);  // for returned position
	itTransform = Matrix4::transposed(iTransform); // for returned normal
}

Matrix4 SceneObject::getTransform() const {
	return transform;
}

Vector3 SceneObject::toLocalPosition(const Vector3& wp) const {
	return Matrix4::transformV3(iTransform, wp);
}

AABB SceneObject::getAABB() const {
	return geometry->getAABB(transform);
}

bool SceneObject::isIntersect(const Ray &ray, Intersection *intersect) {
	Ray tray = ray.makeTransformed(iTransform);
	if(geometry->isIntersect(tray, intersect)) {
		// fix transform
		intersect->position = Matrix4::transformV3(transform, intersect->position);
		intersect->normal = Matrix4::mulV3(itTransform, intersect->normal);
		intersect->normal.normalize();
		intersect->distance = Vector3::distance(ray.origin, intersect->position);
		return true;
	}
	return false;
	//return geometry->isIntersect(ray, intersect);
}

void SceneObject::setName(const std::string &newname) {
	name = newname;
}

std::string SceneObject::getName() const {
	return name;
}
