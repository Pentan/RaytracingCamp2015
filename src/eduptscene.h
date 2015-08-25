#ifndef R1H_EDUPTSCENE_H
#define R1H_EDUPTSCENE_H

#include "material.h"
#include "bsdf.h"
#include "bsdfdiffuse.h"
#include "bsdfspecular.h"
#include "bsdfrefraction.h"
#include "bsdfpaint.h"
#include "scene.h"

namespace r1h {

/// scene
class EduptScene {
public:
	static bool load(Scene *scene, double aspect);
	static bool load2(Scene *scene, double aspect);
};

}

#endif
