//
// Created by Twometer on 2 Feb 2022.
//

#ifndef BLINK_SHADER_SRC_HPP
#define BLINK_SHADER_SRC_HPP

static const char* const SHADER_RECT_VERT = R"|(
#version 330 core

layout (location = 0) in vec4 vertPosTex;

out vec2 fragTexCoord;
out vec2 fragPos;

uniform mat4 mvp_matrix;
uniform vec4 pos_rect;
uniform vec4 tex_rect;

void main() {
    vec2 pos = (vertPosTex.xy * pos_rect.zw) + pos_rect.xy;
    vec2 tex = (vertPosTex.zw * tex_rect.zw) + tex_rect.xy;
    gl_Position = mvp_matrix * vec4(pos, 0.0, 1.0);
    fragPos = pos;
    fragTexCoord = tex;
}
)|";

static const char* const SHADER_RECT_FRAG = R"|(
#version 330 core

uniform vec4 color;

out vec4 fragColor;

void main() {
    fragColor = color;
}
)|";

static const char* const SHADER_GLYPH_FRAG = R"|(
#version 330 core

uniform float time;
uniform sampler2D tex;

in vec2 fragPos;
in vec2 fragTexCoord;

out vec4 fragColor;

vec3 hsl2rgb(vec3 c) {
    vec3 rgb = clamp(abs(mod(c.x*6.0+vec3(0.0,4.0,2.0),6.0)-3.0)-1.0, 0.0, 1.0);
    return c.z + c.y * (rgb-0.5)*(1.0-abs(2.0*c.z-1.0));
}

void main() {
    float alpha = texture(tex, fragTexCoord).r;
    vec4 rainbow = vec4(hsl2rgb(vec3((fragPos.x + fragPos.y) * 0.0025 + time * 0.5, 0.5, 0.5)), 1.0);
    fragColor = vec4(1, 1, 1, alpha) * rainbow;
}
)|";

static const char* const SHADER_CURSOR_FRAG = R"|(
#version 330 core

uniform float time;

in vec2 fragPos;
out vec4 fragColor;

void main() {
    float alpha = abs(sin(3 * time));
    fragColor = vec4(1, 1, 1, alpha);
}
)|";

#endif //BLINK_SHADER_SRC_HPP
