#version 450 core
layout (location = 0) out vec4 out_color;
layout (location = 0) in vec3 passed_color;
layout (location = 1) in vec2 passed_texture_coordinates;

/// This shader performs upsampling on a texture,
/// as taken from Call Of Duty method, presented at ACM Siggraph 2014.

uniform sampler2D uniform_frame;
uniform float uniform_filter_radius;

/// Fetches a texel at the given offset
vec4 fetch_at_offset(float x, float y) {
    return texture(uniform_frame, passed_texture_coordinates + vec2(x, y)); 
}

void main() {
    float x = uniform_filter_radius;
    float y = uniform_filter_radius;
    
    /// Take 9 samples around the current texel:
    /// a - b - c
    /// d - e - f
    /// g - h - i
    /// with e being the current texel:
    vec4 a = fetch_at_offset(-x, y);
    vec4 b = fetch_at_offset(0.0, y);
    vec4 c = fetch_at_offset(x, y);
    
    vec4 d = fetch_at_offset(-x, 0.0);
    vec4 e = fetch_at_offset(0.0, 0.0);
    vec4 f = fetch_at_offset(x, 0.0);
    
    vec4 g = fetch_at_offset(-x, -y);
    vec4 h = fetch_at_offset(0.0, -y);
    vec4 i = fetch_at_offset(x, -y);
    
    /// Apply weighted distribution, by using a 3x3 tent filter:
    out_color = e * 4.0;
    out_color += (b + d + f + h) * 2.0;
    out_color += (a + c + g + i);
    out_color *= 1.0 / 16.0;
}

