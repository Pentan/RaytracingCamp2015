/*
### Mesh data format ###
vertices[] = {Vector3, ...}
normals[] = {Vector3, ...}
attributes[] = {{Vector3, ...}, ...}
faces[] = {Face, ...}

MapCoord { id mapid, co1, co2, co3; }

Face {
    id v1, v2, v3;
    id n1, n2, n3;
    attributes[] = {MapCoord, ...}
    Material *m
}
*/

#ifndef R1H_MESH_H
#define R1H_MESH_H

#include <vector>

#include "r1htypes.h"
#include "ray.h"
#include "material.h"
#include "intersection.h"
#include "geometry.h"
#include "aabb.h"
#include "bvhnode.h"

namespace r1h {

class Mesh : public Geometry, public BVHLeaf {
public:
	
    static const int kTypeID;
    
    struct AttrCoord {
        int attrid;         // index of Geometry's attributes
        int co0, co1, co2;
    };
    
    struct Face {
        int v0, v1, v2;
        int n0, n1, n2;
        std::vector<AttrCoord> attrs;
        int matid;
		
		R1hFPType area;
		Vector3 normal;
		Vector3 tangent;
		
        Face();
        Face(const int a, const int b, const int c, const int m);
        
        void setV(const int a, const int b, const int c);
        void setN(const int a, const int b, const int c);
        void addAttr(const int attrid, const int a, const int b, const int c);
    };
    
public:
    // empty mesh
    Mesh(const int vreserve=0, const int freserv=0);
    ~Mesh();
    
    size_t addVertexWithAttrs(const Vector3 &p, const Vector3 &n, const Vector3 &uv=0, const int uvid=-1);
    size_t addVertex(const Vector3 &v);
    size_t getVertexCount() const;
	
	size_t addNormal(const Vector3 &v);
    size_t getNormalCount() const;
    
	size_t newAttributeContainer();
    size_t addAttribute(const int attrid, const Vector3 &v);
	size_t getAttributeCount(const int attrid) const;
    
    size_t addFace(const Mesh::Face &f);
    size_t addFace(const int a, const int b, const int c, const int matid=0);
    size_t getFaceCount() const;
	
    Vector3 getVaryingAttr(const int faceid, const int attrid, const Vector3 weights) const;
	
	void postProcess();
    void calcSmoothNormals();
    void buildBVH();
	
	// override
	// Geometry
	virtual AABB getAABB(const Matrix4& tm) const;
    virtual bool isIntersect(const Ray &ray, Intersection *intersect) const;
    virtual void prepareRendering();
	// BVHLeaf
	virtual bool isIntersectLeaf(int dataid, const Ray &ray, Intersection *intersect) const;
	//bool triangleIntersect(const int faceid, const Ray &ray, Intersection *intersect) const;
	
	/// for debug
	void dumpFaces() const;
	
private:
	std::vector<Vector3> vertices;
	std::vector<Vector3> normals;
	std::vector<Vector3> tangents;
    std::vector<std::vector<Vector3> > attributes;
    std::vector<Face> faces;
	R1hFPType surfArea;
	
    BVHNode *bvhRoot;
    
    int vertex_reserved;
    int face_reserved;
};

}
#endif
