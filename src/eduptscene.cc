
#include "eduptscene.h"
#include "sphere.h"
#include "sceneobject.h"
#include "mesh.h"
#include "material.h"

using namespace r1h;

enum ReflectionType {
	DIFFUSE,
	SPECULAR,
	REFRACTION,
	PAINT,
	BACKGROUND
};

/// scene
bool EduptScene::load(Scene *scene, double aspect) {
	
	struct SphereDef {
		R1hFPType r;
		Vector3 pos;
		Color emittion;
		Color diffuse;
		ReflectionType type;
	} spheres[] = {
		
		{1e5, Vector3( 1e5 + 1.0, 40.8, 81.6),    Color(), Color(0.75, 0.25, 0.25), DIFFUSE},
		{1e5, Vector3(-1e5 + 99.0, 40.8, 81.6),   Color(), Color(0.25, 0.25, 0.75), DIFFUSE},
		{1e5, Vector3(50.0, 40.8, 1e5),           Color(), Color(0.75, 0.75, 0.75), DIFFUSE},
		{1e5, Vector3(50.0, 40.8, 1e5 + 250.0),   Color(), Color(0.0),              DIFFUSE},
		{1e5, Vector3(50.0, 1e5, 81.6),           Color(), Color(0.75, 0.75, 0.75), DIFFUSE},
		{1e5, Vector3(50.0, 1e5 + 81.6, 81.6),    Color(), Color(0.75, 0.75, 0.75), DIFFUSE},
		{20.0, Vector3(65.0, 20.0, 20.0),         Color(), Color(0.25, 0.75, 0.25), DIFFUSE},
		{16.5, Vector3(27.0, 16.5, 47.0),         Color(), Color(0.99, 0.99, 0.99), SPECULAR},
		{16.5, Vector3(77.0, 16.5, 78.0),         Color(), Color(0.99, 0.99, 0.99), REFRACTION},
		{15.0, Vector3(50.0, 90.0, 81.6),         Color(36.0, 36.0, 36.0), Color(), DIFFUSE}
		/*
        {1e5, Vector3( 1e5 + 1.0, 40.8, 81.6),    Color(), Color(0.75, 0.25, 0.25), DIFFUSE},
		{1e5, Vector3(-1e5 + 99.0, 40.8, 81.6),   Color(), Color(0.25, 0.25, 0.75), DIFFUSE},
		{1e5, Vector3(50.0, 40.8, 1e5),           Color(), Color(0.75, 0.75, 0.75), DIFFUSE},
		{1e5, Vector3(50.0, 40.8, 1e5 + 250.0),   Color(), Color(0.0),              DIFFUSE},
		{1e5, Vector3(50.0, 1e5, 81.6),           Color(), Color(0.75, 0.75, 0.75), DIFFUSE},
		{1e5, Vector3(50.0, 1e5 + 81.6, 81.6),    Color(), Color(0.75, 0.75, 0.75), DIFFUSE},
		{20.0, Vector3(65.0, 20.0, 20.0),         Color(), Color(0.25, 0.75, 0.25), DIFFUSE},
		{16.5, Vector3(27.0, 16.5, 47.0),         Color(), Color(0.75, 0.75, 0.25), PAINT},
		{16.5, Vector3(77.0, 16.5, 78.0),         Color(), Color(0.25, 0.75, 0.75), PAINT},
		{15.0, Vector3(50.0, 90.0, 81.6),         Color(36.0, 36.0, 36.0), Color(), DIFFUSE}
        */
	};
	int numspheres = sizeof(spheres) /sizeof(SphereDef);
	
	for(int i = 0; i < numspheres; i++) {
		SphereDef &sphrdef = spheres[i];
		
		SceneObject *obj = new SceneObject();
		
		Sphere *sphere = new Sphere(sphrdef.r, sphrdef.pos);
		obj->setGeometry(GeometryRef(sphere));
		
		SingleBSDFMaterial *mat = new SingleBSDFMaterial();
		switch(sphrdef.type) {
			case DIFFUSE:
				mat->setBSDF(BSDFRef(new DiffuseBSDF()));
				break;
			case SPECULAR:
				mat->setBSDF(BSDFRef(new SpecularBSDF()));
				break;
			case REFRACTION:
				mat->setBSDF(BSDFRef(new RefractionBSDF()));
				break;
			case PAINT:
				mat->setBSDF(BSDFRef(new PaintBSDF()));
				break;
			case BACKGROUND:
				mat = nullptr;
				break;
		}
		mat->setReflectanceColor(sphrdef.diffuse);
		mat->setEmittanceColor(sphrdef.emittion);
		obj->addMaterial(MaterialRef(mat));
		
		scene->addObject(SceneObjectRef(obj));
	}
	/*
	/////
	// triangle
	{
		SceneObjectRef objref(new SceneObject());
		Mesh *mesh = new Mesh(3, 1);
		
		mesh->addVertexWithAttrs(Vector3(21.0, 38.0, 81.5), Vector3(0.0, 1.0, 0.0));
		mesh->addVertexWithAttrs(Vector3(34.0, 16.0, 100.0), Vector3(0.0, 1.0, 0.0));
		mesh->addVertexWithAttrs(Vector3(48.0, 33.5, 70.0), Vector3(0.0, 1.0, 0.0));
		mesh->addVertexWithAttrs(Vector3(43.0, 5.5, 73.0), Vector3(0.0, 1.0, 0.0));
		
		mesh->addVertexWithAttrs(Vector3(11.0, 38.0, 81.5), Vector3(0.0, 1.0, 0.0));
		mesh->addVertexWithAttrs(Vector3(24.0, 16.0, 100.0), Vector3(0.0, 1.0, 0.0));
		mesh->addVertexWithAttrs(Vector3(38.0, 33.5, 70.0), Vector3(0.0, 1.0, 0.0));
		mesh->addVertexWithAttrs(Vector3(33.0, 5.5, 73.0), Vector3(0.0, 1.0, 0.0));
		
		mesh->addFace(0, 1, 2);
		mesh->addFace(3, 1, 2);
		
		mesh->addFace(4, 5, 6);
		mesh->addFace(7, 5, 6);
		
		mesh->calcSmoothNormals();
		mesh->buildBVH();
		
		objref->setGeometry(GeometryRef(mesh));
		objref->addMaterial(MaterialRef(new EduptMaterial(Color(0.25, 0.75, 0.25), Color(), DIFFUSE)));
		scene->addObject(objref);
	}
	/////
	*/
	
	SkyMaterial *bgmat = new SkyMaterial();
	bgmat->setColor(Color(0.5));
	scene->setSkyMaterial(SkyMaterialRef(bgmat));
	
	Camera *camera = new Camera();
	camera->setLookat(
					  Vector3(50.0, 52.0, 220.0),
					  Vector3(50.0, 52.0, 220.0 - 1.0),
					  Vector3(0.0, 1.0, 0.0)
	);
	camera->setFocalLength(28.0);
 	camera->setSensorWidthWithAspect(36.0, aspect);
	scene->setCamera(CameraRef(camera));
	
	return true;
}

