#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <map>
#include <vector>
#include "wavefrontobj.h"

using namespace r1h;

/*
// vector
case OBJ_v:
    break;
case OBJ_vn:
    break;
case OBJ_vt:
    break;
case MTL_Ns:
    break;
case MTL_Ka:
    break;
case MTL_Kd:
    break;
case MTL_Ks:
    break;
case MTL_Ni:
    break;
case MTL_d:
    break;
default:

// string
case OBJ_o:
    break;
case OBJ_g:
    break;
case OBJ_s:
    break;
case OBJ_f:
    break;
case OBJ_mtllib:
    break;
case OBJ_usemtl:
    break;
case MTL_map_Kd:
    break;

// int
case MTL_illum:
        break;
*/

WavefrontObj::WavefrontObj(std::string path) {
    ptMap["v"] =        ParameterDesc(OBJ_v,  kVectorValue);
    ptMap["vn"] =       ParameterDesc(OBJ_vn, kVectorValue);
    ptMap["vt"] =       ParameterDesc(OBJ_vt, kVectorValue);
    ptMap["Ns"] =       ParameterDesc(MTL_Ns, kVectorValue);
    ptMap["Ka"] =       ParameterDesc(MTL_Ka, kVectorValue);
    ptMap["Kd"] =       ParameterDesc(MTL_Kd, kVectorValue);
    ptMap["Ks"] =       ParameterDesc(MTL_Ks, kVectorValue);
    ptMap["Ni"] =       ParameterDesc(MTL_Ni, kVectorValue);
    ptMap["d"] =        ParameterDesc(MTL_d,  kVectorValue);
    
    ptMap["o"] =        ParameterDesc(OBJ_o, kStringValue);
    ptMap["g"] =        ParameterDesc(OBJ_g, kStringValue);
    ptMap["s"] =        ParameterDesc(OBJ_s, kStringValue);
    ptMap["mtllib"] =   ParameterDesc(OBJ_mtllib, kStringValue);
    ptMap["usemtl"] =   ParameterDesc(OBJ_usemtl, kStringValue);
    ptMap["newmtl"] =   ParameterDesc(MTL_newmtl, kStringValue);
    ptMap["map_Kd"] =   ParameterDesc(MTL_map_Kd, kStringValue);

	ptMap["l"] =		ParameterDesc(OBJ_l, kLineValue);
    ptMap["f"] =        ParameterDesc(OBJ_f, kFaceValue);
    
    ptMap["illum"] =    ParameterDesc(MTL_illum, kIntegerValue);
	
	// separate filename and base path
	size_t foundpos = path.find_last_of("/\\");
	if(foundpos == std::string::npos) {
		filepath = path;
		basepath = "";
	} else {
		basepath = path.substr(0, foundpos + 1); // include separater
		filepath = path.substr(foundpos + 1);
	}
	std::cout << "(base,file):" << basepath << ", " << filepath << std::endl;
}

void WavefrontObj::setBasePath(std::string path) {
	basepath = path;
	if(basepath[basepath.length() - 1] != '/') {
		basepath.push_back('/');
	}
}

bool WavefrontObj::load() {
    return loadFile(filepath.c_str());
}

bool WavefrontObj::loadFile(std::string fpath) {
    std::ifstream *fsp = new std::ifstream(fpath);
    
    if(!fsp->is_open()) {
        //std::cout << fpath << " couldn't open" << std::endl;
        delete fsp;
        
        std::string fullpath;
		
        if(basepath.length() <= 0) {
            //std::cout << fpath << " couldn't open. no base path." << std::endl;
            //return false;
			
			std::cout << "WARNING: no base path defined for " << fpath << std::endl;
			fullpath = fpath;
        } else {
			fullpath = basepath + fpath;
		}
        fsp = new std::ifstream(fullpath.c_str());
        if(!fsp->is_open()) {
            std::cout << fpath << " or " << fullpath << " couldn't open" << std::endl;
            delete fsp;
            return false;
        }
    }
    
    //std::map<std::string, ParameterType> ptMap;
    //if(ptMap.size() <= 0) {
    //}
    
    const int BufMmax = 1024;
    char *buf = new char[1024];
    
    std::vector<FaceInfo> finfovec;
    finfovec.reserve(8);
    
    int lineCount = 0;
    while(!fsp->eof()) {
        ++lineCount;
        
        fsp->getline(buf, BufMmax);
        if(fsp->gcount() <= 0) {
            //std::cout << lineCount << ": empty line. continue" << std::endl;
            continue;
        }
        
        std::istringstream sstrm(buf);
        if(!sstrm) {
            std::cout << "line string stream couldn't open" << std::endl;
            continue;
        }
        
        while(!sstrm.eof()) {
            std::string tmps;
            sstrm >> tmps;
            
            if(tmps[0] == '#' || tmps[0] == '\0') {
                //std::cout << lineCount << " skip line" << std::endl;
                break;
            }
            
            ParameterDesc pd = ptMap[tmps];
            
            switch(pd.valueType) {
                case kVectorValue:{
                        // vector
                        double vary[3] = {0.0, 0.0, 0.0};
                        int i = 0;
                        for(; i < 3 && !sstrm.eof(); i++) {
                            sstrm >> vary[i];
                        }
                        this->foundVector(pd.paramType, vary[0], vary[1], vary[2]);
                    }
                    break;
                case kStringValue: {
                        // string
                        std::string s;
                        sstrm >> s;
                        this->foundString(pd.paramType, s);
                    }
                    break;
                case kIntegerValue: {
                        // int
                        int i;
                        sstrm >> i;
                        this->foundInteger(pd.paramType, i);
                    }
					break;
				case kLineValue: {
						// line
						int v0, v1;
						sstrm >> v0;
						sstrm >> v1;
						this->foundLine(pd.paramType, v0, v1);
					}
					break;
                case kFaceValue: {
                        // face
                        finfovec.clear();
                        
                        while(!sstrm.eof()) {
                            FaceInfo fcinfo;
                            std::string fistr;
                            sstrm >> fistr;
                            //std::cout << "'" << fistr << "' : ";
                            
                            std::istringstream facestrm(fistr);
                            facestrm >> fcinfo.v;
                            if(facestrm.eof()) {
								// no normal and uv
                                fcinfo.vt = 0;
                                fcinfo.vn = 0;
                            } else {
								// expects v/[t]/n
                                char purgec;
                                facestrm >> purgec;
                                facestrm >> fcinfo.vt;
                                if(facestrm.fail()) {
                                    fcinfo.vt = 0;
                                    facestrm.clear();
                                }
                                facestrm >> purgec;
                                facestrm >> fcinfo.vn;
                            }
                            finfovec.push_back(fcinfo);
                        }
                        this->foundFace(pd.paramType, finfovec);
                    }
                    break;
                default:
                    std::cout << lineCount << " Wavefront .obj unknown line : " << tmps << std::endl;
            }
        }
    }
    
    delete [] buf;
    
    fsp->close();
    delete fsp;
    
    this->endFile(fpath);
	
	return true;
}

void WavefrontObj::importMTL(std::string filename) {
    loadFile(filename);
}

