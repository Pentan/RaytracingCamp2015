#include "tonemapper.h"
#include "framebuffer.h"

using namespace r1h;

/// pass-through tone mapper
ToneMapper::ToneMapper() : AbstractToneMapper()
{
};

ToneMapper::~ToneMapper() {
    //printf("tone mapper destructed\n");
};
