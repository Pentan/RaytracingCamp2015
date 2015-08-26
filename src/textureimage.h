﻿#ifndef R1H_TEXTURE_IMAGE_H
#define R1H_TEXTURE_IMAGE_H

#include <string>
#include "texturebase.h"

namespace r1h {

class ImageTexture : public Texture {
public:
	ImageTexture();
	~ImageTexture();

	bool load(const std::string& path);

	virtual Color sample(const Vector3 &p) const;
	
private:
	Color *texels;
	int width;
	int height;
};

}
#endif