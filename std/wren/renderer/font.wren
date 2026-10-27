import "std/wren/core/guard" for Guard
import "std/wren/renderer/pattern_letter" for PatternLetter

foreign class Font {
    construct new(name, case_size, space_size) {}

    foreign name
    foreign case_size
    foreign case_size=(v)
    foreign space_size
    foreign space_size=(v)
    foreign pattern
    foreign setPattern_(list)

    pattern=(list) {
        if (!Guard.isType(list, List, "Font.pattern: must be a List")) return

        for (letter in list) {
            if (!Guard.isType(letter, PatternLetter, "Font.pattern: every element must be a PatternLetter")) return
        }       
        setPattern_(list)
        return list
    }

    toString{"name:%(name),size:%(case_size),space:%(space_size),pattern:%(pattern)"}
}
