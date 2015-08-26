
#include <iostream>
#include <algorithm>
#include "mesh.h"

using namespace r1h;

const int Mesh::kTypeID = 'MESH';

Mesh::Face::Face() : matid(-1) {}
Mesh::Face::Face(const int a, const int b, const int c, const int m) {
	AttrCoord aco = {0, a, b, c};
	v0 = n0 = a;
	v1 = n1 = b;
	v2 = n2 = c;
	attrs.push_back(aco);
	matid = m;
}

void Mesh::Face::setV(const int a, const int b, const int c) {
	v0 = a;
	v1 = b;
	v2 = c;
}
void Mesh::Face::setN(const int a, const int b, const int c) {
	n0 = a;
	n1 = b;
	n2 = c;
}
void Mesh::Face::addAttr(const int attrid, const int a, const int b, const int c) {
	AttrCoord aco = {attrid, a, b, c};
	attrs.push_back(aco);
}

/////
Mesh::Mesh(const int vreserve, const int freserv):
	Geometry(), bvhRoot(0)
{
	assetType = kTypeID;
	
	if(vreserve > 0) {
		vertices.reserve(vreserve);
		normals.reserve(vreserve);
	}
	if(freserv > 0) {
		faces.reserve(freserv);
	}
	vertex_reserved = vreserve;
	face_reserved = freserv;
}

Mesh::~Mesh() {
	if(bvhRoot) {
		delete bvhRoot;
	}
}

size_t Mesh::addVertexWithAttrs(const Vector3 &p, const Vector3 &n, const Vector3 &uv, const int uvid) {
	size_t ret = addVertex(p);
	addNormal(n);
	if(uvid >= 0) {
		addAttribute(uvid, uv);
	}
	return ret;
}
size_t Mesh::addVertex(const Vector3 &v) {
	vertices.push_back(v);
	//printf("v (%lf,%lf,%lf)\n", v.x, v.y, v.z); //+++++
	return vertices.size() - 1;
}
size_t Mesh::getVertexCount() const {
	return vertices.size();
}

size_t Mesh::addNormal(const Vector3 &v) {
	normals.push_back(v);
	//printf("vn (%lf,%lf,%lf)\n", v.x, v.y, v.z); //+++++
	return normals.size() - 1;
}
size_t Mesh::getNormalCount() const {
	return normals.size();
}

size_t Mesh::newAttributeContainer() {
	std::vector<Vector3> attrv;
	attrv.reserve(vertex_reserved);
	attributes.push_back(attrv);
	return attributes.size() - 1;
}
size_t Mesh::addAttribute(const int attrid, const Vector3 &v) {
	if(attrid < 0 || attrid > (int)attributes.size()) {
		throw "vertex attributes out of range";
	}
	attributes[attrid].push_back(v);
	//printf("vt (%lf,%lf,%lf)\n", v.x, v.y, v.z); //+++++
	return attributes[attrid].size() - 1;
}
size_t Mesh::getAttributeCount(const int attrid) const {
	if(attrid < 0 || attrid > (int)attributes.size()) {
		throw "vertex attributes out of range";
	}
	return attributes[attrid].size();
}

size_t Mesh::addFace(const Mesh::Face &f) {
	faces.push_back(f);
	//printf("f (%d/%d/%d,%d/%d/%d,%d/%d/%d)\n",
	//	   f.v0, f.attrs[0].co0, f.n0,
	//	   f.v1, f.attrs[0].co1, f.n1,
	//	   f.v2, f.attrs[0].co2, f.n2);
	return faces.size() - 1;
}
size_t Mesh::addFace(const int a, const int b, const int c, const int matid) {
	faces.push_back(Face(a, b, c, matid));
	return faces.size() - 1;
}
size_t Mesh::getFaceCount() const {
	return faces.size();
}

Vector3 Mesh::getVaryingAttr(const int faceid, const int attrid, const Vector3 weights) const {
    const Face &face = faces[faceid];
    const AttrCoord &attrco = face.attrs[attrid];
    const std::vector<Vector3> &attrvec = attributes[attrco.attrid];
    const Vector3 a0 = attrvec[attrco.co0];
    const Vector3 a1 = attrvec[attrco.co1];
    const Vector3 a2 = attrvec[attrco.co2];
    
    return a0 * weights.x + a1 * weights.y + a2 * weights.z;
}

