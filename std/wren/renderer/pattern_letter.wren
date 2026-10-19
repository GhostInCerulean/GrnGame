foreign class PatternLetter {
    construct new(letter, xLeft, yRight) {}
    construct new(letter) {}

    foreign letter
    foreign x_left
    foreign y_right

    foreign letter=(v)
    foreign x_left=(v)
    foreign y_right=(v)

    foreign toString
}