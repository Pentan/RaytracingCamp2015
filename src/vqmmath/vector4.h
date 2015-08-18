#ifndef VQM_VECTOR4_H
#define VQM_VECTOR4_H

#include <cstdio>
#include <cmath>

#include "constants.h"
#include "vector3.h"

namespace vqm {

template<typename FPType> union Vector4 {
    struct {FPType x, y, z, w;};
    struct {FPType r, g, b, a;};
    struct {FPType s, t, p, q;};
    FPType v[4];

    //
    Vector4(): x(0.0), y(0.0), z(0.0), w(0.0) {}
    Vector4(const FPType n): x(n), y(n), z(n), w(n) {}
    Vector4(const FPType ix, const FPType iy, const FPType iz, const FPType iw): x(ix), y(iy), z(iz), w(iw) {}
    
    // self operation
    inline void set(const FPType ix, const FPType iy, const FPType iz, const FPType iw);
    inline void set(const FPType iv[4]);
    inline void set(const Vector3<FPType> vxyz, const FPType iw);
    
    inline Vector3<FPType> getXYZ(void) const;
    inline FPType length(void) const;
    
    inline void normalize(void);
    inline void negate(void);

    // 2 vector
    static inline FPType distance(const Vector4 v0, const Vector4 v1);
    static inline Vector4 normalized(const Vector4 v);
    static inline Vector4 negated(const Vector4 v);
    static inline Vector4 mul(const Vector4 v0, const Vector4 v1);
    static inline Vector4 div(const Vector4 v0, const Vector4 v1);
    
    static inline FPType dot(const Vector4 v0, const Vector4 v1);
    static inline Vector4 cross(const Vector4 v0, const Vector4 v1);
    
    static inline Vector4 lerp(const Vector4 v0, const Vector4 v1, FPType t);
    static inline Vector4 project(const Vector4 v0, const Vector4 v1);
    
    // util
    static void sprint(char *buf, const Vector4 v);
    
    // operators
    inline Vector4 operator+(const Vector4 &v);
    inline Vector4 operator-(const Vector4 &v);
    inline Vector4 operator*(const FPType s);
    inline Vector4 operator/(const FPType s);
    inline Vector4 operator+=(const Vector4 &b);
};

template<typename FPType> inline const Vector4<FPType> operator*(const FPType s, const Vector4<FPType> &v);
template<typename FPType> inline const Vector4<FPType> operator/(const FPType s, const Vector4<FPType> &v);

}

// implementation
#include "vector4.inl"

#endif
