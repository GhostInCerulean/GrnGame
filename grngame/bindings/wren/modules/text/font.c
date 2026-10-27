#include "cglm/types-struct.h"
#include "grngame/bindings/wren/modules/vec2_module.h"
#include "grngame/core/app.h"
#include "grngame/dev/logging.h"
#include "grngame/utils/clear.h"
#include "wren.h"

#include <stdlib.h>
#include <string.h>

// Font.new(name, case_size, space_size)
void Font_Allocate(WrenVM *vm)
{
    int32 count = wrenGetSlotCount(vm);
    Font *font = (Font *)wrenSetSlotNewForeign(vm, 0, 0, sizeof(Font));
    CLEAR_PTR(font, 0);
    font->name = "font";
    font->size_of_case = (vec2s){.x = 8, .y = 8};
    font->space_size = 4; // (8 / 2)
    if (count >= 2 && wrenGetSlotType(vm, 1) == WREN_TYPE_STRING)
    {
        const char *name = wrenGetSlotString(vm, 1);
        if (name)
        {
            font->name = name;
        }
    }
    if (count >= 3)
    {
        const vec2s *case_size = wrenGetSlotVec2(vm, 2);
        if (case_size)
        {
            font->size_of_case = *case_size;
            font->space_size = (int32)(font->size_of_case.x / 2.0f);
        }
    }
    if (count >= 4 && wrenGetSlotType(vm, 3) == WREN_TYPE_NUM)
    {
        font->space_size = (int32)wrenGetSlotDouble(vm, 3);
    }
}

void Font_GetName(WrenVM *vm)
{
    Font *font = (Font *)wrenGetSlotForeign(vm, 0);
    wrenSetSlotString(vm, 0, font->name ? font->name : "");
}

void Font_GetCaseSize(WrenVM *vm)
{
    Font *font = (Font *)wrenGetSlotForeign(vm, 0);
    SetSlotVec2(vm, font->size_of_case);
}

void Font_SetCaseSize(WrenVM *vm)
{
    Font *font = (Font *)wrenGetSlotForeign(vm, 0);
    const vec2s *v = wrenGetSlotVec2(vm, 1);
    if (v)
        font->size_of_case = *v;
}

void Font_GetSpaceSize(WrenVM *vm)
{
    Font *font = (Font *)wrenGetSlotForeign(vm, 0);
    wrenSetSlotDouble(vm, 0, (double)font->space_size);
}

void Font_SetSpaceSize(WrenVM *vm)
{
    Font *font = (Font *)wrenGetSlotForeign(vm, 0);
    if (wrenGetSlotType(vm, 1) == WREN_TYPE_NUM)
    {
        font->space_size = (int32)wrenGetSlotDouble(vm, 1);
    }
    wrenSetSlotDouble(vm, 0, (double)font->space_size);
}

void Font_SetPattern(WrenVM *vm)
{
    Font *font = (Font *)wrenGetSlotForeign(vm, 0);

    if (wrenGetSlotType(vm, 1) != WREN_TYPE_LIST)
    {
        return;
    }

    int32 count = wrenGetListCount(vm, 1);
    if (count > FONT_PATTERN_MAX)
        count = FONT_PATTERN_MAX;

    PatternLetter tmp[FONT_PATTERN_MAX];
    CLEAR(tmp, 0);

    wrenEnsureSlots(vm, 3);
    for (int32 i = 0; i < count; i++)
    {
        wrenGetListElement(vm, 1, i, 2);
        const PatternLetter *item = (const PatternLetter *)wrenGetSlotForeign(vm, 2);
        if (!item)
        {
            return;
        }
        tmp[i] = *item;
    }

    memcpy(font->pattern, tmp, sizeof(tmp));
}

void Font_GetPattern(WrenVM *vm)
{
    Font *font = (Font *)wrenGetSlotForeign(vm, 0);
    wrenEnsureSlots(vm, 3);
    wrenSetSlotNewList(vm, 0);
    wrenGetVariable(vm, "std/wren/renderer/pattern_letter", "PatternLetter", 1);

    for (int32 i = 0; i < FONT_PATTERN_MAX; i++)
    {
        PatternLetter *pl = (PatternLetter *)wrenSetSlotNewForeign(vm, 2, 1, sizeof(PatternLetter));
        *pl = font->pattern[i];
        wrenInsertInList(vm, 0, -1, 2);
    }
}

void RegisterFontModule()
{
    const char *font_mod = "std/wren/renderer/font";
    const char *font_cls = "Font";

    RegisterClass_(font_mod, font_cls, Font_Allocate, NULL);
    RegisterMethod(font_mod, font_cls, false, "name", Font_GetName);
    RegisterMethod(font_mod, font_cls, false, "case_size", Font_GetCaseSize);
    RegisterMethod(font_mod, font_cls, false, "case_size=(_)", Font_SetCaseSize);
    RegisterMethod(font_mod, font_cls, false, "space_size", Font_GetSpaceSize);
    RegisterMethod(font_mod, font_cls, false, "space_size=(_)", Font_SetSpaceSize);
    RegisterMethod(font_mod, font_cls, false, "setPattern_(_)", Font_SetPattern);
    RegisterMethod(font_mod, font_cls, false, "pattern", Font_GetPattern);
}