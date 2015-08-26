#ifndef R1H_OBJLOADER_H
#define R1H_OBJLOADER_H

#include <string>
#include <map>

namespace r1h {

class WavefrontObj {
protected:
    std::string basepath;
    std::string filepath;
    
public:
    enum ParameterType {
        // obj
        OBJ_unknown = 0,
        OBJ_v,       //= 'v',		// vertex
        OBJ_vn,      //= 'vn',		// vertex normal
        OBJ_vt,      //= 'vt',		// vertex texcoord
        OBJ_o,       //= 'o',		// object
        OBJ_g,       //= 'g',		// group
        OBJ_s,       //= 's',		// smooth shading
        OBJ_f,       //= 'f',		// face
		OBJ_l,       //= 'l',		// line
        OBJ_mtllib,  //= 'mlib',	// outside materials
        OBJ_usemtl,  //= 'usem',	// use material
        // matl
        MTL_newmtl,  //= 'newm',	// define material
        MTL_Ns,      //= 'Ns',		// specular factor
        MTL_Ka,      //= 'Ka',		// ambient color
        MTL_Kd,      //= 'Kd',		// diffuse color
        MTL_Ks,      //= 'Ks',		// specular color
        MTL_Ni,      //= 'Ni',		// ior
        MTL_d,       //= 'd',		// transparency
        MTL_illum,   //= 'ilm',	// illimination mode
        MTL_map_Kd  //= 'mpKd'	// diffuse texture
    };
    
    enum ValueType {
        kNoneValue,
        kVectorValue,
        kStringValue,
		kFaceValue,
		kLineValue,
        kIntegerValue
    };
    
    struct ParameterDesc {
        ParameterType paramType;
        ValueType valueType;
        ParameterDesc(): paramType(OBJ_unknown), valueType(kNoneValue) {};
        ParameterDesc(const ParameterType pt, const ValueType vt): paramType(pt), valueType(vt) {};
    };
    
    
private:
    std::map<std::string, ParameterDesc> ptMap;
    bool loadFile(std::string fpath);
    
public:
    ////
    struct FaceInfo {
        int v, vt, vn;
    };
    
    WavefrontObj(std::string path);
    virtual ~WavefrontObj(){}
    
    virtual void setBasePath(std::string path);
    virtual bool load();
    
    // implement below
    // v, vn, vt, Ns, Ka, Kd, Ks, Ni, d
    virtual void foundVector(const ParameterType pt, const double x, const double y, const double z) = 0;
    // mtllib, o, s, usemtl, newmtl, MTL_map_Kd
    virtual void foundString(const ParameterType pt, const std::string &str) = 0;
    // illum
	virtual void foundInteger(const ParameterType pt, const int i) = 0;
	// l
	virtual void foundLine(const ParameterType pt, const int v0, const int v1) = 0;
    // f
    virtual void foundFace(const ParameterType pt, const std::vector<FaceInfo> &fids) = 0;
    
    // mtl
    virtual void importMTL(std::string filename); // optional
    
    // endof .mtl or .obj
    virtual void endFile(std::string fullpath) = 0;
    
    void addParameterDesc(const std::string &key, const ParameterDesc &desc) {
        ptMap[key] = desc;
    }
};

}
#endif
