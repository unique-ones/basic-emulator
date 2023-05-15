#include <libretro/retro.h>

int main(int argc, char** argv) {
    display_t display;
    display_create(&display, "Emulator", 800, 600);

    // renderer with apple2 pc21 font
    renderer_t renderer;
    renderer_create(&renderer, "assets/pc21.ttf");
    renderer_clear_color(&(f32vec4_t){ 0.1f, 0.1f, 0.1f, 1.0f });

    emulator_t emulator;
    emulator_create(&emulator, &renderer);

    // set up callbacks
    display_callback_argument(&display, &emulator);
    display_key_callback(&display, emulator_key_callback);
    display_char_callback(&display, emulator_char_callback);

    u32 fps_display_counter = 0;
    while (display_running(&display)) {
        f32mat4_t orthogonal;
        f32mat4_create_orthogonal(&orthogonal, 0.0f, (f32) display.width, (f32) display.height, 0.0f);
        shader_uniform_f32mat4(&renderer.glyph_shader, "uniform_transform", &orthogonal);
        renderer_clear();

        // stage 1
        // input processing
        if (emulator.text.submit) {
            // spin off executing thread
            emulator_run(&emulator);
        }

        // stage 2, render graphics
        switch (emulator.state) {
            case EMULATOR_STATE_INPUT: {
                // if the emulator is in input state, we just draw the command prompt and the user's input
                renderer_begin_batch(&renderer);

                // history
                f32vec2_t position_iterator = { 0.0f, 0.0f };
                for (text_entry_t* it = emulator.history->begin; it; it = it->next) {
                    position_iterator.x = 0.0f;
                    renderer_draw_text_with_cursor(&renderer, &position_iterator, &(f32vec3_t){ 0.0f, 0.9f, 0.0f },
                                                   0.5f, UINT32_MAX, "%.*s", it->length, it->data);
                }

                position_iterator.x = 0.0f;
                renderer_draw_text_with_cursor(&renderer, &position_iterator, &(f32vec3_t){ 0.0f, 1.0f, 0.0f }, 0.5f,
                                               emulator.text.cursor, "%.*s", emulator.text.fill, emulator.text.data);
                renderer_end_batch(&renderer);
                break;
            }
            case EMULATOR_STATE_EXECUTION: {
                // if the emulator is in execution state, we check the render group for commands and flush
                // them to the graphics card
                renderer_begin_batch(&renderer);
                renderer_end_batch(&renderer);
                break;
            }
            default:
                break;
        }

        // stage 3
        // check for incoming input
        display_update_input(&display);
        f64 frame_time = display_update_frame(&display);
        if (fps_display_counter++ == 10000) {
            f64 fps = 1.0 / frame_time;
            char fps_text_buffer[128];
            snprintf(fps_text_buffer, sizeof fps_text_buffer, "Basic Emulator - %d FPS", (int) fps);
            display_title(&display, fps_text_buffer);
            fps_display_counter = 0;
        }
    }

    // don't be a dork, free your resources :)
    emulator_destroy(&emulator);
    renderer_destroy(&renderer);
    display_destroy(&display);

    return 0;
}