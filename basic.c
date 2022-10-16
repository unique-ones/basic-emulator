#include <libretro/retro.h>

int main(int argc, char** argv) {
    display_t display;
    display_create(&display, "Emulator", 800, 600);

    // input buffer might be moved to the emulator
    input_buffer_t input_buffer;
    input_buffer_create(&input_buffer, 128);
    display_input(&display, &input_buffer);

    // text renderer with apple2 printchar21 font
    text_renderer_t text_renderer;
    text_renderer_create(&text_renderer, "assets/pc21.ttf");
    renderer_clear_color(&(f32vec4_t){ 0.1f, 0.1f, 0.1f, 1.0f });

    // we might need to pass the emulator instance to the display, as we need to check it's state to delegate the input
    // accordingly - if the emulator is in execution state, we have to write to the program memory, if the emulator is
    // in input state, we have to write to the input buffer and to the program memory
    emulator_t* emulator = emulator_new();

    while (display_running(&display)) {
        // stage 1
        // check for incoming input
        display_update_input();

        // stage 2
        // input processing
        if (input_buffer.submit) {
            // delegate to emulator
            // maybe tokenize input before execution
            input_buffer.submit = false;
        }

        renderer_clear();
        f32mat4_t orthogonal;
        f32mat4_create_orthogonal(&orthogonal, 0.0f, (f32) display.width, (f32) display.height, 0.0f);
        shader_uniform_f32mat4(&text_renderer.renderer.shader, "uniform_transform", &orthogonal);

        // stage 3, render graphics
        switch (emulator->state) {
            case EMULATOR_STATE_INPUT: {
                // if the emulator is in input state, we just draw the command prompt and the user's input
                text_renderer_draw_input(&text_renderer, &input_buffer, &(f32vec2_t){ 0.0f, 0.0f },
                                         &(f32vec3_t){ 0.0f, 1.0f, 0.0f }, 0.5f);
                break;
            }
            case EMULATOR_STATE_EXECUTION: {
                // here we need a asynchronous renderer api, where we check if any draw-call was submitted from the
                // emulator thread, basically all we need to do here is call renderer_end_batch or something like that,
                // as any other renderer function only prepares data. it is important that we only call opengl code from
                // the main thread
                break;
            }
            default:
                break;
        }

        display_update_frame(&display);
    }

    // don't be a dork, free your resources :)
    emulator_free(emulator);
    input_buffer_destroy(&input_buffer);
    text_renderer_destroy(&text_renderer);
    display_destroy(&display);

    return 0;
}