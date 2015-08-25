

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
	if(p.x < min.x) min.x = p.x;
	if(p.y < min.y) min.y = p.y;
	if(p.z < min.z) min.z = p.z;
	
	if(p.x > max.x) max.x = p.x;
	if(p.y > max.y) max.y = p.y;
	if(p.z > max.z) max.z = p.z;
	
	updateCentroid();
}

void AABB::expand(const AABB &aabb) {
	if(min.x > aabb.min.x) min.x = aabb.min.x;
	if(min.y > aabb.min.y) min.y = aabb.min.y;
	if(min.z > aabb.min.z) min.z = aabb.min.z;
	
	if(max.x < aabb.max.x) max.x = aabb.max.x;
	if(max.y < aabb.max.y) max.y = aabb.max.y;
	if(max.z < aabb.max.z) max.z = aabb.max.z;
	
	updateCentroid();
}

void AABB::updateCentroid() {
	centroid = (min + max) * 0.5;
}

bool AABB::isInside(const Vector3 &p) const {
	return ((p.x > min.x && p.y > min.y && p.z > min.z) &&
			(p.x < max.x && p.y < max.y && p.z < max.z) );
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