bool EduptScene::load2(Scene *scene, double aspect) {
	// DOF test scene
	struct SphereDef {
		R1hFPType r;
		Vector3 pos;
		Color emittion;
		Color diffuse;
		ReflectionType type;
	} spheres[] = {
		{10.0, Vector3(-15.0, 0.0, 20.0),         Color(), Color(0.0 , 0.75, 0.25), DIFFUSE},
		{10.0, Vector3( 15.0, 0.0, 20.0),         Color(), Color(0.5 , 0.75, 0.5 ), DIFFUSE},
		{10.0, Vector3(-15.0, 0.0, 40.0),         Color(), Color(0.75, 0.0 , 0.25), DIFFUSE},
		{10.0, Vector3( 15.0, 0.0, 40.0),         Color(), Color(0.75, 0.5 , 0.5 ), DIFFUSE},
		{10.0, Vector3(-15.0, 0.0, 60.0),         Color(), Color(0.0 , 0.25, 0.75), DIFFUSE},
		{10.0, Vector3( 15.0, 0.0, 60.0),         Color(), Color(0.5 , 0.5 , 0.75), DIFFUSE},
		{10.0, Vector3(-15.0, 0.0, 80.0),         Color(), Color(0.75, 0.75, 0.0 ), DIFFUSE},
		{10.0, Vector3( 15.0, 0.0, 80.0),         Color(), Color(0.75, 0.5 , 0.5 ), DIFFUSE},

	};
	int numspheres = sizeof(spheres) /sizeof(SphereDef);
	
	// spheres
	for(int i = 0; i < numspheres; i++) {
		SphereDef &sphrdef = spheres[i];
		
		SceneObject *obj = new SceneObject();
		
		//Sphere *sphere = new Sphere(sphrdef.r, sphrdef.pos);
		Sphere *sphere = new Sphere(sphrdef.r, Vector3(0.0));
		obj->setGeometry(GeometryRef(sphere));
		
		Matrix4 m;
		m.translate(sphrdef.pos);
		//m.scale(1.0, 0.5, 1.0);
		//m.rotate(0.7, 0.0, 0.0, 1.0);
		obj->setTransform(m);
		
		SingleBSDFMaterial *mat = new SingleBSDFMaterial();
		switch(sphrdef.type) {
			case DIFFUSE:
				mat->setBSDF(BSDFRef(new DiffuseBSDF()));
				break;
			case SPECULAR:
				mat->setBSDF(BSDFRef(new SpecularBSDF()));
				break;
			case REFRACTION:
				mat->setBSDF(BSDFRef(new RefractionBSDF()));
				break;
			case PAINT:
				mat->setBSDF(BSDFRef(new PaintBSDF()));
				break;
			case BACKGROUND:
				mat = nullptr;
				break;
		}
		mat->setReflectanceColor(sphrdef.diffuse);
		mat->setEmittanceColor(sphrdef.emittion);
		obj->addMaterial(MaterialRef(mat));
		
		scene->addObject(SceneObjectRef(obj));
	}
	
	// bg
	SkyMaterial *bgmat = new SkyMaterial();
	bgmat->setColor(Color(0.5));
	scene->setSkyMaterial(SkyMaterialRef(bgmat));
	
	// camera
	Camera *camera = new Camera();
	camera->setLookat(
					  Vector3(0.0, 10.0, 0.0),
					  Vector3(0.0, 0.0, 40.0),
					  Vector3(0.0, 1.0, 0.0)
					  );
	camera->setFocalLength(28.0);
 	camera->setSensorWidthWithAspect(36.0, aspect);
	camera->setFocusDistance(1.0);
	camera->setFNumber(1.0);
	scene->setCamera(CameraRef(camera));
	
	return true;
}
