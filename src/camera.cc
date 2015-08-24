
#include <iostream>
#include <cmath>
#include "camera.h"
#include "random.h"

using namespace r1h;

Camera::Camera():
/*
    position(0.0, 0.0, 0.0),
    direction(0.0, 0.0, -1.0),
    up(0.0, 1.0, 0.0),
    side(1.0, 0.0, 0.0),
    aspect(1.778),
    screenLeft(1.0),
	focusDistance(1.0),
	apertureRadius(0.0)
 */
	focalLength(28.0),
	fNumber(0.0),
	sensorWidth(36.0),
	sensorHeight(24.0),
	focusDistance(1.0)
{}

Camera::~Camera() {}

void Camera::setLookat(const Vector3 &eye, const Vector3 &look, const Vector3 &nup) {
	invTransform = Matrix4::makeLookAt(eye.x, eye.y, eye.z, look.x, look.y, look.z, nup.x, nup.y, nup.z);
	invTransform.invert();
	/*
	position = eye;
    direction = Vector3::normalized(look - eye);
    up = nup;
    side = Vector3::normalized(Vector3::cross(direction, up));
    up = Vector3::normalized(Vector3::cross(direction, side));
	 */
}
void Camera::setTransform(const Matrix4& transmat) {
	invTransform = Matrix4::inverted(transmat, nullptr);
}
/*
void Camera::setAspectRatio(const R1hFPType asp) {
    aspect = asp;
}
void Camera::setFocal(const R1hFPType focalmm, const R1hFPType sensorwidth) {
    screenLeft = sensorwidth / focalmm;
}
void Camera::setFieldOfView(const R1hFPType vdegree) {
    screenLeft = tan(vdegree * kPI / 180.0 * 0.5) * 2.0;
}

void Camera::setFocusDistance(const R1hFPType dist) {
	focusDistance = dist;
}
void Camera::setApertureRadius(const R1hFPType r) {
	apertureRadius = r;
}

R1hFPType Camera::getAspectRatio() const {
	return aspect;
}
*/
// lens
void Camera::setFocalLength(const R1hFPType fl) {
	focalLength = fl;
}
void Camera::setFNumber(const R1hFPType fn) {
	fNumber = fn;
}
void Camera::setFocusDistance(const R1hFPType dist) {
	focusDistance = dist;
}

// sensor
void Camera::setSensorSize(const R1hFPType w, const R1hFPType h) {
	sensorWidth = w;
	sensorHeight = h;
}
void Camera::setSensorWidthWithAspect(const R1hFPType w, const R1hFPType aspect) {
	sensorWidth = w;
	sensorHeight = w / aspect;
}

R1hFPType Camera::getSensorAspectRatio() const {
	return sensorWidth / sensorHeight;
}

Ray Camera::getRay(const double tx, const double ty, Random *rnd) const {
	Vector3 eyep, dir;
	
	// calc image plane position
	Vector3 ip;
	ip.x = sensorWidth * 0.5 * tx * focusDistance / focalLength;
	ip.y = sensorHeight * 0.5 * ty * focusDistance / focalLength;
	ip.z = -focusDistance;
	
	if(fNumber > 0.0) {
		// thin lens
		R1hFPType apertuerR = focalLength / fNumber * 0.0005; // 1/1000*0.5[m]
		R1hFPType rx, ry;
		do {
			rx = rnd->next11();
			ry = rnd->next11();
		} while(!apertureTest(rx, ry));
		
		eyep.set(rx * apertuerR, ry * apertuerR, 0.0);
		dir = ip - eyep;
		dir.normalize();
	} else {
		// pinhole
		eyep.set(0.0, 0.0, 0.0);
		dir = Vector3::normalized(ip);
	}
	
	eyep = Matrix4::transformV3(invTransform, eyep);
	dir = Matrix4::mulV3(invTransform, dir);
	
	/*
	Vector3 left = side * (screenLeft * tx * 0.5);
    Vector3 top = up * (screenLeft / aspect * ty * 0.5);
	Vector3 eyep, dir;
	
	if(apertureRadius > 0.0) {
		// use DOF
		R1hFPType rx, ry;
		do {
			rx = rnd->next11();
			ry = rnd->next11();
		} while(!apertureTest(rx, ry));
		
		rx *= apertureRadius;
		ry *= apertureRadius;
		
		Vector3 focusp = position + (direction + left + top) * focusDistance;
		eyep = position + side * rx + up * ry;
		dir = focusp - eyep;
	}
	else {
		eyep = position;
		dir = direction + left + top;
	}
	
    dir.normalize();
	 */
	
    return Ray(eyep, dir);
}

bool Camera::apertureTest(R1hFPType u, R1hFPType v) const {
	return (u * u + v * v) < 1.0;
	//return (fabs(u) + fabs(v)) < 1.0;
}
