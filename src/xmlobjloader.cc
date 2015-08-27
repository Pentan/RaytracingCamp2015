
#include "xmlobjloader.h"
#include "material.h"

using namespace r1h;

XMLSceneObjLoader::XMLSceneObjLoader(std::string path, Scene *sc):
	WavefrontObj(path), scene(sc)
{
	filepath = path;
	basepath = "";

	scnobj = SceneObjectRef(new SceneObject());
	scnobj->setName(path);
	mesh = new Mesh();
	// for vt
	mesh->newAttributeContainer();
	// index starts 1
	mesh->addVertex(Vector3(0.0));
	mesh->addNormal(Vector3(0.0));
	mesh->addAttribute(0, Vector3(0.0));
	// set
	scnobj->setGeometry(GeometryRef(mesh));
	
	curMatID = 0;
}

XMLSceneObjLoader::~XMLSceneObjLoader() {
}

// implement below
// v, vn, vt, Ns, Ka, Kd, Ks, Ni, d
void XMLSceneObjLoader::foundVector(const ParameterType pt, const double x, const double y, const double z) {
	Vector3 v(x, y, z);
	
	switch(pt) {
		case OBJ_v:
			mesh->addVertex(v);
			break;
		case OBJ_vn:
			mesh->addNormal(v);
			break;
		case OBJ_vt:
			mesh->addAttribute(0, v);
			break;
		case MTL_Ns:	break;
		case MTL_Ka:	break;
		case MTL_Kd:	break;
		case MTL_Ks:	break;
		case MTL_Ni:	break;
		case MTL_d:		break;
		default:
			break;
	}
}

// mtllib, o, s, usemtl, newmtl, MTL_map_Kd
void XMLSceneObjLoader::foundString(const ParameterType pt, const std::string &str) {
	switch(pt) {
		case OBJ_usemtl:
			if(matIdMap.find(str) == matIdMap.end()) {
				matIdMap[str] = int(matIdMap.size());
			}
			curMatID = matIdMap[str];
			break;
		case OBJ_o:
		case OBJ_g:
			// read as 1 mesh
			break;
		case OBJ_s:			break;
		case OBJ_mtllib:	break;
		case MTL_newmtl:	break;
		case MTL_map_Kd:	break;
		default:
			break;
	}
}

// illum
void XMLSceneObjLoader::foundInteger(const ParameterType pt, const int i) {
	switch(pt) {
		case MTL_illum:
			break;
		default:
			printf("unknown integer value %d found in material\n", i);
	}
}

// l
void XMLSceneObjLoader::foundLine(const ParameterType pt, const int v0, const int v1) {
	switch (pt) {
		case OBJ_l:
			break;
		default:
			printf("unknown line value %d,%d found in obj\n", v0, v1);
	}
}

// f
void XMLSceneObjLoader::foundFace(const ParameterType pt, const std::vector<FaceInfo> &fids) {
	Mesh::Face face;
	
	// first triangle
	face.setV(fids[0].v, fids[1].v, fids[2].v);
	face.setN(fids[0].vn, fids[1].vn, fids[2].vn);
	face.addAttr(0, fids[0].vt, fids[1].vt, fids[2].vt);
	face.matid = curMatID;
	mesh->addFace(face);
	
	//std::cout << "f " << fids[0].v << "," << fids[1].v << "," << fids[2].v << std::endl;
	
	if(fids.size() > 3) {
		// quad
		face.attrs.clear();

		face.setV(fids[2].v, fids[3].v, fids[0].v);
		face.setN(fids[2].vn, fids[3].vn, fids[0].vn);
		face.addAttr(0, fids[2].vt, fids[3].vt, fids[0].vt);
		face.matid = curMatID;
		mesh->addFace(face);
	}
}

// mtl
void XMLSceneObjLoader::importMTL(std::string filename) {
	// noop
}

// endof .mtl or .obj
void XMLSceneObjLoader::endFile(std::string fullpath) {
	if(mesh->getFaceCount() > 0) {
		// if don't has normal, calc it.
		if(mesh->getNormalCount() <= 1) {
			mesh->calcSmoothNormals();
		}
		
		// This is geometry loader. don't add to scene
		//scene->addObject(scnobj);
	}
}

SceneObject* XMLSceneObjLoader::getSceneObject() {
	return scnobj.get();
}

size_t XMLSceneObjLoader::getMaterialCount() {
	return matIdMap.size();
}

