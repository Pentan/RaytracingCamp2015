#ifndef R1H_TEXTUREBASE_H
#define R1H_TEXTUREBASE_H

#include <vector>
#include <memory>
#include "r1htypes.h"
#include "asset.h"

namespace r1h {

class SceneObject;
class Intersection;
	
/// base class
class Texture : public Asset {
public:
	enum MapType {
		kUV,
		kWorld,
		kLocal
	};
	
public:
	Texture();
	
	// must override
	virtual ~Texture();
	virtual Color sample(const Vector3 &p) const = 0;
	
	// option
	virtual Color sample(const SceneObject *obj, const Intersection *isect);
	
	virtual Vector3 applyTransform(const Vector3 &p) const;
	virtual void setIsUseTransform(const bool isuse);
	virtual void setTransform(const Matrix4 m);
	
	virtual void setMapType(MapType type);
	virtual int getMapType() const;
	
private:
	bool isUseTransform;
	Matrix4 transform;
	
	int mapType;
};

typedef std::shared_ptr<Texture> TextureRef;

/// stub
class StubTexture : public Texture {
public:
	StubTexture(const std::string& refid) : Texture() {
		assetId = refid;
		assetType = Asset::Type::kStub;
	};
	~StubTexture() {};
	
	Color sample(const Vector3 &p) const { return Color(1.0, 0.0, 0.0); };
};

/// Constant color
class ConstantColorTexture : public Texture {
public:
	ConstantColorTexture(const Color col=Color(1.0,0.0,0.0));
	~ConstantColorTexture();
	
	Color sample(const Vector3 &p) const;
	void setColor(const Color& col);
	
private:
	Color color;
};

}
#endif
