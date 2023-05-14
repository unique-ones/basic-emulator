#version 450 core
layout (location = 0) in vec3 attrib_position;
layout (location = 1) in vec3 attrib_color;
layout (location = 2) in vec2 attrib_texture_coordinates;
layout (location = 0) out vec3 passed_color;

uniform mat4 uniform_transform;

void main() {
    gl_Position = uniform_transform * vec4(attrib_position, 1.0f);
    passed_color = attrib_color;
}
