import "std/wren/core/event" for Event
import "std/wren/core/time" for Time
import "std/wren/dev/log" for Log
import "std/wren/renderer/text" for Text
import "std/wren/renderer/pattern_letter" for PatternLetter

class Main {
static on_start() {
    
}

static on_update(dt) {
}
static on_fixed_update(dt) {}
static on_render() {
    Text.draw_("font",null,"hello world",30,0,30,48,1,0,1,255)

    Text.draw_("font",null,"hello frommm grngame",0,50,30,48,1,180,1,100)
}
static on_destroy() {}


}