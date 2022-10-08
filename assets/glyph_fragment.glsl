#version 450 core
layout (location = 0) in vec3 passed_color;
layout (location = 1) in vec2 passed_texture_coordinates;
layout (location = 0) out vec4 out_color;

uniform sampler2D uniform_glyph_atlas;

void main() {
    out_color = vec4(passed_color, texture(uniform_glyph_atlas, passed_texture_coordinates).r);
}
