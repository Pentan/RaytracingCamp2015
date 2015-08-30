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
	height(0),
	interpolate(kNearest)
{}

ImageTexture::~ImageTexture() {
	if(texels) {
		delete [] texels;
	}
}

bool ImageTexture::load(const std::string& path, int ipo, int flip, R1hFPType gamma, R1hFPType power) {
	int w, h, comp;

	interpolate = ipo;

	stbi_set_flip_vertically_on_load(flip);
	stbi_ldr_to_hdr_gamma(float(gamma));
	//stbi_ldr_to_hdr_scale(float(power)); // want to apply both HDR and LDR case.

	float *buf = stbi_loadf(path.c_str(), &w, &h, &comp, 0);

	//printf("image:%s size:(%d,%d), comp:%d\n", path.c_str(), w, h, comp);

	texels = new Color[w * h];
	width = w;
	height = h;
	
	for(int y = 0; y < h; y++) {
		for(int x = 0; x < w; x++) {
			int index = x + y * w;
			Color *txl = &texels[index];
			float *srcpxl = &buf[index * comp];
			int ic;
			for (ic = 0; ic < comp; ic++) {
				if (ic > 2) { break; }
				txl->v[ic] = srcpxl[ic] * power;
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
	
	uv.x = uv.x - floor(uv.x);
	uv.y = uv.y - floor(uv.y);

	uv.x *= width;
	uv.y *= height;
	
	// nearest
	int u = int(uv.x);
	int v = int(uv.y);
	//
	R1hFPType ut = uv.x - R1hFPType(u);
	R1hFPType vt = uv.y - R1hFPType(v);
	
	// repeat
	u = u % width;
	v = v % height;

	Color ret;

	switch(interpolate) {
		case kNearest:
			ret = texels[u + v * width];
			break;
		case kLinear:
		{
			R1hFPType t00 = ut * vt;
			R1hFPType t10 = (1.0 - ut) * vt;
			R1hFPType t01 = ut * (1.0 - vt);
			R1hFPType t11 = (1.0 - ut) * (1.0 - vt);

			int u0 = u;
			int u1 = (u + 1) % width;
			int v0 = v;
			int v1 = (v + 1) % height;

			ret = texels[u0 + v0 * width] * t11 +
				  texels[u1 + v0 * width] * t01 +
				  texels[u0 + v1 * width] * t10 +
				  texels[u1 + v1 * width] * t00;
		}
			break;
	}

	return ret;
}
