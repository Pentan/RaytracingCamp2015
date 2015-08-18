
#include "framebuffer.h"
#include <cstdio>

using namespace r1h;

FrameBuffer::FrameBuffer(int w, int h):
    buffer(0), width(w), height(h)
{
    buffer = new Pixel[width * height];
}

FrameBuffer::~FrameBuffer() {
    delete [] buffer;
}

Color FrameBuffer::getColorAt(const int x, const int y) const {
    int ix = (x < 0)? 0 : ((x >= width)? width - 1 : x);
    int iy = (y < 0)? 0 : ((y >= height)? height - 1 : y);
    
    Pixel &pixel = buffer[ ix + iy * width ];
    return pixel.color / pixel.sampleCount;
};


FrameBuffer::Tile FrameBuffer::makeTile(const int x, const int y, const int w, const int h) const {
    Tile ret;
	ret.startx = x;
	ret.starty = y;
	ret.endx = x + w;
	ret.endy = y + h;
    
	// iteration expectes for(i=start; i < end; i++)
	
    if(ret.startx < 0) { ret.startx = 0; }
    else if(ret.startx >= width) { ret.startx = width - 1; }
    if(ret.starty < 0) { ret.starty = 0; }
    else if(ret.starty >= height) { ret.starty = height - 1; }
	
    if(ret.endx < 0) { ret.endx = 0; }
    else if(ret.endx >= width) { ret.endx = width; }
    if(ret.endy < 0) { ret.endy = 0; }
    else if(ret.endy >= height) { ret.endy = height; }
    
    return ret;
}

void FrameBuffer::accumulate(const int x, const int y, const Color& col) {
    int ix = (x < 0)? 0 : ((x >= width)? width - 1 : x);
    int iy = (y < 0)? 0 : ((y >= height)? height - 1 : y);
    
    Pixel &pixel = buffer[ ix + iy * width ];
    pixel.addColor(col);
}

/////
FrameBuffer::Pixel::Pixel():
	color(0.0), sampleCount(0)
{
}

void FrameBuffer::Pixel::clear() {
    color.set(0.0, 0.0, 0.0);
    sampleCount = 0;
}
void FrameBuffer::Pixel::addColor(const Color &col) {
    color = color + col;
    sampleCount += 1;
}
