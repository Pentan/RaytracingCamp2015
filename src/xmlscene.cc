#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstring>

#include "camera.h"
#include "xmlscene.h"
#include "scenesupport.h"

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
	
	Texture* parseTextureElement(tinyxml2::XMLElement *elm);
	Material* parseMaterialElement(tinyxml2::XMLElement *elm);
	Geometry* parseGeometryElement(tinyxml2::XMLElement *elm);
	
	bool checkIntAttribute(tinyxml2::XMLElement *elm, const char *key, int *val);
	bool checkFloatAttribute(tinyxml2::XMLElement *elm, const char *key, R1hFPType *val);
	bool checkStringAttribute(tinyxml2::XMLElement *elm, const char *key, std::string *val);
	bool checkVector3Attribute(tinyxml2::XMLElement *elm, const char *key, Vector3 *val);
	bool checkColorAttribute(tinyxml2::XMLElement *elm, const char *key, Color *val);
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
			Geometry *geom = parseGeometryElement(tmpelm);
			if(geom) {
				int res = loader->registerAsset(geom->getAssetId(), GeometryRef(geom));
				if(res != kRegistered) {
					std::cout << "Geometry " << geom->getAssetId() << " register failed:" << res << std::endl;
				}
			}
			
		} else if(strcmp(elmname, "texture") == 0) {
			Texture *tex = parseTextureElement(tmpelm);
			if(tex) {
				int res = loader->registerAsset(tex->getAssetId(), TextureRef(tex));
				if(res != kRegistered) {
					std::cout << "Texture " << tex->getAssetId() << " register failed:" << res << std::endl;
				}
			}
			
		} else if(strcmp(elmname, "material") == 0) {
			Material *mat = parseMaterialElement(tmpelm);
			if(mat) {
				int res = loader->registerAsset(mat->getAssetId(), MaterialRef(mat));
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
	std::cout << "TODO: parseCamera" << std::endl;
	
	Scene *scn = loader->getScene();
	Camera *cam = scn->getCamera();
	
	tinyxml2::XMLElement *tmpelm = elm->FirstChildElement();
	while(tmpelm) {
		const char *elmname = tmpelm->Name();
		
		if(strcmp(elmname, "transform") == 0) {
			
		} else if(strcmp(elmname, "lenz") == 0) {
			
		} else if(strcmp(elmname, "sensor") == 0) {
			
		}
		
		tmpelm = tmpelm->NextSiblingElement();
	}
	
	return true;
}
bool XMLSceneLoader::XmlParser::parseSky(tinyxml2::XMLElement *elm) {
	std::cout << "TODO: parseSky" << std::endl;
	return true;
}
bool XMLSceneLoader::XmlParser::parseObject(tinyxml2::XMLElement *elm) {
	std::cout << "TODO: parseObject" << std::endl;
	return true;
}

Texture* XMLSceneLoader::XmlParser::parseTextureElement(tinyxml2::XMLElement *elm) {
	Texture *ret = nullptr;
	std::cout << "TODO parseTextureElement" << std::endl;
	return ret;
}

Material* XMLSceneLoader::XmlParser::parseMaterialElement(tinyxml2::XMLElement *elm) {
	Material *ret = nullptr;
	std::cout << "TODO parseMaterialElement" << std::endl;
	return ret;
}

Geometry* XMLSceneLoader::XmlParser::parseGeometryElement(tinyxml2::XMLElement *elm) {
	Geometry *ret = nullptr;
	std::cout << "TODO parseGeometryElement" << std::endl;
	return ret;
}

bool XMLSceneLoader::XmlParser::checkIntAttribute(tinyxml2::XMLElement *elm, const char *key, int *val) {
	tinyxml2::XMLError err;
	int tmpval;
	err = elm->QueryIntAttribute(key, &tmpval);
	if(err != tinyxml2::XML_SUCCESS) {
		std::cout << "illegal int attribute for " << key << std::endl;
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
		std::cout << "illegal double attribute for " << key << std::endl;
		return false;
	}
	*val = R1hFPType(tmpval);
	return true;
}
bool XMLSceneLoader::XmlParser::checkStringAttribute(tinyxml2::XMLElement *elm, const char *key, std::string *val) {
	const char *attrval = elm->Attribute(key);
	if(!attrval) {
		std::cout << "illegal attribute for " << key << std::endl;
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
		std::cout << "illegal Vector3 format for " << key << ":" << tmpstr << std::endl;
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
		std::cout << "illegal Color format for " << key << ":" << tmpstr << std::endl;
		return false;
	}
	*val = col;
	return true;
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
	scene = scn;
	renderer = rndr;
	
	basepath = SceneSupport::getBasePath(xmlpath, &filename);
	
	std::cout << basepath << " | " << filename << std::endl;
	
	return parser->load(xmlpath, this);
}

Scene* XMLSceneLoader::getScene() {
	return scene;
}
Renderer* XMLSceneLoader::getRederer() {
	return renderer;
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


