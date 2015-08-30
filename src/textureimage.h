#ifndef R1H_TEXTURE_IMAGE_H
#define R1H_TEXTURE_IMAGE_H

#include <string>
#include "texturebase.h"

namespace r1h {

class ImageTexture : public Texture {
public:
	enum Interpolate {
		kNearest,
		kLinear
	};

	ImageTexture();
	~ImageTexture();

	bool load(const std::string& path, int ipo=kNearest, int flip=0, R1hFPType gamma=2.2, R1hFPType power=1.0);

	virtual Color sample(const Vector3 &p) const;
	
private:
	Color *texels;
	int width;
	int height;
	int interpolate;
};

}
#endif