#version 450 core
layout (location = 0) in vec3 passed_color;
layout (location = 0) out vec4 out_color;

void main() {
    out_color = vec4(passed_color, 1.0);
}
