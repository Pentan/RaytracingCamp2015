#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstring>
#include <cassert>

#include "camera.h"
#include "xmlscene.h"
#include "scenesupport.h"
#include "material.h"
#include "sphere.h"
#include "aabbgeometry.h"
#include "xmlobjloader.h"

using namespace r1h;

//////////
// TinyXML backend xml parser
#include <tinyxml/tinyxml2.h>

class XMLSceneLoader::XmlParser {
public:
	XmlParser();
	~XmlParser();
	
	bool load(std::string& filename, XMLSceneLoader *xmlloader);
	
private:
	XMLSceneLoader *loader;
	
private:
	bool parseRenderConfig(tinyxml2::XMLElement *elm);
	bool parseAssetLibrary(tinyxml2::XMLElement *elm);
	bool parseCamera(tinyxml2::XMLElement *elm);
	bool parseSky(tinyxml2::XMLElement *elm);
	bool parseObject(tinyxml2::XMLElement *elm);
	
	TextureRef parseTextureElement(tinyxml2::XMLElement *elm);
	MaterialRef parseMaterialElement(tinyxml2::XMLElement *elm);
	GeometryRef parseGeometryElement(tinyxml2::XMLElement *elm);
	
	Vector3 parseVector3Element(tinyxml2::XMLElement *elm);
	Color parseColorElement(tinyxml2::XMLElement *elm);
	Matrix4 parseMatrix4Element(tinyxml2::XMLElement *elm);
	
	Matrix4 parseTransformElement(tinyxml2::XMLElement *elm);
	
	bool checkIntAttribute(tinyxml2::XMLElement *elm, const char *key, int *val);
	bool checkFloatAttribute(tinyxml2::XMLElement *elm, const char *key, R1hFPType *val);
	bool checkStringAttribute(tinyxml2::XMLElement *elm, const char *key, std::string *val);
	bool checkVector3Attribute(tinyxml2::XMLElement *elm, const char *key, Vector3 *val);
	bool checkColorAttribute(tinyxml2::XMLElement *elm, const char *key, Color *val);
	
	//
	MaterialRef parseSingleBSDFMaterial(tinyxml2::XMLElement *elm);
};

/////
// implementation

XMLSceneLoader::XmlParser::XmlParser() {}
XMLSceneLoader::XmlParser::~XmlParser() {}

bool XMLSceneLoader::XmlParser::load(std::string& filename, XMLSceneLoader *xmlloader) {
	tinyxml2::XMLError xmlerr;
	tinyxml2::XMLDocument doc;
	
	std::cout << "start load xml:" << filename << std::endl;
	
	loader = xmlloader;
	
	xmlerr = doc.LoadFile(filename.c_str());
	if(xmlerr != tinyxml2::XML_SUCCESS) {
		doc.PrintError();
		return false;
	}
	
	tinyxml2::XMLElement *rootelm = doc.RootElement();
	if(!rootelm) {
		std::cerr << "root element not found" << std::endl;
		return false;
	}
	if(strcmp(rootelm->Name(), "scene") != 0) {
		std::cerr << "root element is not <scene>. illigal format." << std::endl;
		return false;
	}
	
	bool ret = true;
	tinyxml2::XMLElement *tmpelm = rootelm->FirstChildElement();
	while(tmpelm) {
		bool res;
		const char *elmname = tmpelm->Name();
		
		if(strcmp(elmname, "object") == 0) {
			res = parseObject(tmpelm);
		} else if(strcmp(elmname, "renderer") == 0) {
			res = parseRenderConfig(tmpelm);
		} else if(strcmp(elmname, "library") == 0) {
			res = parseAssetLibrary(tmpelm);
		} else if(strcmp(elmname, "camera") == 0) {
			res = parseCamera(tmpelm);
		} else if(strcmp(elmname, "sky") == 0) {
			res = parseSky(tmpelm);
		} else {
			std::cout << "unknown element <" << elmname << "> found. skip it." << std::endl;
			res = true;
		}
		
		if(!res) {
			std::cout << "parse error" << std::endl;
			ret = false;
			break;
		}
		tmpelm = tmpelm->NextSiblingElement();
	}
	
	std::cout << "done" << std::endl;
	
	return ret;
}

