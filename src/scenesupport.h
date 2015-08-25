
#ifndef R1H_SCENESUPPORT_H
#define R1H_SCENESUPPORT_H

#include "material.h"
#include "bsdf.h"
#include "wavefrontobj.h"
#include "scene.h"
#include "mesh.h"

/// support class
namespace r1h {

// utilities
class SceneSupport {
public:
	static std::string getBasePath(const std::string& path, std::string* outfilename=nullptr);
};

/// simple material
class ObjMaterial : public Material {
public:
	ObjMaterial();
	~ObjMaterial();
	
	// override methods
	Color skyColor(const Ray &ray) const;
	Color getReflectance(const SceneObject *obj, const Intersection &isect) const;
	Color getEmittance(const SceneObject *obj, const Intersection &isect) const;
	void makeNextRays(const Ray &ray, const SceneObject *obj, const Intersection &isect, const int depth, Random *rnd, std::vector<Ray> *outvecs) const;
	
	//
	void setKd(Color kd);
	void setEmissionColor(Color emit);
private:
	BSDF *bsdf;
	Color diffuseColor;
	Color emitColor;
};

// simple .obj loader
class SimpleObjLoader : public WavefrontObj {
public:
	SimpleObjLoader(std::string filepath, Scene *sc);
	~SimpleObjLoader();
	
	// v, vn, vt, Ns, Ka, Kd, Ks, Ni, d
	void foundVector(const ParameterType pt, const double x, const double y, const double z);
	// mtllib, o, s, usemtl, newmtl, MTL_map_Kd
	void foundString(const ParameterType pt, const std::string &str);
	// illum
	void foundInteger(const ParameterType pt, const int i);
	// l
	void foundLine(const ParameterType pt, const int v0, const int v1);
	// f
	void foundFace(const ParameterType pt, const std::vector<FaceInfo> &fidvec);
	// mtl
	//void importMTL(const char *filename);
	// endof .mtl or .obj
	void endFile(std::string fullpath);
	
	void calcCameraPosition(R1hFPType fov, R1hFPType aspect);
private:
	Scene *scene;
	MaterialRef defaultMat;
	std::map<std::string, MaterialRef> matMap;
	ObjMaterial *curMat;
	MaterialRef curMatRef;
	SceneObject *curObj;
	std::vector<SceneObjectRef> tmpObjs;
	Mesh *curMesh;
	int curMatId;
	AABB sceneAABB;
	int curNumVs;	// number of vertices only read from obj file.
	int curNumVNs;
	int curNumVTs;
	int vOffset;
	int vnOffset;
	int vtOffset;
};

}


#endif
