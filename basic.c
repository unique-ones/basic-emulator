#include <libretro/retro.h>

int main(int argc, char** argv) {
    display_t display;
    display.title = "basic emulator";
    display.width = 600;
    display.height = 400;
    display_create(&display);

    shader_type_t attributes[2] = { FLOAT3, FLOAT3 };
    vertex_buffer_layout_t layout;
    layout.attributes = attributes;
    layout.count = STACK_ARRAY_SIZE(attributes);

    vertex_buffer_t vertex_buffer;
    vertex_buffer_create(&vertex_buffer);
    vertex_buffer_layout(&vertex_buffer, &layout);
    vertex_buffer_destroy(&vertex_buffer);

    while (display_running(&display)) {
        display_update(&display);
    }
    display_destroy(&display);
    return 0;
}