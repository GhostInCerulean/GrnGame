foreign class Vec2 {
    construct new() {}
    construct new(x, y) {}

    foreign x
    foreign y
    foreign x=(v)
    foreign y=(v)

    +(other) {
        if (other is Num) {
            return Vec2.new(x + other, y + other)
        }
        return Vec2.new(x + other.x, y + other.y)
    }

    *(other) {
        if (other is Num) {
            return Vec2.new(x * other, y * other)
        }
        return Vec2.new(x * other.x, y * other.y)
    }

    -(other) {
        if (other is Num) {
            return Vec2.new(x - other, y - other)
        }
        return Vec2.new(x - other.x, y - other.y)
    }

    /(other) {
        if (other is Num) {
            return Vec2.new(x / other, y / other)
        }
        return Vec2.new(x / other.x, y / other.y)
    }

    toString { "%(x),%(y)" }
}