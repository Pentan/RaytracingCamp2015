#ifndef R1H_CAMERA_H
#define R1H_CAMERA_H

#include <memory>
#include "r1htypes.h"
#include "ray.h"
#include "sceneobject.h"

namespace r1h {

class Camera : public SceneObject {
public:
    Camera();
    ~Camera();
    
    void setLookat(const Vector3 &eye, const Vector3 &look, const Vector3 &nup);
	void setTransform(const Matrix4& transmat);
	
	// lens
	void setFocalLength(const R1hFPType fl);
	void setFNumber(const R1hFPType fn);
	void setFocusDistance(const R1hFPType dist);
	
	// sensor
	void setSensorSize(const R1hFPType w, const R1hFPType h);
	void setSensorWidthWithAspect(const R1hFPType w, const R1hFPType aspect);
	
	//
	R1hFPType getSensorAspectRatio() const;
	
    /// expects tx and ty are (-1,1)
    Ray getRay(const double tx, const double ty, Random *rnd) const;
    
private:
	Matrix4 invTransform;
	
	R1hFPType focalLength;		// [mm]
	R1hFPType fNumber;			// [N]
	R1hFPType sensorWidth;		// [mm]
	R1hFPType sensorHeight;		// [mm]
	R1hFPType focusDistance;	// [m]
	
	bool apertureTest(R1hFPType u, R1hFPType v) const;
};

typedef std::shared_ptr<Camera> CameraRef;
	
}
#endif
