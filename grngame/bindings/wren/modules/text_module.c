#include "grngame/core/app.h"
#include "wren.h"
#include <stdio.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void PatternLetter_Allocate(WrenVM *vm)
{
    PatternLetter *pl = (PatternLetter *)wrenSetSlotNewForeign(vm, 0, 0, sizeof(PatternLetter));
    int32 count = wrenGetSlotCount(vm);

    if (count >= 2 && wrenGetSlotType(vm, 1) == WREN_TYPE_STRING)
    {
        const char *str = wrenGetSlotString(vm, 1);
        pl->letter = (str && str[0] != '\0') ? str[0] : '\0';
    }
    else
    {
        pl->letter = '\0';
    }

    if (count >= 4)
    {
        pl->x_left = (int16)wrenGetSlotDouble(vm, 2);
        pl->y_right = (int16)wrenGetSlotDouble(vm, 3);
    }
    else
    {
        pl->x_left = 0;
        pl->y_right = 0;
    }
}

void PatternLetter_Init(WrenVM *vm)
{
    (void)vm;
}

void PatternLetter_GetLetter(WrenVM *vm)
{
    PatternLetter *pl = (PatternLetter *)wrenGetSlotForeign(vm, 0);
    char str[2] = {pl->letter, '\0'};
    wrenSetSlotString(vm, 0, str);
}

void PatternLetter_GetXLeft(WrenVM *vm)
{
    PatternLetter *pl = (PatternLetter *)wrenGetSlotForeign(vm, 0);
    wrenSetSlotDouble(vm, 0, (float64)pl->x_left);
}

void PatternLetter_GetYRight(WrenVM *vm)
{
    PatternLetter *pl = (PatternLetter *)wrenGetSlotForeign(vm, 0);
    wrenSetSlotDouble(vm, 0, (float64)pl->y_right);
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

void PatternLetter_SetXLeft(WrenVM *vm)
{
    PatternLetter *pl = (PatternLetter *)wrenGetSlotForeign(vm, 0);
    pl->x_left = (int16)wrenGetSlotDouble(vm, 1);
    wrenSetSlotDouble(vm, 0, (float64)pl->x_left);
}

void PatternLetter_SetYRight(WrenVM *vm)
{
    PatternLetter *pl = (PatternLetter *)wrenGetSlotForeign(vm, 0);
    pl->y_right = (int16)wrenGetSlotDouble(vm, 1);
    wrenSetSlotDouble(vm, 0, (float64)pl->y_right);
}

void PatternLetter_ToString(WrenVM *vm)
{
    PatternLetter *pl = (PatternLetter *)wrenGetSlotForeign(vm, 0);
    char buffer[255];
    snprintf(buffer, sizeof(buffer), "'%c':%d, %d", pl->letter, pl->x_left, pl->y_right);
    wrenSetSlotString(vm, 0, buffer);
}

void draw_text(WrenVM *vm)
{
    // Slot 0 = Classe Text
    const char *font_name = wrenGetSlotString(vm, 1);
    // Slot 2 = pattern (List ou Null)
    const char *text = wrenGetSlotString(vm, 3);
    float32 x = (float32)wrenGetSlotDouble(vm, 4);
    float32 y = (float32)wrenGetSlotDouble(vm, 5);
    uint16 char_w = (uint16)wrenGetSlotDouble(vm, 6);
    uint16 char_h = (uint16)wrenGetSlotDouble(vm, 7);
    int32 space_between_letters = (int32)wrenGetSlotDouble(vm, 8);
    int32 max_width = (int32)wrenGetSlotDouble(vm, 9);
    uint8 c = (uint8)wrenGetSlotDouble(vm, 10);
    uint8 a = (uint8)wrenGetSlotDouble(vm, 11);

    const PatternLetter *pattern_ptr = NULL;
    PatternLetter stack_pattern[256];

    if (wrenGetSlotType(vm, 2) == WREN_TYPE_LIST)
    {
        int32 list_count = wrenGetListCount(vm, 2);
        if (list_count > 255)
            list_count = 255;

        wrenEnsureSlots(vm, 13);
        for (int32 i = 0; i < list_count; i++)
        {
            wrenGetListElement(vm, 2, i, 12);
            PatternLetter *item = (PatternLetter *)wrenGetSlotForeign(vm, 12);
            if (item)
                stack_pattern[i] = *item;
        }
        // need to end by '\0'
        stack_pattern[list_count] = (PatternLetter){'\0', 0, 0};
        pattern_ptr = stack_pattern;
    }

    int32 out_w = 0;
    int32 out_h = 0;
    TextDraw(font_name, pattern_ptr, text, x, y, char_w, char_h, space_between_letters, max_width, c, a, &out_w,
             &out_h);

    wrenEnsureSlots(vm, 2);
    wrenSetSlotNewList(vm, 0);

    wrenSetSlotDouble(vm, 1, (float64)out_w);
    wrenInsertInList(vm, 0, -1, 1);

    wrenSetSlotDouble(vm, 1, (float64)out_h);
    wrenInsertInList(vm, 0, -1, 1);
}

void RegisterTextModule()
{
    const char *pattern_mod = "std/wren/renderer/pattern_letter";
    const char *pattern_cls = "PatternLetter";

    RegisterClass_(pattern_mod, pattern_cls, PatternLetter_Allocate, NULL);

    RegisterMethod(pattern_mod, pattern_cls, false, "init new(_)", PatternLetter_Init);
    RegisterMethod(pattern_mod, pattern_cls, false, "init new(_,_,_)", PatternLetter_Init);
    RegisterMethod(pattern_mod, pattern_cls, false, "letter", PatternLetter_GetLetter);
    RegisterMethod(pattern_mod, pattern_cls, false, "letter=(_)", PatternLetter_SetLetter);
    RegisterMethod(pattern_mod, pattern_cls, false, "x_left", PatternLetter_GetXLeft);
    RegisterMethod(pattern_mod, pattern_cls, false, "x_left=(_)", PatternLetter_SetXLeft);
    RegisterMethod(pattern_mod, pattern_cls, false, "y_right", PatternLetter_GetYRight);
    RegisterMethod(pattern_mod, pattern_cls, false, "y_right=(_)", PatternLetter_SetYRight);
    RegisterMethod(pattern_mod, pattern_cls, false, "toString", PatternLetter_ToString);

    const char *module = "std/wren/renderer/text";
    const char *cls = "Text";
    bool is_static = true;

    RegisterMethod(module, cls, is_static, "draw_(_,_,_,_,_,_,_,_,_,_,_)", draw_text);
}