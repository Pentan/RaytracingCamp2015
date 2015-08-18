#ifndef R1H_ASSET_H
#define R1H_ASSET_H

#include <string>

namespace r1h {

class Asset {
public:
	std::string getAssetId() const { return assetId; };
	std::string getAssetName() const { return assetName; };
	
protected:
	std::string assetId;
	std::string assetName;
};

}

#endif
