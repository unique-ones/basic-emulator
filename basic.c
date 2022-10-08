#include <libretro/retro.h>

const f32vec3_t COLOR_RED = { 1.0f, 0.0f, 0.0f };
const f32vec3_t COLOR_GREEN = { 0.0f, 1.0f, 0.0f };
const f32vec3_t COLOR_BLUE = { 0.0f, 0.0f, 1.0f };
const f32vec3_t COLOR_WHITE = { 1.0f, 1.0f, 1.0f };

const char* BASIC_PROGRAM =
        "]LIST\n\n10    HOME\n"
        "20    LET W = 40: LET H = 24\n"
        "30    LET X = 20: LET Y = 12\n"
        "40    HTAB X: VTAB Y\n"
        "50    PRINT \"S\"\n"
        "60    GOSUB 1000\n"
        "70    GOTO 40\n"
        "1000  LET K = PEEK(49152)\n"
        "1010  IF K = 196 THEN X = X + 1\n"
        "1020  IF K = 193 THEN X = X - 1\n"
        "1030  IF K = 215 THEN Y = Y - 1\n"
        "1040  IF K = 211 THEN Y = Y + 1\n"
        "1050  RETURN";

int main(int argc, char** argv) {
    display_t display;
    display_create(&display, "Applesoft-BASIC Emulator", 800, 600);

    text_renderer_t text_renderer;
    text_renderer_create(&text_renderer, "assets/pc21.ttf");

    while (display_running(&display)) {
        renderer_clear();
        renderer_clear_color(&(f32vec4_t){ 0.1f, 0.1f, 0.1f, 1.0f });

        // Orthogonal projection matrix
        f32mat4_t orthogonal;
        f32mat4_create_ortho(&orthogonal, 0.0f, (f32) display.width, (f32) display.height, 0.0f);
        shader_uniform_f32mat4(&text_renderer.renderer.shader, "uniform_transform", &orthogonal);

        f32vec2_t text_position = { 0.0f, 0.0f };
        text_renderer_draw_text(&text_renderer, BASIC_PROGRAM, &text_position, &COLOR_GREEN, 0.5f);
        display_update(&display);
    }
    display_destroy(&display);
    return 0;
}