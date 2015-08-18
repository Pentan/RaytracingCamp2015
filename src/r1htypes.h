#ifndef R1H_MATH_H
#define R1H_MATH_H

#include <math.h>

#include "vqmmath/vector3.h"
#include "vqmmath/vector4.h"
#include "vqmmath/matrix4.h"

namespace r1h {

// math
typedef double R1hFPType;

const R1hFPType kPI = 3.14159265358979323846;
const R1hFPType kINF = 1e128;
const R1hFPType kEPS = 1e-8;

typedef vqm::Vector3<R1hFPType> Vector3;
typedef vqm::Vector4<R1hFPType> Vector4;
typedef vqm::Matrix4<R1hFPType> Matrix4;

// color
typedef double R1hColorType;
typedef vqm::Vector3<R1hColorType> Color;

}

#endif