bool XMLSceneLoader::XmlParser::parseRenderConfig(tinyxml2::XMLElement *elm) {
	std::cout << "parseRenderConfig" << std::endl;
	
	Renderer *rndr = loader->getRederer();
	Renderer::Config conf = rndr->getConfig();
	
	checkIntAttribute(elm, "width", &conf.width);
	checkIntAttribute(elm, "height", &conf.height);
	checkIntAttribute(elm, "samples", &conf.samples);
	checkIntAttribute(elm, "subSamples", &conf.subSamples);
	checkIntAttribute(elm, "minDepth", &conf.minDepth);
	checkIntAttribute(elm, "maxDepth", &conf.maxDepth);
	checkIntAttribute(elm, "tileSize", &conf.tileSize);
	checkIntAttribute(elm, "threads", &conf.defaultThreads);
	checkStringAttribute(elm, "output", &conf.outputFile);
	
	rndr->setConfig(conf);
	
	std::cout << "done" << std::endl;
	
	return true;
}
bool XMLSceneLoader::XmlParser::parseAssetLibrary(tinyxml2::XMLElement *elm) {
	std::cout << "TODO: parseAssetLibrary" << std::endl;
	
	tinyxml2::XMLElement *tmpelm = elm->FirstChildElement();
	while(tmpelm) {
		const char *elmname = tmpelm->Name();
		
		if(strcmp(elmname, "geometry") == 0) {
			GeometryRef geom = parseGeometryElement(tmpelm);
			if(geom.get() != nullptr) {
				int res = loader->registerAsset(geom->getAssetId(), geom);
				if(res != kRegistered) {
					std::cout << "Geometry " << geom->getAssetId() << " register failed:" << res << std::endl;
				}
			} else {
			}
			
		} else if(strcmp(elmname, "texture") == 0) {
			TextureRef tex = parseTextureElement(tmpelm);
			if(tex.get() != nullptr) {
				int res = loader->registerAsset(tex->getAssetId(), tex);
				if(res != kRegistered) {
					std::cout << "Texture " << tex->getAssetId() << " register failed:" << res << std::endl;
				}
			}
			
		} else if(strcmp(elmname, "material") == 0) {
			MaterialRef mat = parseMaterialElement(tmpelm);
			if(mat.get() != nullptr) {
				int res = loader->registerAsset(mat->getAssetId(), mat);
				if(res != kRegistered) {
					std::cout << "Material " << mat->getAssetId() << " register failed:" << res << std::endl;
				}
			}
			
		} else {
			std::cout << "unknow library entry <" << elmname << ">" << std::endl;
		}
		
		tmpelm = tmpelm->NextSiblingElement();
	}
	
	return true;
}
bool XMLSceneLoader::XmlParser::parseCamera(tinyxml2::XMLElement *elm) {
	std::cout << "parseCamera" << std::endl;
	
	Scene *scn = loader->getScene();
	Camera *cam = scn->getCamera();
	
	tinyxml2::XMLElement *tmpelm = elm->FirstChildElement();
	while(tmpelm) {
		const char *elmname = tmpelm->Name();
		
		if(strcmp(elmname, "transform") == 0) {
			Matrix4 mat = parseTransformElement(tmpelm);
			cam->setTransform(mat);
			
		} else if(strcmp(elmname, "lens") == 0) {
			R1hFPType fl, fn;
			if(checkFloatAttribute(tmpelm, "focalLength", &fl)) {
				cam->setFocalLength(fl);
			}
			if(checkFloatAttribute(tmpelm, "fNumber", &fn)) {
				cam->setFNumber(fn);
			}
			
		} else if(strcmp(elmname, "sensor") == 0) {
			R1hFPType w, h, a;
			int flag = 0;
			if(checkFloatAttribute(tmpelm, "width", &w)) {
				flag |= 1;
			}
			if(checkFloatAttribute(tmpelm, "height", &h)) {
				flag |= 2;
			}
			if(checkFloatAttribute(tmpelm, "aspect", &a)) {
				flag |= 4;
			}
			
			if((flag & 3) == 3) {
				cam->setSensorSize(w, h);
			} else if((flag & 5) == 5) {
				cam->setSensorWidthWithAspect(w, a);
			} else if((flag & 6) == 6) {
				w = h * a;
				cam->setSensorSize(w, h);
			} else {
				std::cout << "<sensor> seting is illegal. needs 2 of w, h and aspect." << std::endl;
			}
			
		}
		
		tmpelm = tmpelm->NextSiblingElement();
	}
	std::cout << "done" << std::endl;
	return true;
}
bool XMLSceneLoader::XmlParser::parseSky(tinyxml2::XMLElement *elm) {
	std::cout << "parseSky" << std::endl;
	Scene *scn = loader->getScene();
	SkyMaterialRef mat = SkyMaterialRef(new SkyMaterial());
	
	tinyxml2::XMLElement *tmpelm = elm->FirstChildElement();
	while(tmpelm) {
		const char *elmname = tmpelm->Name();
		
		if(strcmp(elmname, "color") == 0) {
			Color col = parseColorElement(tmpelm);
			mat->setColor(col);
			
		} else if(strcmp(elmname, "texture") == 0) {
			TextureRef tex = parseTextureElement(tmpelm);
			mat->setTexture(tex);
			
		} else if(strcmp(elmname, "transform") == 0) {
			Matrix4 trans = parseTransformElement(tmpelm);
			mat->setTransform(trans);
			
		} else {
			std::cout << "unknown element <" << elmname << "> found" << std::endl;
		}
		
		tmpelm = tmpelm->NextSiblingElement();
	}
	std::cout << "done" << std::endl;
	
	// noproblem. attach to scene.
	scn->setSkyMaterial(mat);
	
	return true;
}
bool XMLSceneLoader::XmlParser::parseObject(tinyxml2::XMLElement *elm) {
	std::cout << "TODO: parseObject" << std::endl;
	Scene *scn = loader->getScene();
	SceneObject *scnobj = new SceneObject();
	
	tinyxml2::XMLElement *tmpelm = elm->FirstChildElement();
	while(tmpelm) {
		const char *elmname = tmpelm->Name();
		
		if(strcmp(elmname, "transform") == 0) {
			std::cout << " transform found" << std::endl;
			Matrix4 mat = parseTransformElement(tmpelm);
			scnobj->setTransform(mat);
			
		} else if(strcmp(elmname, "geometry") == 0) {
			GeometryRef geomref = parseGeometryElement(tmpelm);
			if(geomref.get() != nullptr) {
				scnobj->setGeometry(geomref);
			}
			
		} else if(strcmp(elmname, "material") == 0) {
			MaterialRef matref = parseMaterialElement(tmpelm);
			if(matref.get() != nullptr) {
				scnobj->addMaterial(matref);
			}
			
		} else {
			std::cout << "unknown element <" << elmname << "> found" << std::endl;
		}
		
		tmpelm = tmpelm->NextSiblingElement();
	}
	
	if(scnobj != nullptr) {
		scn->addObject(SceneObjectRef(scnobj));
	}
	
	return true;
}

