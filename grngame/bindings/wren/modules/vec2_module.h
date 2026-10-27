#pragma once

#include "cglm/types-struct.h"
#include "grngame/math/types.h"
#include "grngame/utils/c_cpp.h"
#include "wren.h"
#include <math.h>

BEGIN_DECLARATIONS

vec2s *wrenGetSlotVec2(WrenVM *vm, int32 slot);
void SetSlotVec2(WrenVM *vm, vec2s value);

END_DECLARATIONS