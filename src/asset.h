#ifndef R1H_ASSET_H
#define R1H_ASSET_H

#include <string>

namespace r1h {

class Asset {
public:
	enum Type {
		kStub		= -1,
		kStandard	= 0
	};
public:
	Asset() :
		assetType(kStandard)
	{}
	virtual ~Asset() {}
	
	std::string getAssetId() const { return assetId; };
	void setAssetId(const std::string& id) { assetId = id; };
	
	std::string getAssetName() const { return assetName; };
	void setAssetName(const std::string& name) { assetName = name; };
	
	int getAssetType() const { return assetType; };
	
protected:
	std::string assetId;
	std::string assetName;
	int assetType;
};

}

#endif