TextureRef XMLSceneLoader::XmlParser::parseTextureElement(tinyxml2::XMLElement *elm) {
	TextureRef ret;
	std::string refid;
	if(checkStringAttribute(elm, "ref", &refid)) {
		// TODO: stub is not allowed
		//ret = new StubTexture(refid);
		//std::cout << "StubTexture:" << refid << std::endl;
		
		ret = loader->findTextureAsset(refid);
		
	} else {
		// others
		// TODO:
		std::cout << "TODO parseTextureElement when not stub." << std::endl;
		
		ret = TextureRef(nullptr);
	}
	//std::cout << "TODO parseTextureElement" << std::endl;
	return ret;
}

MaterialRef XMLSceneLoader::XmlParser::parseMaterialElement(tinyxml2::XMLElement *elm) {
	MaterialRef ret = nullptr;
	std::string refid;
	if(checkStringAttribute(elm, "ref", &refid)) {
		// TODO: stub is not allowed
		//ret = new StubMaterial(refid);
		//std::cout << "StubMaterial:" << refid << std::endl;
		ret = loader->findMaterialAsset(refid);
		
	} else {
		//std::cout << "TODO parseMaterialElement when not stub." << std::endl;
		std::string mattype;
		if(checkStringAttribute(elm, "type", &mattype)) {
			if(mattype.compare("singlebsdf") == 0) {
				ret = parseSingleBSDFMaterial(elm);
			}
		} else {
			std::cerr << "illegal material" << std::endl;
			assert(false);
		}
	}
	//std::cout << "TODO parseMaterialElement" << std::endl;
	return ret;
}

