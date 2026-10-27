#include "cglm/types-struct.h"
#include "grngame/core/app.h"
#include "wren.h"
#include <math.h>

vec2s *wrenGetSlotVec2(WrenVM *vm, int32 slot)
{
    return (vec2s *)wrenGetSlotForeign(vm, slot);
}

void SetSlotVec2(WrenVM *vm, vec2s value)
{
    wrenEnsureSlots(vm, 2);
    wrenGetVariable(vm, "std/wren/math/vec2", "Vec2", 1);
    vec2s *out = (vec2s *)wrenSetSlotNewForeign(vm, 0, 1, sizeof(vec2s));
    *out = value;
}

void Vec2_Allocate(WrenVM *vm)
{
    vec2s *v = (vec2s *)wrenSetSlotNewForeign(vm, 0, 0, sizeof(vec2s));
    int32 count = wrenGetSlotCount(vm);

    if (count >= 3)
    {
        v->x = (float32)wrenGetSlotDouble(vm, 1);
        v->y = (float32)wrenGetSlotDouble(vm, 2);
    }
    else
    {
        // set nan for positionnal sound
        v->x = NAN;
        v->y = NAN;
    }
}

void Vec2_GetX(WrenVM *vm)
{
    vec2s *v = (vec2s *)wrenGetSlotForeign(vm, 0);
    wrenSetSlotDouble(vm, 0, (float64)v->x);
}

void Vec2_GetY(WrenVM *vm)
{
    vec2s *v = (vec2s *)wrenGetSlotForeign(vm, 0);
    wrenSetSlotDouble(vm, 0, (float64)v->y);
}

void Vec2_SetX(WrenVM *vm)
{
    vec2s *v = (vec2s *)wrenGetSlotForeign(vm, 0);
    v->x = (float32)wrenGetSlotDouble(vm, 1);
    wrenSetSlotDouble(vm, 0, (float64)v->x);
}

void Vec2_SetY(WrenVM *vm)
{
    vec2s *v = (vec2s *)wrenGetSlotForeign(vm, 0);
    v->y = (float32)wrenGetSlotDouble(vm, 1);
    wrenSetSlotDouble(vm, 0, (float64)v->y);
}

void RegisterVec2Module()
{
    const char *mod = "std/wren/math/vec2";
    const char *cls = "Vec2";

    RegisterClass_(mod, cls, Vec2_Allocate, NULL);

    RegisterMethod(mod, cls, false, "x", Vec2_GetX);
    RegisterMethod(mod, cls, false, "y", Vec2_GetY);
    RegisterMethod(mod, cls, false, "x=(_)", Vec2_SetX);
    RegisterMethod(mod, cls, false, "y=(_)", Vec2_SetY);
}