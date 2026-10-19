// todo : factorise
#include "text.h"
#include "../math/types.h"
#include "cglm/types-struct.h"
#include "grngame/assets/asset_manager.h"
#include "grngame/dev/logging.h"
#include "grngame/renderer/sprite.h"
#include "grngame/utils/attributes.h"
#include "grngame/utils/string_compat.h"
#include <SDL3_ttf/SDL_ttf.h>

static const PatternLetter DEFAULT_FONT_PATTERN[] = {
    {'A', 0, 0}, {'B', 0, 0}, {'C', 0, 0}, {'D', 0, 0}, {'E', 0, 0},  {'F', 0, 0}, {'G', 0, 0}, {'H', 0, 0},
    {'I', 0, 0}, {'J', 0, 0}, {'K', 0, 0}, {'L', 0, 0}, {'M', 0, 0},  {'N', 0, 0}, {'O', 0, 0}, {'P', 0, 0},
    {'Q', 0, 0}, {'R', 0, 0}, {'S', 0, 0}, {'T', 0, 0}, {'U', 0, 0},  {'V', 0, 0}, {'W', 0, 0}, {'X', 0, 0},
    {'Y', 0, 0}, {'Z', 0, 0}, {'a', 0, 0}, {'b', 0, 0}, {'c', 0, 0},  {'d', 0, 0}, {'e', 0, 0}, {'f', 0, 0},
    {'g', 0, 0}, {'h', 0, 0}, {'i', 0, 0}, {'j', 0, 0}, {'k', 0, 0},  {'l', 0, 0}, {'m', 0, 0}, {'n', 0, 0},
    {'o', 0, 0}, {'p', 0, 0}, {'q', 0, 0}, {'r', 0, 0}, {'s', 0, 0},  {'t', 0, 0}, {'u', 0, 0}, {'v', 0, 0},
    {'w', 0, 0}, {'x', 0, 0}, {'y', 0, 0}, {'z', 0, 0}, {'0', 0, 0},  {'1', 0, 0}, {'2', 0, 0}, {'3', 0, 0},
    {'4', 0, 0}, {'5', 0, 0}, {'6', 0, 0}, {'7', 0, 0}, {'8', 0, 0},  {'9', 0, 0}, {'.', 0, 0}, {',', 0, 0},
    {'!', 0, 0}, {'?', 0, 0}, {':', 0, 0}, {';', 0, 0}, {'\'', 0, 0}, {'-', 0, 0}, {'(', 0, 0}, {')', 0, 0},
    {' ', 0, 0}, {'\0', 0, 0}};

static inline int32 FindPatternLetter(const PatternLetter *pattern, char c)
{
    // todo o(1)
    for (int32 i = 0; pattern[i].letter != '\0'; i++)
    {
        if (pattern[i].letter == c)
            return i;
    }
    return -1;
}

static float32 GetWordWidth(const char *word, const PatternLetter *pattern, uint16 char_w, int32 space_between_letters,
                            uint8 c)
{
    float32 width = 0.0f;
    for (uint32 i = 0; word[i] != '\0' && word[i] != ' ' && word[i] != '\n'; i++)
    {
        int32 idx = FindPatternLetter(pattern, word[i]);
        if (idx < 0)
        {
            width += (float32)((char_w + space_between_letters) * c);
            continue;
        }

        int16 x_left = pattern[idx].x_left;
        int16 y_right = pattern[idx].y_right;

        width += (float32)(((int32)char_w + x_left + y_right + space_between_letters) * c);
    }

    if (width > 0.0f)
    {
        width -= (float32)(space_between_letters * c);
    }
    return width;
}

