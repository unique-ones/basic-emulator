// Copyright (c) 2025 Elias Engelbert Plank

#include "core/display.h"
#include "gpu/renderer.h"
#include "core/emu.h"

int main() {
    Display display;
    display_create(&display, "Emulator", 800, 600);

    // renderer with apple2 pc21 font
    Renderer renderer;
    renderer_create(&renderer, "assets/pc21.ttf");
    renderer_clear_color(&(F32Vector4) { 0.05f, 0.05f, 0.05f, 1.0f });

    Emulator emulator;
    emulator_create(&emulator, &renderer);

    // set up callbacks
    display_callback_argument(&display, &emulator);
    display_key_callback(&display, emulator_key_callback);
    display_char_callback(&display, emulator_char_callback);

    F32Vector3 const amber = { 1.0f, 0.6f, 0.0f };
    F32Vector3 const amber_dimmed = { 0.9f, 0.5f, 0.0f };

    while (display_running(&display)) {
        renderer_resize(&renderer, display.width, display.height);

        // CRT rendering can be toggled with F2
        if (emulator.enable_crt) {
            renderer_crt_begin_capture(&renderer);
        }
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
                // history
                F32Vector2 position_iterator = { 30.0f, 30.0f };
                for (TextEntry *it = emulator.history->begin; it; it = it->next) {
                    position_iterator.x = 30.0f;
                    renderer_begin_batch(&renderer);
                    renderer_draw_text(&renderer, &position_iterator, &amber_dimmed, 0.5f, "]%.*s", it->length,
                                       it->data);
                    renderer_end_batch(&renderer);
                }

                // reset draw cursor
                position_iterator.x = 30.0f;

                // draw current input buffer
                renderer_begin_batch(&renderer);
                renderer_draw_text_with_cursor(&renderer, &position_iterator, &amber, 0.5f, emulator.text.cursor,
                                               "%.*s", emulator.text.fill, emulator.text.data);
                renderer_end_batch(&renderer);

                // should definitely document this more, here we must clear the render group,
                // due to the fact that we might draw the emulator text output in the next frame,
                // which would result in left over render commands from our input text
                renderer_begin_batch(&renderer);
                break;
            }
            case EMULATOR_STATE_EXECUTION: {
                // if the emulator is in execution state, we check the render group for commands and flush
                // them to the graphics card, do not clear the render groups as the emulator must
                // decide that for himself
                renderer_end_batch(&renderer);
                break;
            }
        }

        if (emulator.enable_crt) {
            renderer_crt_end_capture(&renderer);
        }

        // stage 3
        // check for incoming input
        display_update_input(&display);
        display_update_frame(&display);
    }

    // don't be a dork, free your resources :)
    emulator_destroy(&emulator);
    renderer_destroy(&renderer);
    display_destroy(&display);

    return 0;
}
