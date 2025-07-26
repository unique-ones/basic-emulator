#version 410 core
layout (location = 0) out vec4 out_color;
layout (location = 0) in vec3 passed_color;
layout (location = 1) in vec2 passed_texture_coordinates;

uniform sampler2D uniform_capture;
uniform sampler2D uniform_bloom;
uniform vec2 uniform_curvature;
uniform vec2 uniform_resolution;
uniform vec2 uniform_opacity;
uniform float uniform_vignette_opacity;
uniform float uniform_vignette_roundness;
uniform float uniform_brightness;

vec2 curve_remap(vec2 uv) {
    uv = uv * 2.0 - 1.0;
    vec2 offset = abs(uv.yx) / uniform_curvature;
    uv = uv + uv * offset * offset;
    uv = uv * 0.5 + 0.5;
    return uv;
}

vec4 scan_line(float uv, float resolution, float opacity) {
    const float PI = 3.1415926535897;
    float intensity = sin(uv * resolution * PI * 2.0);
    intensity = ((0.5 * intensity) + 0.5) * 0.9 + 0.1;
    return vec4(vec3(pow(intensity, opacity)), 1.0);
}

vec4 vignette(vec2 uv) {
    float intensity = uv.x * uv.y * (1.0 - uv.x) * (1.0 - uv.y);
    return vec4(vec3(clamp(pow((uniform_resolution.x / uniform_vignette_roundness) * intensity, uniform_vignette_opacity), 0.0, 1.0)), 1.0);
}

void main() {
    vec2 remapped_uv = curve_remap(passed_texture_coordinates);
    if (remapped_uv.x < 0.0 || remapped_uv.y < 0.0 || remapped_uv.x > 1.0 || remapped_uv.y > 1.0) {
        out_color = vec4(0.0);
    } else {
        vec4 capture = texture(uniform_capture, remapped_uv);
        vec4 bloom = texture(uniform_bloom, remapped_uv);
        out_color = mix(capture, bloom, 0.5);
        out_color *= vignette(remapped_uv);
        out_color *= scan_line(remapped_uv.x, uniform_resolution.y, uniform_opacity.x); 
        out_color *= scan_line(remapped_uv.y, uniform_resolution.x, uniform_opacity.y);
        out_color *= vec4(vec3(uniform_brightness), 1.0);
    }
}
