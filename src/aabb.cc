

#include <algorithm>
#include "aabb.h"

using namespace r1h;

AABB::AABB():
	min(FLT_MAX), max(-FLT_MAX)
{}

AABB::AABB(const R1hFPType minval, const R1hFPType maxval)
{
	if(minval < maxval) {
		min = Vector3(minval);
		max = Vector3(maxval);
	} else {
		min = Vector3(maxval);
		max = Vector3(minval);
	}
}

AABB::AABB(const Vector3 minvec, const Vector3 maxvec)
{
	for(int i = 0; i < 3; i++) {
		if(minvec.v[i] < maxvec.v[i]) {
			min.v[i] = minvec.v[i];
			max.v[i] = maxvec.v[i];
		} else {
			min.v[i] = maxvec.v[i];
			max.v[i] = minvec.v[i];
		}
	}
}

void AABB::clear() {
	min = Vector3(FLT_MAX);
	max = Vector3(-FLT_MAX);
}

Vector3 AABB::getSize() const {
	return max - min;
}

void AABB::expand(const Vector3 &p) {
	if(p.x_ < min.x_) min.x_ = p.x_;
	if(p.y_ < min.y_) min.y_ = p.y_;
	if(p.z_ < min.z_) min.z_ = p.z_;
	
	if(p.x_ > max.x_) max.x_ = p.x_;
	if(p.y_ > max.y_) max.y_ = p.y_;
	if(p.z_ > max.z_) max.z_ = p.z_;
	
	updateCentroid();
}

void AABB::expand(const AABB &aabb) {
	if(min.x_ > aabb.min.x_) min.x_ = aabb.min.x_;
	if(min.y_ > aabb.min.y_) min.y_ = aabb.min.y_;
	if(min.z_ > aabb.min.z_) min.z_ = aabb.min.z_;
	
	if(max.x_ < aabb.max.x_) max.x_ = aabb.max.x_;
	if(max.y_ < aabb.max.y_) max.y_ = aabb.max.y_;
	if(max.z_ < aabb.max.z_) max.z_ = aabb.max.z_;
	
	updateCentroid();
}

void AABB::updateCentroid() {
	centroid = (min + max) * 0.5;
}

bool AABB::isInside(const Vector3 &p) const {
	return ((p.x_ > min.x_ && p.y_ > min.y_ && p.z_ > min.z_) &&
			(p.x_ < max.x_ && p.y_ < max.y_ && p.z_ < max.z_) );
}

bool AABB::isIntersect(const Ray &ray, R1hFPType *outmin, int *outaxis) const {
	R1hFPType tmin, tmax;
	R1hFPType tmpmin, tmpmax;
	int minaxis = 0;
	
	tmin = -DBL_MAX;
	tmax = DBL_MAX;
	
	for(int i = 0; i < 3; i++) {
		
		//std::cout << "check for component [" << i << "]" << std::endl;
		//std::cout << " ray.dir_[" << i << "]:" << ray.dir_.v[i] << std::endl;
		if(fabs(ray.direction.v[i]) < kEPS) {
			//std::cout << " parallel ray. skip:" << i << " (" << ray.dir_.v[i] << ")" << std::endl;
			continue;
		}
		R1hFPType vdiv = 1.0 / ray.direction.v[i];
		//std::cout << " vdiv:" << vdiv << std::endl;
		/*
		if(ray.org_.v[i] > min.v[i] && ray.org_.v[i] < max.v[i]) {
			std::cout << " !!!origin component[" << i << "] " << ray.org_.v[i]  << " is inside a range (" << min.v[i] << "," << max.v[i] << ")" << std::endl;
		}
		*/
		if(ray.direction.v[i] >= 0.0) {
			tmpmin = (min.v[i] - ray.origin.v[i]) * vdiv;
			tmpmax = (max.v[i] - ray.origin.v[i]) * vdiv;
		} else {
			tmpmax = (min.v[i] - ray.origin.v[i]) * vdiv;
			tmpmin = (max.v[i] - ray.origin.v[i]) * vdiv;
		}
		
		// reduction width
		if(tmpmin > tmin) {
			tmin = tmpmin;
			minaxis = i;
		}
		if(tmpmax < tmax) {
			tmax = tmpmax;
		}
		
		//std::cout << " tmp[" << i << "]:" << tmpmin << "," << tmpmax << std::endl;
		//std::cout << " t[" << i << "]:" << tmin << "," << tmax << std::endl;
		
		// not hit
		if(tmax < tmin) {
			//std::cout << "### not hit:" << tmin << "," << tmax << std::endl;
			return false;
		}
	}
		
	// behind the ray
	if(tmax < 0.0 && tmin < 0.0) {
		//std::cout << "### intersected but behind." << std::endl;
		return false;
	}
	
	// output
	if(outmin) {
		*outmin = tmin;
	}
	if(outaxis) {
		*outaxis = minaxis;
	}
	
	return true;
}
