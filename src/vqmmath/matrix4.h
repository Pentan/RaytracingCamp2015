#ifndef VQM_MATRIX4_H
#define VQM_MATRIX4_H

#include "vector3.h"
#include "vector4.h"

namespace vqm {

template<typename FPType> union Matrix4 {
    struct {
        FPType m00, m01, m02, m03;
        FPType m10, m11, m12, m13;
        FPType m20, m21, m22, m23;
        FPType m30, m31, m32, m33;
    };
    FPType m[16];
    
    // utilities
    static inline void sprint(char *buf, const Matrix4 &m, const bool isbr);
    
    // constructors
    // default constructor makes identity.
    Matrix4():
        m00(1.0), m01(0.0), m02(0.0), m03(0.0),
        m10(0.0), m11(1.0), m12(0.0), m13(0.0),
        m20(0.0), m21(0.0), m22(1.0), m23(0.0),
        m30(0.0), m31(0.0), m32(0.0), m33(1.0)
    {}
    Matrix4(
        const FPType im00, const FPType im01, const FPType im02, const FPType im03,
        const FPType im10, const FPType im11, const FPType im12, const FPType im13,
        const FPType im20, const FPType im21, const FPType im22, const FPType im23,
        const FPType im30, const FPType im31, const FPType im32, const FPType im33):
        m00(im00), m01(im01), m02(im02), m03(im03),
        m10(im10), m11(im11), m12(im12), m13(im13),
        m20(im20), m21(im21), m22(im22), m23(im23),
        m30(im30), m31(im31), m32(im32), m33(im33)
    {}
    Matrix4(const FPType im[16]):
        m00(im[0]), m01(im[1]), m02(im[2]), m03(im[3]),
        m10(im[4]), m11(im[5]), m12(im[6]), m13(im[7]),
        m20(im[8]), m21(im[9]), m22(im[10]), m23(im[11]),
        m30(im[12]), m31(im[13]), m32(im[14]), m33(im[15])
    {}
    
    // self operation
    inline void set(
        const FPType nm00, const FPType nm01, const FPType nm02, const FPType nm03,
        const FPType nm10, const FPType nm11, const FPType nm12, const FPType nm13,
        const FPType nm20, const FPType nm21, const FPType nm22, const FPType nm23,
        const FPType nm30, const FPType nm31, const FPType nm32, const FPType nm33
    );
    inline void set(const FPType nm[16]);
    inline void setColumns(const Vector4<FPType> &c0, const Vector4<FPType> &c1, const Vector4<FPType> &c2, const Vector4<FPType> &c3);
    inline void setRows(const Vector4<FPType> &r0, const Vector4<FPType> &r1, const Vector4<FPType> &r2, const Vector4<FPType> &r3);
    inline void setIdentity();
    inline void setTranslation(const FPType tx, const FPType ty, const FPType tz);
    inline void setRotation(const FPType rad, const FPType ax, const FPType ay, const FPType az);
    inline void setScale(const FPType sx, const FPType sy, const FPType sz);
    
    inline Vector4<FPType> getColumn(const int col) const;
    inline void setColumn(const int col, const Vector4<FPType> &v);
    inline Vector4<FPType> getRow(const int row) const;
    inline void setRow(const int row, const Vector4<FPType> &v);
    
    inline bool invert();
    inline void transpose();
    inline bool invTrans();
    
    inline void translate(const FPType tx,const FPType ty, const FPType tz);
    inline void translate(const Vector3<FPType> &transv);
    inline void rotate(const FPType rad, const FPType ax, const FPType ay, const FPType az);
    inline void rotate(const FPType rad, const Vector3<FPType> &axisv);
    inline void scale(const FPType sx, const FPType sy, const FPType sz);
    inline void scale(const Vector3<FPType> &scalev);

    // 1 matrix operations
    static inline Matrix4 makeTranslation(const FPType tx, const FPType ty, const FPType tz);
    static inline Matrix4 makeRotation(const FPType rad, const FPType ax, const FPType ay, const FPType az);
    static inline Matrix4 makeScale(const FPType sx, const FPType sy, const FPType sz);

    static inline Matrix4 makeOrtho(const FPType left, const FPType right, const FPType bottom, const FPType top, const FPType nearz, const FPType farz);
    static inline Matrix4 makeFrustum(const FPType left, const FPType right, const FPType bottom, const FPType top, const FPType nearz, const FPType farz);
    static inline Matrix4 makePerspective(const FPType yrad, const FPType aspect, const FPType nearz, const FPType farz);
    static inline Matrix4 makeLookAt(const FPType eyex, const FPType eyey, const FPType eyez, const FPType lookx, const FPType looky, const FPType lookz, const FPType upx, const FPType upy, const FPType upz);
    
    static inline Matrix4 inverted(const Matrix4 &m, bool *isinv);
    static inline Matrix4 transposed(const Matrix4 &m);
    static inline Matrix4 invTransed(const Matrix4 &m, bool *isinv);
    
    static inline Matrix4 translated(const Matrix4 &m, const FPType tx, const FPType ty, const FPType tz);
    static inline Matrix4 translated(const Matrix4 &m, const Vector3<FPType> &transv);
    static inline Matrix4 rotated(const Matrix4 &m, const FPType rad, const FPType ax, const FPType ay, const FPType az);
    static inline Matrix4 rotated(const Matrix4 &m, const FPType rad, const Vector3<FPType> axisv);
    static inline Matrix4 scaled(const Matrix4 &m, const FPType sx, const FPType sy, const FPType sz);
    static inline Matrix4 scaled(const Matrix4 &m, const Vector3<FPType> &scalev);

    // vs vectors
    static inline Vector3<FPType> mulV3(const Matrix4 &ml, const Vector3<FPType> &vr);
    static inline Vector3<FPType> transformV3(const Matrix4 &ml, const Vector3<FPType> &vr);
    
    static inline Vector4<FPType> mulV4(const Matrix4 &ml, const Vector4<FPType> &vr);
    static inline Vector3<FPType> mulAndProjectV3(const Matrix4 ml, const Vector3<FPType> vr);
    
    // operaters
    inline Matrix4 operator+(const Matrix4 &mr);
    inline Matrix4 operator-(const Matrix4 &mr);
    inline Matrix4 operator*(const Matrix4 &mr);
    inline Matrix4 operator*(const FPType s);
};

// VS scalar
template<typename FPType> inline Matrix4<FPType> operator*(const FPType s, Matrix4<FPType> &m);

// VS scalar
template<typename FPType> inline Matrix4<FPType> operator/(const FPType s, Matrix4<FPType> &m);

}

// implementation
#include "matrix4.inl"

#endif
