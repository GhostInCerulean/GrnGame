import "std/wren/core/guard" for Guard
import "std/wren/renderer/font" for Font
import "std/wren/math/vec2" for Vec2

class Text {
    foreign static draw_(font, text, pos, space_between_letters, max_width, coeff, alpha)

    static draw(font, text, pos, space_between_letters, max_width, coeff, alpha) {
        if (!Guard.isType(font, Font, "Text.draw: 'font' must be a Font")) return
        if (!Guard.isType(text, String, "Text.draw: 'text' must be a String")) return
        if (!Guard.isType(pos, Vec2, "Text.draw: 'pos' must be a Vec2")) return
        if (!Guard.isType(space_between_letters, Num, "Text.draw: 'space_between_letters' must be a Num")) return
        if (!Guard.isType(max_width, Num, "Text.draw: 'max_width' must be a Num")) return
        if (!Guard.isType(coeff, Num, "Text.draw: 'coeff' must be a Num")) return
        if (!Guard.isType(alpha, Num, "Text.draw: 'alpha' must be a Num")) return

        return draw_(font, text, pos, space_between_letters, max_width, coeff, alpha)
    }
}
