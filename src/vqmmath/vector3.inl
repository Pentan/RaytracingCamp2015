#ifndef VQM_VECTOR3_INL
#define VQM_VECTOR3_INL

namespace vqm {

// self operations
template<typename FPType>
inline void Vector3<FPType>::set(const FPType ix, const FPType iy, const FPType iz) {
    x = ix;
    y = iy;
    z = iz;
}
template<typename FPType>
inline void Vector3<FPType>::set(const FPType iv[3]) {
    x = iv[0];
    y = iv[1];
    z = iv[2];
}
template<typename FPType>
inline FPType Vector3<FPType>::length(void) const {
    return sqrt(x * x + y * y + z * z);
}

template<typename FPType>
inline void Vector3<FPType>::normalize(void) {
    FPType l = x * x + y * y + z * z;
    if(l > VEC_EPS) {
        l = 1.0 / sqrt(l);
        x *= l;
        y *= l;
        z *= l;
    }
}

template<typename FPType>
inline Vector3<FPType> Vector3<FPType>::negate(void) {
    x = -x;
    y = -y;
    z = -z;
}

// 2 vector operations
template<typename FPType>
inline FPType Vector3<FPType>::distance(const Vector3 v0, const Vector3 v1) {
    Vector3 tmpv = v0 - v1;
    return tmpv.length();
}

template<typename FPType>
inline Vector3<FPType> Vector3<FPType>::normalized(const Vector3 v) {
    Vector3 ret = v;
    ret.normalize();
    return ret;
}

template<typename FPType>
inline Vector3<FPType> Vector3<FPType>::negated(const Vector3 v) {
    Vector3 retv(-v.x, -v.y, -v.z);
    return retv;
}
template<typename FPType>
inline Vector3<FPType> Vector3<FPType>::mul(const Vector3 v0, const Vector3 v1) {
    Vector3 retv;
    retv.x = v0.x * v1.x;
    retv.y = v0.y * v1.y;
    retv.z = v0.z * v1.z;
    return retv;
}
template<typename FPType>
inline Vector3<FPType> Vector3<FPType>::div(const Vector3 v0, const Vector3 v1) {
    Vector3 retv;
    retv.x = v0.x / v1.x;
    retv.y = v0.y / v1.y;
    retv.z = v0.z / v1.z;
    return retv;
}

template<typename FPType>
inline FPType Vector3<FPType>::dot(const Vector3 v0, const Vector3 v1) {
    return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
}
template<typename FPType>
inline Vector3<FPType> Vector3<FPType>::cross(const Vector3 v0, const Vector3 v1) {
    Vector3 retv;
    retv.x = v0.y * v1.z - v0.z * v1.y;
    retv.y = v0.z * v1.x - v0.x * v1.z;
    retv.z = v0.x * v1.y - v0.y * v1.x;
    return retv;
}

template<typename FPType>
inline Vector3<FPType> Vector3<FPType>::lerp(const Vector3 v0, const Vector3 v1, const FPType t) {
    Vector3 retv;
    FPType t0 = 1.0 - t;
    retv.x = v0.x * t0 + v1.x * t;
    retv.y = v0.y * t0 + v1.y * t;
    retv.z = v0.z * t0 + v1.z * t;
    return retv;
}
template<typename FPType>
inline Vector3<FPType> Vector3<FPType>::project(const Vector3 v0, const Vector3 v1) {
    Vector3 nv = Vector3::normalized(v1);
    FPType d = Vector3::dot(v0, nv);
    return nv + d;
}

// util
template<typename FPType>
void Vector3<FPType>::sprint(char *buf, const Vector3 v) {
    sprintf(buf, "v3(%.4lf,%.4lf,%.4lf)", (double)v.x, (double)v.y, (double)v.z);
}

// operators
template<typename FPType>
inline Vector3<FPType> Vector3<FPType>::operator+(const Vector3 &b) const {
    return Vector3(x + b.x, y + b.y, z + b.z);
}
template<typename FPType>
inline Vector3<FPType> Vector3<FPType>::operator-(const Vector3 &b) const {
    return Vector3(x - b.x, y - b.y, z - b.z);
}
template<typename FPType>
inline Vector3<FPType> Vector3<FPType>::operator*(const FPType s) const {
    return Vector3(x * s, y * s, z * s);
}
template<typename FPType>
inline Vector3<FPType> Vector3<FPType>::operator/(const FPType s) const {
    return Vector3(x / s, y / s, z / s);
}

template<typename FPType>
inline Vector3<FPType> Vector3<FPType>::operator+=(const Vector3 &b) {
    x += b.x;
    y += b.y;
    z += b.z;
    return *this;
}

template<typename FPType> inline const Vector3<FPType> operator*(const FPType s, const Vector3<FPType> &v) {
    return v * s;
}

template<typename FPType> inline const Vector3<FPType> operator/(const FPType s, const Vector3<FPType> &v) {
    return v / s;
}

}

#endif