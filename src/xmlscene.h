#ifndef R1H_XMLSCENE_H
#define R1H_XMLSCENE_H

#include <string>
#include <map>

#include "scene.h"
#include "renderer.h"
#include "texture.h"

namespace r1h {

class XMLSceneLoader {
public:
	enum AssetRegisterResult {
		kRegistered,
		kAlreadyRegistered,
		kNullItem
	};
	
public:
    XMLSceneLoader();
    ~XMLSceneLoader();
    
    bool load(std::string& xmlpath, Scene *scn, Renderer *rndr);
	
	Scene* getScene();
	Renderer* getRederer();
	
	std::string getBasePath() const;
	
	int registerAsset(std::string key, GeometryRef geom);
	int registerAsset(std::string key, MaterialRef mat);
	int registerAsset(std::string key, TextureRef tex);
	
	GeometryRef findGeometryAsset(std::string key);
	MaterialRef findMaterialAsset(std::string key);
	TextureRef findTextureAsset(std::string key);
	
private:
    std::string basepath;
    std::string filename;
    Scene *scene;
    Renderer *renderer;
    
    struct Library {
        std::map<std::string, GeometryRef> geometries;
        std::map<std::string, MaterialRef> materials;
		std::map<std::string, TextureRef> textures;
    } assets;
	
	class XMLParser;
	XMLParser* parser;
	
	//bool resolveReference();
};

}

#endif