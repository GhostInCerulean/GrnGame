import "std/wren/core/event" for Event
import "std/wren/core/time" for Time
import "std/wren/dev/log" for Log
import "std/wren/renderer/text" for Text
import "std/wren/renderer/pattern_letter" for PatternLetter
import "std/wren/renderer/font" for Font
import "std/wren/math/vec2" for Vec2 

class Main {
    static on_start() {
        var font_pattern = [
            PatternLetter.new("H", Vec2.new(-1, -1),1),
            PatternLetter.new("L", Vec2.new(0, -3)),
            PatternLetter.new("W", Vec2.new(0, -1)),
            PatternLetter.new("E", Vec2.new(0, -3)),
            PatternLetter.new("O", Vec2.new(0, -2)),
            PatternLetter.new("R", Vec2.new(0, -2)),
            PatternLetter.new("D", Vec2.new(0, -3)),
        ]

        var size = 
        
        __font = Font.new("font", Vec2.new(8, 8), 3)
        __font.pattern = font_pattern

        __text_pos = Vec2.new(40, 90)
        System.print(__font)
    }

    static on_update(dt) {
    }

    static on_fixed_update(dt) {
    }

    static on_render() {
        Text.draw(
            __font,         // font (Font)
            "HELLO WORLD",  // text (String)
            __text_pos,     // pos (Vec2)
            1,              // space_between_letters (Num)
            300,            // max_width (Num)
            2,              // coeff (Num)
            255             // alpha (Num)
        )
    }

    static on_destroy() {
    }
}