#pragma once

#include "cglm/types-struct.h"
#include "grngame/math/types.h"
#include "grngame/utils/c_cpp.h"
#include <stdbool.h>

BEGIN_DECLARATIONS

#define FONT_PATTERN_MAX 128 // size of a char

// intern
typedef struct TextLayout
{
    float32 start_x;
    float32 start_y;
    float32 cursor_x;
    float32 cursor_y;
    float32 line_height;
    float32 current_line_width;
    float32 max_line_width;
    bool line_start;
} TextLayout;

typedef struct PatternLetter
{
    char letter;
    vec2s size;     // x = left, y = right
    int32 y_offset; // offset for ex: g, p, q, y
} PatternLetter;

typedef struct Font
{
    const char *name;
    PatternLetter pattern[FONT_PATTERN_MAX];
    vec2s size_of_case;
    int32 space_size;
} Font;

bool TextDraw(const Font *font, const char *text, vec2s position, int32 space_between_letters, int32 max_width,
              uint8 coeff, uint8 alpha, vec2s *size_out);

END_DECLARATIONS