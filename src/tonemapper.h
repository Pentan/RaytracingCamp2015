#ifndef R1H_TONEMAPPER_H
#define R1H_TONEMAPPER_H

#include "r1htypes.h"
#include "abstracttonemapper.h"

namespace r1h {

class FrameBuffer;

/// pass-through tone mapper
class ToneMapper : public AbstractToneMapper 
{
public:
    ToneMapper();
    virtual ~ToneMapper();
};

}
#endif