void Mesh::postProcess() {
	// calc face tangent space, surface area and so on.
	surfArea = 0.0;
	
	for(size_t i = 0; i < faces.size(); i++) {
		Face &f = faces[i];
		// calc face normal
		const Vector3 v01 = vertices[f.v1] - vertices[f.v0];
		const Vector3 v02 = vertices[f.v2] - vertices[f.v0];
		const Vector3 vn = Vector3::cross(v01, v02);
		
		f.normal = Vector3::normalized(vn);
		f.area = vn.length();
		surfArea += f.area;
	}
}

void Mesh::calcSmoothNormals() {
    // clear normals
	if(normals.size() < vertices.size()) {
		normals.resize(vertices.size());
	}
    for(size_t i = 0; i < normals.size(); i++) {
        normals[i] = Vector3(0.0, 0.0, 0.0);
    }
    // calc face normal
    for(size_t i = 0; i < faces.size(); i++) {
        const Face &f = faces[i];
        const Vector3 v01 = vertices[f.v1] - vertices[f.v0];
        const Vector3 v02 = vertices[f.v2] - vertices[f.v0];
        const Vector3 nv = Vector3::cross(v01, v02);
        normals[f.n0] = normals[f.n0] + nv;
        normals[f.n1] = normals[f.n1] + nv;
        normals[f.n2] = normals[f.n2] + nv;
    }
    // normalize
    for(size_t i = 0; i < normals.size(); i++) {
        normals[i] = Vector3::normalized(normals[i]);
    }
}

void Mesh::buildBVH() {
    if(bvhRoot) {
        // rebuild?
        std::cout << "WARNING: rebuild BVH ? [mesh:" << this << "]" << std::endl;
        delete bvhRoot;
        bvhRoot = 0;
    }
    
    size_t facenum = faces.size();
    //std::cout  << "faces:" << facenum << std::endl;
    
	if (facenum == 0) {
		bvhRoot = new BVHNode();
	}
	else {
		AABB *faceAABBs = new AABB[facenum];
		for (size_t i = 0; i < facenum; i++) {
			Face &fc = faces[i];
			AABB &ab = faceAABBs[i];
			ab.expand(vertices[fc.v0]);
			ab.expand(vertices[fc.v1]);
			ab.expand(vertices[fc.v2]);
			ab.dataId = int(i);
		}
		bvhRoot = new BVHNode();
		size_t maxdepth;
		maxdepth = bvhRoot->buildAABBTree(faceAABBs, (int)facenum);
		//std::cout << "max BVH depth:" << maxdepth << std::endl;

		delete[] faceAABBs;
	}
}

AABB Mesh::getAABB(const Matrix4& tm) const {
	AABB ret;
	for(size_t i = 0; i < vertices.size(); i++) {
		Vector3 p = Matrix4::transformV3(tm, vertices[i]);
		ret.expand(p);
	}
	return ret;
	//return bvhRoot->aabb;
}

//bool Mesh::triangleIntersect(const int faceid, const Ray &ray, Intersection *intersect) const {
bool Mesh::isIntersectLeaf(int dataid, const Ray &ray, Intersection *intersect) const {
	if (faces.size() <= 0) {
		return false;
	}

    const Face &face = faces[dataid];
    const Vector3 &v0 = vertices[face.v0];
    const Vector3 &v1 = vertices[face.v1];
    const Vector3 &v2 = vertices[face.v2];
    
    const Vector3 v01 = v1 - v0;
    const Vector3 v02 = v2 - v0;
    const Vector3 r = ray.origin - v0;
    const Vector3 u = Vector3::cross(ray.direction, v02);
    const Vector3 v = Vector3::cross(r, v01);
    
    const double div = 1.0 / Vector3::dot(u, v01);
    const double t = Vector3::dot(v, v02) * div;
    const double b = Vector3::dot(u, r) * div;
    const double c = Vector3::dot(v, ray.direction) * div;
    
    /*
	 printf("face(%d,%d,%d)¥n", face.v0, face.v1, face.v2);
	 v0.print("v0");
	 v1.print("v1");
	 v2.print("v2");
	 v01.print("v01");
	 v02.print("v02");
	 ray.dir_.print("ray.dir_");
	 ray.org_.print("ray.org_");
	 r.print("r");
	 u.print("u");
	 v.print("v");
	 printf("(t,b,c)=(%lf,%lf,%lf)¥n", t, b, c);//+++++
	 */
    if((b + c < 1.0 && b > 0.0 && c > 0.0) && t > 0.0) {
        // hit
        const double a = 1.0 - b - c;
        intersect->distance = t;
        intersect->position = v0 * a + v1 * b + v2 * c;
        intersect->varyingWeight.set(a, b, c);
        intersect->faceId = dataid;
        
        return true;
    }
    return false;
}