GeometryRef XMLSceneLoader::XmlParser::parseGeometryElement(tinyxml2::XMLElement *elm) {
	GeometryRef ret;
	std::string refid;
	
	if(checkStringAttribute(elm, "ref", &refid)) {
		// TODO: stub is not allowed
		//ret = new StubGeometry(refid);
		//std::cout << "StubGeometry:" << refid << std::endl;
		
		ret = loader->findGeometryAsset(refid);
		
	} else {
		//std::cout << "TODO parseGeometryElement when not stub." << std::endl;
		
		bool isok;
		std::string geomtype;
		isok = checkStringAttribute(elm, "type", &geomtype);
		assert(isok);
		
		if(geomtype.compare("mesh") == 0) {
			// TODO:parse mesh geometry. obj mesh only now.
			std::cout << "parse mesh geometry" << std::endl;
			
			std::string objname;
			assert(checkStringAttribute(elm, "src", &objname));
			
			XMLSceneObjLoader objloader(objname, loader->getScene());
			objloader.setBasePath(loader->getBasePath());
			objloader.load();
			
			ret = objloader.getSceneObject()->getGeometryRef();
			
		} else if(geomtype.compare("sphere") == 0) {
			std::cout << "parse sphere geometry" << std::endl;
			R1hFPType radius = 1.0;
			Vector3 pos(0.0);
			checkFloatAttribute(elm, "radius", &radius);
			checkVector3Attribute(elm, "position", &pos);
			
			Sphere *sphere = new Sphere(radius, pos);
			ret = GeometryRef(sphere);
			
		} else if(geomtype.compare("cube") == 0) {
			std::cout << "parse cube geometry" << std::endl;
			Vector3 size;
			Vector3 pos;
			checkVector3Attribute(elm, "size", &size);
			checkVector3Attribute(elm, "position", &pos);
			
			Vector3 minp = pos - size * 0.5;
			Vector3 maxp = pos + size * 0.5;
			
			AABBGeometry *abgeom = new AABBGeometry(minp, maxp);
			ret = GeometryRef(abgeom);
			
		} else {
			assert(false);
		}
	}
	
	return ret;
}

Vector3 XMLSceneLoader::XmlParser::parseVector3Element(tinyxml2::XMLElement *elm) {
	Vector3 ret;
	//std::cout << "parseVector3Element" << std::endl;
	checkVector3Attribute(elm, "value", &ret);
	return ret;
}
Color XMLSceneLoader::XmlParser::parseColorElement(tinyxml2::XMLElement *elm) {
	Color ret;
	//std::cout << "TODO parseColorElement" << std::endl;
	// TODO: RGB only
	checkColorAttribute(elm, "value", &ret);
	return ret;
}
Matrix4 XMLSceneLoader::XmlParser::parseMatrix4Element(tinyxml2::XMLElement *elm) {
	Matrix4 ret;
	std::cout << "TODO parseMatrix4Element" << std::endl;
	return ret;
}

Matrix4 XMLSceneLoader::XmlParser::parseTransformElement(tinyxml2::XMLElement *elm) {
	Matrix4 mat;
	
	tinyxml2::XMLElement *tmpelm = elm->FirstChildElement();
	while(tmpelm) {
		const char *elmname = tmpelm->Name();
		Matrix4 tmpmat;
		
		if(strcmp(elmname, "translate") == 0) {
			Vector3 trans;
			if(checkVector3Attribute(tmpelm, "value", &trans)) {
				mat.translate(trans);
			}
			
		} else if(strcmp(elmname, "rotate") == 0) {
			Vector3 axis;
			R1hFPType angle = 0.0;
			bool res;
			res = checkVector3Attribute(tmpelm, "axis", &axis);
			if(res) {
				res = checkFloatAttribute(tmpelm, "angle", &angle);
			}
			if(res) {
				axis.normalize();
				mat.rotate(angle, axis);
			}
			
		} else if(strcmp(elmname, "scale") == 0) {
			Vector3 scl;
			if(checkVector3Attribute(tmpelm, "value", &scl)) {
				mat.scale(scl);
			}
			
		} else if(strcmp(elmname, "lookat") == 0) {
			Vector3 pos, up, lookat;
			bool res;
			res = checkVector3Attribute(tmpelm, "position", &pos);
			if(res) {
				res = checkVector3Attribute(tmpelm, "up", &up);
			}
			if(res) {
				res = checkVector3Attribute(tmpelm, "lookat", &lookat);
			}
			if(res) {
				Matrix4 lookmat = Matrix4::makeLookAt(pos.x, pos.y, pos.z, lookat.x, lookat.y, lookat.z, up.x, up.y, up.z);
				mat = mat * lookmat;
			}
		} else {
			std::cout << "unknown transform element: <" << elmname << ">" << std::endl;
		}
		
		tmpelm = tmpelm->NextSiblingElement();
	}
	
	return mat;
}

