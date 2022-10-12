#include <libretro/retro.h>

int main(int argc, char** argv) {
    display_t display;
    display_create(&display, "Emulator", 800, 600);

    input_buffer_t input_buffer;
    input_buffer_create(&input_buffer, 128);
    display_input(&display, &input_buffer);

    text_renderer_t text_renderer;
    text_renderer_create(&text_renderer, "assets/pc21.ttf");

    while (display_running(&display)) {
        renderer_clear();
        renderer_clear_color(&(f32vec4_t){ 0.1f, 0.1f, 0.1f, 1.0f });
        // Orthogonal projection matrix
        f32mat4_t orthogonal;
        f32mat4_create_orthogonal(&orthogonal, 0.0f, (f32) display.width, (f32) display.height, 0.0f);
        shader_uniform_f32mat4(&text_renderer.renderer.shader, "uniform_transform", &orthogonal);
        text_renderer_draw_input(&text_renderer, &input_buffer, &(f32vec2_t){ 0.0f, 0.0f },
                                 &(f32vec3_t){ 0.0f, 1.0f, 0.0f }, 0.5f);
        display_update(&display);
    }

    input_buffer_destroy(&input_buffer);
    text_renderer_destroy(&text_renderer);
    display_destroy(&display);

    return 0;
}