#ifndef R1H_SCENE_H
#define R1H_SCENE_H

#include <vector>
#include "r1htypes.h"
#include "ray.h"
#include "renderer.h"
#include "sceneobject.h"
#include "material.h"
#include "materialsky.h"
#include "camera.h"
#include "intersection.h"
#include "bvhnode.h"

namespace r1h {

class Camera;

class Scene : public BVHLeaf {
public:
    Scene();
    ~Scene();
    
    // setup
    //bool load();
	bool loadWavefrontObj(std::string filename);
	
    bool loadXML(std::string filename, Renderer* render);
    
    int addObject(SceneObjectRef objref);
    SceneObject* getObject(int objid);
	int getObjectsCount() const;
	
    Camera* getCamera();
	void setCamera(CameraRef camref);
    
	SkyMaterial* getSkyMaterial();
	void setSkyMaterial(SkyMaterialRef matref);
	
	void prepareRendering();
	
	// render
    Color radiance(Renderer::Context *cntx, const Ray &ray);
    
	// override
	bool isIntersectLeaf(int dataid, const Ray &ray, Intersection *intersect) const;
	
private:
    CameraRef camera;
    std::vector<SceneObjectRef> sceneObjects;
    BVHNode *objectBVH;
    
    SkyMaterialRef  skyMaterial;
	
	bool intersectSceneObjects(const Ray &ray, Intersection *intersect);
};

}

#endif