bool XMLSceneLoader::XmlParser::checkIntAttribute(tinyxml2::XMLElement *elm, const char *key, int *val) {
	tinyxml2::XMLError err;
	int tmpval;
	err = elm->QueryIntAttribute(key, &tmpval);
	if(err != tinyxml2::XML_SUCCESS) {
		//std::cout << "illegal int attribute for " << key << std::endl;
		return false;
	}
	*val = tmpval;
	return true;
}
bool XMLSceneLoader::XmlParser::checkFloatAttribute(tinyxml2::XMLElement *elm, const char *key, R1hFPType *val) {
	tinyxml2::XMLError err;
	double tmpval;
	err = elm->QueryDoubleAttribute(key, &tmpval);
	if(err != tinyxml2::XML_SUCCESS) {
		//std::cout << "illegal double attribute for " << key << std::endl;
		return false;
	}
	*val = R1hFPType(tmpval);
	return true;
}
bool XMLSceneLoader::XmlParser::checkStringAttribute(tinyxml2::XMLElement *elm, const char *key, std::string *val) {
	const char *attrval = elm->Attribute(key);
	if(!attrval) {
		//std::cout << "illegal attribute for " << key << std::endl;
		return false;
	}
	*val = attrval;
	return true;
}
bool XMLSceneLoader::XmlParser::checkVector3Attribute(tinyxml2::XMLElement *elm, const char *key, Vector3 *val) {
	std::string tmpstr;
	if(!checkStringAttribute(elm, key, &tmpstr)) {
		// bad value
		return false;
	}
	Vector3 v;
	char c;
	std::istringstream iss(tmpstr);
	iss >> v.x >> c >> v.y >> c >> v.z;
	if(iss.fail()) {
		//std::cout << "illegal Vector3 format for " << key << ":" << tmpstr << std::endl;
		return false;
	}
	*val = v;
	return true;
}
bool XMLSceneLoader::XmlParser::checkColorAttribute(tinyxml2::XMLElement *elm, const char *key, Color *val) {
	std::string tmpstr;
	if(!checkStringAttribute(elm, key, &tmpstr)) {
		// bad value
		return false;
	}
	Color col;
	char c;
	std::istringstream iss(tmpstr);
	iss >> col.r >> c >> col.g >> c >> col.b;
	if(iss.fail()) {
		//std::cout << "illegal Color format for " << key << ":" << tmpstr << std::endl;
		return false;
	}
	*val = col;
	return true;
}