bool TextDraw(const char *font_name, const PatternLetter *pattern, const char *text, float32 x, float32 y,
              uint16 char_w, uint16 char_h, int32 space_between_letters, int32 max_width, uint8 c, uint8 a,
              int32 *size_out_w, int32 *size_out_h)
{
    if (UNLIKELY(!font_name || !text))
    {
        if (size_out_w)
            *size_out_w = 0;
        if (size_out_h)
            *size_out_h = 0;
        return false;
    }

    if (!pattern || pattern[0].letter == '\0')
    {
        LOG_WARNING("Pattern is NULL or empty for font '%s', using default pattern.", font_name);
        pattern = DEFAULT_FONT_PATTERN;
    }

    bool find = false;

    // set manually texture to do better log
    Texture tex = FindImage(font_name, &find);
    if (!find)
    {
        LOG_WARNING("Impossible to get font texture: %s", font_name);
        if (size_out_w)
            *size_out_w = 0;
        if (size_out_h)
            *size_out_h = 0;
        return false;
    }

    Sprite s = {.w = char_w, .h = char_h, .tex = tex, .find = true, .name = font_name};

    float32 start_x = x;
    float32 start_y = y;
    float32 cursor_x = start_x;
    float32 cursor_y = start_y;
    float32 line_height = (float32)(char_h * c);

    float32 max_line_width = 0.0f;
    float32 current_line_width = 0.0f;
    bool all_drawn = true;

    for (uint32 i = 0; text[i] != '\0'; i++)
    {
        if (text[i] == '\n')
        {
            if (current_line_width > max_line_width)
                max_line_width = current_line_width;

            cursor_x = start_x;
            cursor_y += line_height;
            current_line_width = 0.0f;
            continue;
        }

        if (max_width > 0 && text[i] != ' ' && (i == 0 || text[i - 1] == ' ' || text[i - 1] == '\n'))
        {
            float32 word_w = GetWordWidth(&text[i], pattern, char_w, space_between_letters, c);
            if (cursor_x > start_x && (cursor_x - start_x + word_w) > (float32)max_width)
            {
                if (current_line_width > max_line_width)
                    max_line_width = current_line_width;

                cursor_x = start_x;
                cursor_y += line_height;
                current_line_width = 0.0f;
            }
        }

        int32 idx = FindPatternLetter(pattern, text[i]);
        int16 x_left = (idx >= 0) ? pattern[idx].x_left : 0;
        int16 y_right = (idx >= 0) ? pattern[idx].y_right : 0;

        float32 char_visual_width = (float32)(((int32)char_w + x_left + y_right) * c);
        if (max_width > 0 && cursor_x > start_x && (cursor_x - start_x + char_visual_width) > (float32)max_width)
        {
            if (current_line_width > max_line_width)
                max_line_width = current_line_width;

            cursor_x = start_x;
            cursor_y += line_height;
            current_line_width = 0.0f;
            if (text[i] == ' ')
                continue;
        }

        if (idx < 0)
        {
            LOG_DEBUG("Character '%c' not found in pattern for font %s", text[i], font_name);
            current_line_width = (cursor_x - start_x) + (float32)(char_w * c);
            cursor_x += (float32)((char_w + space_between_letters) * c);
            continue;
        }

        uint16 frame = (uint16)idx;

        cursor_x += (float32)(x_left * c);
        if (!SpriteDraw(s, frame, cursor_x, cursor_y, c, 0, a))
        {
            all_drawn = false;
        }

        current_line_width = (cursor_x - start_x) + (float32)(((int32)char_w + y_right) * c);
        cursor_x += (float32)(((int32)char_w + y_right + space_between_letters) * c);
    }

    if (current_line_width > max_line_width)
    {
        max_line_width = current_line_width;
    }

    if (text[0] == '\0')
    {
        if (size_out_w)
            *size_out_w = 0;
        if (size_out_h)
            *size_out_h = 0;
    }
    else
    {
        if (size_out_w)
            *size_out_w = (int32)max_line_width;
        if (size_out_h)
            *size_out_h = (int32)((cursor_y - start_y) + line_height);
    }

    return all_drawn;
}