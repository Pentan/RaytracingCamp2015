#ifndef R1H_XML_OBJLOADER_H
#define R1H_XML_OBJLOADER_H

#include <cstdio>
#include <string>
#include <vector>
#include <map>
#include "wavefrontobj.h"
#include "scene.h"
#include "sceneobject.h"
#include "mesh.h"

namespace r1h {

class XMLSceneObjLoader: public WavefrontObj {
public:
	XMLSceneObjLoader(std::string path, Scene *sc);
	~XMLSceneObjLoader();
	
	// implement below
	// v, vn, vt, Ns, Ka, Kd, Ks, Ni, d
	virtual void foundVector(const ParameterType pt, const double x, const double y, const double z);
	// mtllib, o, s, usemtl, newmtl, MTL_map_Kd
	virtual void foundString(const ParameterType pt, const std::string &str);
	// illum
	virtual void foundInteger(const ParameterType pt, const int i);
	// l
	virtual void foundLine(const ParameterType pt, const int v0, const int v1);
	// f
	virtual void foundFace(const ParameterType pt, const std::vector<FaceInfo> &fids);
	
	// mtl
	virtual void importMTL(std::string filename);
	
	// endof .mtl or .obj
	virtual void endFile(std::string fullpath);
	
	///
	SceneObject* getSceneObject();
	size_t getMaterialCount();
	
private:
	Scene *scene;
	SceneObjectRef scnobj;
	Mesh* mesh;
	
	std::map<std::string, int> matIdMap;
	int curMatID;
};

}

#endif
