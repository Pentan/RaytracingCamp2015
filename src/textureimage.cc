#include "textureimage.h"

///
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

///

using namespace r1h;

ImageTexture::ImageTexture():
	Texture(),
	texels(nullptr),
	width(0),
	height(0)
{}

ImageTexture::~ImageTexture() {
	if(texels) {
		delete [] texels;
	}
}

bool ImageTexture::load(const std::string& path) {
	int w, h, comp;

	unsigned char *buf = stbi_load(path.c_str(), &w, &h, &comp, 0);
	
	printf("image:%s size:(%d,%d), comp:%d\n", path.c_str(), w, h, comp);
	
	texels = new Color[w * h];
	width = w;
	height = h;
	
	for(int y = 0; y < h; y++) {
		for(int x = 0; x < w; x++) {
			int index = x + y * w;
			Color *txl = &texels[index];
			unsigned char *srcpxl = &buf[index * comp];
			int ic;
			for(ic = 0; ic < comp; ic++) {
				if(ic > 2) { break; }
				txl->v[ic] = pow(srcpxl[ic] / 255.0, 2.2); // TODO: ungamma!
			}
			for(; ic < 3; ic++) {
				txl->v[ic] = txl->v[ic - 1];
			}
		}
	}

	stbi_image_free(buf);

	return false;
}

Color ImageTexture::sample(const Vector3 &p) const {
	
	Vector3 uv = applyTransform(p);
	
	uv.x *= width;
	uv.y *= height;
	
	// nearest
	int u = int(uv.x);
	int v = int(uv.y);
	
	if(u < 0) { u = 0; }
	else if(u >= width) { u = width - 1; }
	if(v < 0) { v = 0; }
	else if(v >= height) { v = height - 1; }
	
	return texels[u + v * width];
}
