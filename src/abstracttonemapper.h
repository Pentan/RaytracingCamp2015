#ifndef R1H_ABSTRACTTONEMAPPER_H
#define R1H_ABSTRACTTONEMAPPER_H

#include <string>
#include <fstream>
#include "r1htypes.h"

namespace r1h {

class FrameBuffer;

/// virtual class
class AbstractToneMapper
{
public:
    AbstractToneMapper() : gamma(2.2) {};
    virtual ~AbstractToneMapper() { /*printf("abstract tone mapper destructed\n");*/ };
    
    void setGamma(const double g) { gamma = g; };
    
    void exportBMP(const FrameBuffer *frmbuf, const std::string outpath);
    void exportTGA(const FrameBuffer *frmbuf, const std::string outpath);
    
protected:
    double gamma;
    
    virtual Color tonemap(const Color &col) { return col; };
    
    inline R1hColorType clamp(R1hColorType c);
    inline int packU8(R1hColorType c);
    inline void packColor(int *rgb, const Color &color);
    
    inline void put16(const unsigned short s, std::ofstream &ofs);
    inline void put32(const unsigned long l, std::ofstream &ofs);
};

}
#endif
