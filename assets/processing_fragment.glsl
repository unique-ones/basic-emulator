#version 450 core
layout (location = 0) out vec4 out_color;
layout (location = 0) in vec3 passed_color;
layout (location = 1) in vec2 passed_texture_coordinates;

uniform sampler2D uniform_frame;

void main() {
    vec3 color = texture(uniform_frame, passed_texture_coordinates).rgb * passed_color;
    out_color = vec4(color, 1.0);
}
