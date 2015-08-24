
#include "abstracttonemapper.h"
#include <iostream>
#include <cstdio>

#include "framebuffer.h"

using namespace r1h;

inline R1hColorType AbstractToneMapper::clamp(R1hColorType c) {
    if(c < 0.0) return 0.0;
    if(c > 1.0) return 1.0;
    return c;
}

inline int AbstractToneMapper::packU8(R1hColorType c) {
    return int(pow(clamp(c), 1.0/gamma) * 255 + 0.5);
}

inline void AbstractToneMapper::packColor(int *rgb, const Color &color) {
    rgb[0] = packU8(color.r);
    rgb[1] = packU8(color.g);
    rgb[2] = packU8(color.b);
}

inline void AbstractToneMapper::put16(const unsigned short s, std::ofstream &ofs) {
    ofs.put(s & 0xff);
    ofs.put((s >> 8) & 0xff);
}

inline void AbstractToneMapper::put32(const unsigned long l, std::ofstream &ofs) {
    ofs.put(l & 0xff);
    ofs.put((l >> 8) & 0xff);
    ofs.put((l >> 16) & 0xff);
    ofs.put((l >> 24) & 0xff);
}

void AbstractToneMapper::exportBMP(const FrameBuffer *frmbuf, const std::string outpath) {
    //printf("export %s\n", outpath.c_str());
    
    int width = frmbuf->getWidth();
    int height = frmbuf->getHeight();
    
    // stream open
    std::ofstream ofs(outpath.c_str(), std::ios::out | std::ios::binary);
    
    // header
    // signature
    ofs << "BM";
    // total length ([RGB datas] + [BMP header(14)] + [BMP image header(40)])
    put32(width * height * 3 + 14 + 40, ofs);
    // reserved
    put16(0, ofs);
    put16(0, ofs);
    // offset to image
    put32(14 + 40, ofs);
    
    // image header
    // header size
    put32(40, ofs);
    // image size
    put32(width, ofs);
    put32(height, ofs);
    // bit lanes
    put16(1, ofs);
    // color bits
    put16(24, ofs);
    // compression (0:none)
    put32(0, ofs);
    // bitmap length
    put32(width * height * 3, ofs);
    // dot per meter (72dpi)
    put32(2835, ofs);
    put32(2835, ofs);
    // CLUT info
    put32(0, ofs);
    put32(0, ofs);
    
    // bitmap
    int rgb[3];
    //for(int y = height - 1; y >= 0; y--) {
	for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            const Color &srccol = frmbuf->getColorAt(x, y);
            const Color outcol = tonemap(srccol);
            packColor(rgb, outcol);
            
            // bmp is BGR
            ofs.put(rgb[2]);
            ofs.put(rgb[1]);
            ofs.put(rgb[0]);
        }
    }
    
    ofs.close();
    //std::cerr << outpath << " saved" << std::endl;
}

void AbstractToneMapper::exportTGA(const FrameBuffer *frmbuf, const std::string outpath) {
    //printf("export %s\n", outpath.c_str());
    
    std::ofstream ofs(outpath, std::ios::out | std::ios::binary);
    
    // now, Bit per pixel is only 24bit format.
    const int bpp = 24;
    int width = frmbuf->getWidth();
    int height = frmbuf->getHeight();
    
    // header
    // image id field length
    ofs.put(0); // no image id field
    // color map flag
    ofs.put(0); // 0: no color map
    // image type
    ofs.put(0x02); // 0x02: uncompressed RGB
    
    // color map origin
    put16(0, ofs);
    // color map length
    put16(0, ofs);
    // color map size
    ofs.put(0);
    
    // image origin
    put16(0, ofs); // x
    put16(0, ofs); // y
    // image size
    put16(width, ofs); // w
    put16(height, ofs); // h
    // bit per pixel
    ofs.put(bpp);
    // image descriptor flags
    ofs.put(0); //0~3:attr(?), 4:is holizontal flipped, 5:is virtical flipped, 6~7:reserved
    
    // pixels
    int stride = bpp / 8;
    int rgb[4];
    
    //for(int y = height - 1; y >= 0; y--) {
    for(int y = 0; y < height; y--) {
        for(int x = 0; x < width; x++) {
            const Color &srccol = frmbuf->getColorAt(x, y);
            const Color outcol = tonemap(srccol);
            packColor(rgb, outcol);
            
            // bmp is BGR
            ofs.put(rgb[2]);
            ofs.put(rgb[1]);
            ofs.put(rgb[0]);
            
            if(stride > 3) {
                ofs.put(0xff);
            }
        }
    }
    
    // footer
    // extension area offset
    put32(0, ofs);
    // developper area offset
    put32(0, ofs);
    // TRUEVISION-XFILE
    ofs << "TRUEVISION-XFILE";
    // EOF
    ofs.put(0);
    
    ofs.close();
    /////
    //std::cerr << outpath << " saved" << std::endl;
}