/////
MaterialRef XMLSceneLoader::XmlParser::parseSingleBSDFMaterial(tinyxml2::XMLElement *elm) {
	std::cout << "parseSingleBSDFMaterial" << std::endl;
	SingleBSDFMaterial *mat = new SingleBSDFMaterial();
	
	tinyxml2::XMLElement *tmpelm = elm->FirstChildElement();
	while(tmpelm) {
		const char *elmname = tmpelm->Name();
		
		if(strcmp(elmname, "bsdf") == 0) {
			std::string bsdftype;
			assert(checkStringAttribute(tmpelm, "type", &bsdftype));
			if(bsdftype.compare("diffuse") == 0) {
				mat->setBSDF(BSDFRef(new DiffuseBSDF()));
				
			} else if(bsdftype.compare("refraction") == 0) {
				RefractionBSDF *tmpbsdf = new RefractionBSDF();
				R1hFPType ior;
				if(checkFloatAttribute(tmpelm, "ior", &ior)) {
					tmpbsdf->setIor(ior);
				}
				mat->setBSDF(BSDFRef(tmpbsdf));
				
			} else if(bsdftype.compare("specular") == 0) {
				mat->setBSDF(BSDFRef(new SpecularBSDF()));
				
			} else {
				std::cerr << "illegal BSDF:" << bsdftype << std::endl;
				assert(false);
			}
			
		} else if(strcmp(elmname, "reflectance") == 0) {
			Color tmpcol;
			if(checkColorAttribute(tmpelm, "color", &tmpcol)) {
				mat->setReflectanceColor(tmpcol);
			} else {
				// texture
				tinyxml2::XMLElement *texelm = tmpelm->FirstChildElement();
				TextureRef tex = parseTextureElement(texelm);
				assert(tex.get());
				mat->setReflectanceTexture(tex);
			}
			
		} else if(strcmp(elmname, "emittance") == 0) {
			Color tmpcol;
			if(checkColorAttribute(tmpelm, "color", &tmpcol)) {
				mat->setEmittanceColor(tmpcol);
			} else {
				// texture
				tinyxml2::XMLElement *texelm = tmpelm->FirstChildElement();
				TextureRef tex = parseTextureElement(texelm);
				assert(tex.get());
				mat->setEmittanceTexture(tex);
			}
		}
		
		tmpelm = tmpelm->NextSiblingElement();
	}
	return MaterialRef(mat);
}

//////////
XMLSceneLoader::XMLSceneLoader():
	parser(new XmlParser())
{}

XMLSceneLoader::~XMLSceneLoader() {
	delete parser;
}

bool XMLSceneLoader::load(std::string&  xmlpath, Scene *scn, Renderer *rndr)
{
	bool ret;
	scene = scn;
	renderer = rndr;
	
	basepath = SceneSupport::getBasePath(xmlpath, &filename);
	
	std::cout << basepath << " | " << filename << std::endl;
	
	ret = parser->load(xmlpath, this);
	/*
	if(ret) {
		ret = resolveReference();
	}
	*/
	return ret;
}

Scene* XMLSceneLoader::getScene() {
	return scene;
}
Renderer* XMLSceneLoader::getRederer() {
	return renderer;
}

std::string XMLSceneLoader::getBasePath() const {
	return basepath;
}

int XMLSceneLoader::registerAsset(std::string key, GeometryRef geom) {
	std::map<std::string, GeometryRef>::iterator ite = assets.geometries.find(key);
	if(ite != assets.geometries.end()) {
		return kAlreadyRegistered;
	}
	if(geom == nullptr) {
		return kNullItem;
	}
	assets.geometries[key] = geom;
	return kRegistered;
}
int XMLSceneLoader::registerAsset(std::string key, MaterialRef mat) {
	std::map<std::string, MaterialRef>::iterator ite = assets.materials.find(key);
	if(ite != assets.materials.end()) {
		return kAlreadyRegistered;
	}
	if(mat == nullptr) {
		return kNullItem;
	}
	assets.materials[key] = mat;
	return kRegistered;
}
int XMLSceneLoader::registerAsset(std::string key, TextureRef tex) {
	std::map<std::string, TextureRef>::iterator ite = assets.textures.find(key);
	if(ite != assets.textures.end()) {
		return kAlreadyRegistered;
	}
	if(tex == nullptr) {
		return kNullItem;
	}
	assets.textures[key] = tex;
	return kRegistered;
}

GeometryRef XMLSceneLoader::findGeometryAsset(std::string key) {
	std::map<std::string, GeometryRef>::iterator ite = assets.geometries.find(key);
	if(ite == assets.geometries.end()) {
		return nullptr;
	}
	return ite->second;
}
MaterialRef XMLSceneLoader::findMaterialAsset(std::string key) {
	std::map<std::string, MaterialRef>::iterator ite = assets.materials.find(key);
	if(ite == assets.materials.end()) {
		return nullptr;
	}
	return ite->second;
}
TextureRef XMLSceneLoader::findTextureAsset(std::string key) {
	std::map<std::string, TextureRef>::iterator ite = assets.textures.find(key);
	if(ite == assets.textures.end()) {
		return nullptr;
	}
	return ite->second;
}

/*
/////
bool XMLSceneLoader::resolveReference() {
	
	// library
	
	
	// objects
	
	
	return true;
}
*/

