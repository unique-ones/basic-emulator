#version 450 core
layout (location = 0) out vec4 out_color;
layout (location = 0) in vec3 passed_color;
layout (location = 1) in vec2 passed_texture_coordinates;

/// This shader performs downsampling on a texture,
/// as taken from Call Of Duty method, presented at ACM Siggraph 2014.
/// This particular method was customly designed to eliminate
/// so called "pulsating artifacts and temporal stability issues".

uniform sampler2D uniform_frame;
uniform vec2 uniform_resolution;

/// Fetches a texel at the given offset
vec4 fetch_at_offset(float x, float y) {
    return texture(uniform_frame, passed_texture_coordinates + vec2(x, y));
}

void main() {
    vec2 texel_size = 1.0 / uniform_resolution;
    float x = texel_size.x;
    float y = texel_size.y;
    float x2 = 2.0 * x;
    float y2 = 2.0 * y;
    
    /// We need to take 13 samples around the current texel:
    /// a - b - c
    /// - j - k - 
    /// d - e - f
    /// - l - m - 
    /// g - h - i
    /// with e being the current texel:
    vec4 a = fetch_at_offset(-x2, y2);
    vec4 b = fetch_at_offset(0.0, y2);
    vec4 c = fetch_at_offset(x2, y2);
    
    vec4 d = fetch_at_offset(x2, y2);
    vec4 e = fetch_at_offset(0.0, 0.0);
    vec4 f = fetch_at_offset(x2, 0.0);
    
    vec4 g = fetch_at_offset(-x2, -y2);
    vec4 h = fetch_at_offset(0.0, -y2);
    vec4 i = fetch_at_offset(x2, -y2);
    
    vec4 j = fetch_at_offset(-x, y);
    vec4 k = fetch_at_offset(x, y);
    vec4 l = fetch_at_offset(-x, -y);
    vec4 m = fetch_at_offset(x, -y);
    
    /// Apply weighted distribution
    out_color = e * 0.125;
    out_color += (a + c + g + i) * 0.03125;
    out_color += (b + d + f + h) * 0.0625;
    out_color += (j + k + l + m) * 0.125;
}
