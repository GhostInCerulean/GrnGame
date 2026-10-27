#include "cglm/types-struct.h"
#include "grngame/bindings/wren/modules/vec2_module.h"
#include "grngame/core/app.h"
#include "wren.h"

void draw_text(WrenVM *vm)
{
    const Font *font = (const Font *)wrenGetSlotForeign(vm, 1);
    const char *text = wrenGetSlotString(vm, 2);
    const vec2s *pos = wrenGetSlotVec2(vm, 3);
    int32 space_between_letters = (int32)wrenGetSlotDouble(vm, 4);
    int32 max_width = (int32)wrenGetSlotDouble(vm, 5);
    uint8 coeff = (uint8)wrenGetSlotDouble(vm, 6);
    uint8 alpha = (uint8)wrenGetSlotDouble(vm, 7);

    vec2s out = {.x = 0.0f, .y = 0.0f};
    TextDraw(font, text, *pos, space_between_letters, max_width, coeff, alpha, &out);

    SetSlotVec2(vm, out);
}

void RegisterTextModule()
{

    const char *module = "std/wren/renderer/text";
    const char *cls = "Text";
    bool is_static = true;

    RegisterMethod(module, cls, is_static, "draw_(_,_,_,_,_,_,_)", draw_text);
}