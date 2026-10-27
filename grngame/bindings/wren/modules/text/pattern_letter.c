#include "cglm/types-struct.h"
#include "grngame/bindings/wren/modules/vec2_module.h"
#include "grngame/core/app.h"
#include "grngame/dev/logging.h"
#include "grngame/utils/clear.h"
#include "wren.h"

#include <stdlib.h>
#include <string.h>

void PatternLetter_Allocate(WrenVM *vm)
{
    PatternLetter *pl = (PatternLetter *)wrenSetSlotNewForeign(vm, 0, 0, sizeof(PatternLetter));
    int32 count = wrenGetSlotCount(vm);

    pl->letter = '\0';
    pl->size = (vec2s){.x = 0, .y = 0};
    pl->y_offset = 0;

    // Slot 1 = letter
    if (count >= 2 && wrenGetSlotType(vm, 1) == WREN_TYPE_STRING)
    {
        const char *str = wrenGetSlotString(vm, 1);
        pl->letter = (str && str[0] != '\0') ? str[0] : '\0';
    }

    // Slot 2 = size (Vec2)
    if (count >= 3)
    {
        const vec2s *size = wrenGetSlotVec2(vm, 2);
        if (size)
            pl->size = *size;
    }

    // Slot 3 = y_offset (Num)
    if (count >= 4 && wrenGetSlotType(vm, 3) == WREN_TYPE_NUM)
    {
        pl->y_offset = (int32)wrenGetSlotDouble(vm, 3);
    }
}

void PatternLetter_GetLetter(WrenVM *vm)
{
    PatternLetter *pl = (PatternLetter *)wrenGetSlotForeign(vm, 0);
    char str[2] = {pl->letter, '\0'};
    wrenSetSlotString(vm, 0, str);
}

void PatternLetter_SetLetter(WrenVM *vm)
{
    PatternLetter *pl = (PatternLetter *)wrenGetSlotForeign(vm, 0);
    if (wrenGetSlotType(vm, 1) == WREN_TYPE_STRING)
    {
        const char *str = wrenGetSlotString(vm, 1);
        pl->letter = (str && str[0] != '\0') ? str[0] : '\0';
    }
    char out[2] = {pl->letter, '\0'};
    wrenSetSlotString(vm, 0, out);
}

void PatternLetter_GetSize(WrenVM *vm)
{
    PatternLetter *pl = (PatternLetter *)wrenGetSlotForeign(vm, 0);
    SetSlotVec2(vm, pl->size);
}

void PatternLetter_SetSize(WrenVM *vm)
{
    PatternLetter *pl = (PatternLetter *)wrenGetSlotForeign(vm, 0);
    const vec2s *v = wrenGetSlotVec2(vm, 1);
    if (v)
        pl->size = *v;
}

void PatternLetter_GetYOffset(WrenVM *vm)
{
    PatternLetter *pl = (PatternLetter *)wrenGetSlotForeign(vm, 0);
    wrenSetSlotDouble(vm, 0, (float64)pl->y_offset);
}

void PatternLetter_SetYOffset(WrenVM *vm)
{
    PatternLetter *pl = (PatternLetter *)wrenGetSlotForeign(vm, 0);
    if (wrenGetSlotType(vm, 1) == WREN_TYPE_NUM)
    {
        pl->y_offset = (int32)wrenGetSlotDouble(vm, 1);
    }
    wrenSetSlotDouble(vm, 0, (float64)pl->y_offset);
}

void RegisterPatternLetterModule()
{
    const char *pattern_mod = "std/wren/renderer/pattern_letter";
    const char *pattern_cls = "PatternLetter";

    RegisterClass_(pattern_mod, pattern_cls, PatternLetter_Allocate, NULL);
    RegisterMethod(pattern_mod, pattern_cls, false, "letter", PatternLetter_GetLetter);
    RegisterMethod(pattern_mod, pattern_cls, false, "letter=(_)", PatternLetter_SetLetter);
    RegisterMethod(pattern_mod, pattern_cls, false, "size", PatternLetter_GetSize);
    RegisterMethod(pattern_mod, pattern_cls, false, "size=(_)", PatternLetter_SetSize);
    RegisterMethod(pattern_mod, pattern_cls, false, "y_offset", PatternLetter_GetYOffset);
    RegisterMethod(pattern_mod, pattern_cls, false, "y_offset=(_)", PatternLetter_SetYOffset);
}