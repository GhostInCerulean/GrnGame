foreign class PatternLetter {
    construct new(letter, size, y_offset) {}
    construct new(letter, size) {}
    construct new(letter) {}

    foreign letter
    foreign letter=(v)
    foreign size
    foreign size=(v)
    foreign y_offset
    foreign y_offset=(v)

    toString { "%(letter):%(size):%(y_offset)" }
}