bool Mesh::isIntersect(const Ray &ray, Intersection *intersect) const {
    
#if 0
    // brute force
    Intersection nearest_isect, tmp_isect;
    for(int iface = 0; iface < faces.size(); iface++) {
        //printf("face[%d]¥n", iface);//+++++
        if(triangleIntersect(iface, ray, &tmp_isect)) {
            if(tmp_isect.distance < nearest_isect.distance) {
                nearest_isect = tmp_isect;
                nearest_isect.faceId = iface;
            }
        }
    }
    if(nearest_isect.faceId < 0) {
        return false;
    }
#else
    // traverse BVH
    Intersection nearest_isect;
	if(!BVHNode::isIntersectBVHTree(this, *bvhRoot, ray, &nearest_isect)) {
        return false;
    }
#endif
    
    // calc face infomation
    const Face &hitface = faces[nearest_isect.faceId];
	*intersect = nearest_isect;
    //intersect->distance = nearest_info.distance;
    //intersect->position = nearest_info.position;
    //intersect->faceId = nearest_info.faceid;
	//intersect->varyingWeight = Vector3(nearest_info.w0, nearest_info.w1, nearest_info.w2);
    intersect->normal =
        normals[hitface.n0] * nearest_isect.varyingWeight.x +
        normals[hitface.n1] * nearest_isect.varyingWeight.y +
        normals[hitface.n2] * nearest_isect.varyingWeight.z;
    intersect->materialId = hitface.matid;
    
    return true;
}

void Mesh::prepareRendering() {
	if(normals.size() <= 0) {
		printf("mesh [0x%p] may not has normals. generate it.\n", this);
		
		normals.resize(vertices.size());
		for(size_t i = 0; i < faces.size(); i++) {
			faces[i].n0 = faces[i].v0;
			faces[i].n1 = faces[i].v1;
			faces[i].n2 = faces[i].v2;
		}
		calcSmoothNormals();
	}
	
	if(!bvhRoot) {
		buildBVH();
	}
}

///
void Mesh::dumpFaces() const {
	int facenum = (int)faces.size();
	
	for(int i = 0; i < facenum; i++) {
		Face fc = faces[i];
		printf("f[%d]:v(%d,%d,%d),n(%d,%d,%d),t(%d,%d,%d)\n", i, fc.v0, fc.v1, fc.v2, fc.n0, fc.n1, fc.n2, fc.attrs[0].co0, fc.attrs[0].co1, fc.attrs[0].co2);
		Vector3 v0 = vertices[fc.v0];
		Vector3 v1 = vertices[fc.v1];
		Vector3 v2 = vertices[fc.v2];
		printf(" v0(%lf,%lf,%lf),v1(%lf,%lf,%lf),v2(%lf,%lf,%lf)\n", v0.x, v0.y, v0.z, v1.x, v1.y, v1.z, v2.x, v2.y, v2.z);
		Vector3 n0 = normals[fc.n0];
		Vector3 n1 = normals[fc.n1];
		Vector3 n2 = normals[fc.n2];
		printf(" n0(%lf,%lf,%lf),n1(%lf,%lf,%lf),n2(%lf,%lf,%lf)\n", n0.x, n0.y, n0.z, n1.x, n1.y, n1.z, n2.x, n2.y, n2.z);
	}
}

