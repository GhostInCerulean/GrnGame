#pragma once

#include "cglm/types-struct.h"
#include "grngame/math/types.h"
#include "grngame/utils/c_cpp.h"
#include <SDL3_ttf/SDL_ttf.h>

BEGIN_DECLARATIONS

typedef struct PaternLetter
{
    char letter;
    int16 x_left;  // + add more space - add less space
    int16 y_right; // + add more space - add less space
} PatternLetter;

bool TextDraw(const char *font_name, const PatternLetter *pattern, const char *text, float32 x, float32 y,
              uint16 char_w, uint16 char_h, int32 space_between_letters, int32 max_width, uint8 coeff, uint8 alpha,
              int32 *size_out_w, int32 *size_out_h);

END_DECLARATIONS